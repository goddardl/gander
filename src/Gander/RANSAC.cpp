//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013, Luke Goddard. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Luke Goddard nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Gander/RANSAC.h"

namespace Gander
{

RANSAC::RANSAC( int modelPoints, int maxBasicSolutions )
	: m_seed( -1 ),
	m_modelPoints( modelPoints ),
	m_maxBasicSolutions( maxBasicSolutions ),
	m_nInliers( 0 ),
	m_confidence( .99 ),
	m_maxIters( 2000 ),
	m_maxRefineIters( 10 ),
	m_threshold( .0001 )
{
}

RANSAC::~RANSAC()
{
}

int RANSAC::findInliers(
		const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2,
		const Eigen::MatrixXd &model, Eigen::VectorXd &error, double &avgError,
		std::vector<bool> &mask
	)
{
    int nInliers = 0;
	int nPoints = points1.cols();
	
	avgError = computeModelError( points1, points2, model, error );

    double t = m_threshold * m_threshold;
    for( int i = 0; i < nPoints; i++ )
	{
        nInliers += mask[i] = error[i] <= t;
	}

    return nInliers;
}

int RANSAC::updateNumberOfIterations( double ep, int max_iters )
{
    ep = std::max( std::min( ep, 1. ), 0. );

    // Remove inf's & nan's
    double num = std::max( 1. - m_confidence, std::numeric_limits<double>::min() );
    double denom = 1. - pow( 1. - ep, m_modelPoints );
	
    if( denom < std::numeric_limits<double>::min() )
	{
        return 0;
	}

    num = log( num );
    denom = log( denom );

    return denom >= 0 || -num >= max_iters * ( -denom ) ? max_iters : (int)( round( num / denom ) );
}

bool RANSAC::checkSubset( const Eigen::MatrixXd &pointSamples, unsigned int nSamples )
{
    if( nSamples <= 2 )
	{
        return true;
	}
	unsigned int i = 0;
    for( i = 0; i <= nSamples - 1; ++i )
    {
        // Check that the i-th selected point does not belong
        // to a line connecting some previously selected points
		unsigned int j = 0;
        for( ; j < i; ++j )
        {
            double dx1 = pointSamples(0,j) - pointSamples(0,i);
            double dy1 = pointSamples(1,j) - pointSamples(1,i);
			unsigned int k = 0;
            for( ; k < j; ++k )
            {
                double dx2 = pointSamples(0,k) - pointSamples(0,i);
                double dy2 = pointSamples(1,k) - pointSamples(1,i);
                if( fabs( dx2*dy1 - dy2*dx1 ) <= std::numeric_limits<float>::epsilon() * ( fabs(dx1) + fabs(dy1) + fabs(dx2) + fabs(dy2) ) )
				{
                    break;
				}
            }
            if( k < j )
			{
                break;
			}
        }
        if( j < i )
		{
            break;
		}
    }
    return i >= nSamples;
}

bool RANSAC::subset( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &pointsSample1, Eigen::MatrixXd &pointsSample2, unsigned int maxAttempts )
{
	unsigned int nPoints = points1.cols();
    std::vector<unsigned int> selectionIndices( m_modelPoints );
		
	unsigned int i = 0, iters = 0;
    for( ; iters < maxAttempts; iters++)
    {
        for( i = 0; i < m_modelPoints && iters < maxAttempts; )
        {
			unsigned int currentIndex = random( nPoints ); // Get a new random index from the available points.
            selectionIndices[i] = currentIndex;

			bool pointHasBeenSampledAlready = false;
            for( unsigned int j = 0; j < i; ++j ) // Check that this point hasn't been sampled already.
			{
                if( currentIndex == selectionIndices[j] )
				{
					pointHasBeenSampledAlready = true;
                    break;
				}
			}

            if( pointHasBeenSampledAlready )
			{
                continue;
			}
			
			pointsSample1.col(i) = points1.col( currentIndex ); 
			pointsSample2.col(i) = points2.col( currentIndex );
			++i;
        }
        if( i == m_modelPoints && ( !checkSubset( pointsSample1, i ) || !checkSubset( pointsSample2, i ) ) )
		{
            continue;
		}
        break;
    }

    return i == m_modelPoints && iters < maxAttempts;
}
	
bool RANSAC::operator()( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model, std::vector<bool> &initialMask )
{
	// Validate our input parameters.
    m_confidence = std::max( std::min( m_confidence, 1. ), 0. );
	
	// Initialize the random number generator.
	srand( m_seed );
	
	unsigned int mostInliersFound = 0, niters = m_maxIters, nPoints = points1.cols();
	double bestError = std::numeric_limits<double>::max();
    bool result = false;
	
	if( m_modelPoints < 1 )
	{
		throw std::runtime_error( "RANSAC: A least one point is required to compute the model." );
	}
	
	if( points1.rows() != points2.rows() || points1.cols() != points2.cols() )
	{
		throw std::runtime_error( "RANSAC: Point lists have different numbers of elements." );
	}
	
	/// Test if there are enough points to proceed. 
	if( nPoints < m_modelPoints )
	{
        return false;
	}
	
	if( initialMask.size() != nPoints )
	{
		initialMask.resize( nPoints, false );
	}

	Eigen::MatrixXd pointsSample1, pointsSample2; // Holds the points currently being tested.
	std::vector<bool> tmask( nPoints ); 
	Eigen::MatrixXd models( model.rows() * m_maxBasicSolutions, model.cols() ); // Holds all of the models we are testing.
	Eigen::VectorXd error( nPoints ); // One error per point.

	/// Is this an overdetermind system?
	bool isOverdetermind( nPoints > m_modelPoints );
    if( isOverdetermind )
    {
        pointsSample1.resize( points1.rows(), m_modelPoints );
        pointsSample2.resize( points2.rows(), m_modelPoints );
    }
    else // This isn't an overdetermind system so we require all of the input points and only one iteration.
    {
        niters = 1;
        pointsSample1 = points1;
        pointsSample2 = points2;
    }
	unsigned int c = 0;	
	for( unsigned int iter = 0; iter < niters; ++iter )
    {
		// Get a subset of the points to test.
        if( isOverdetermind )
        {
            bool found = subset( points1, points2, pointsSample1, pointsSample2, 300 );
            if( !found )
            {
                if( iter == 0 )
				{
                    return false;
				}
                break;
            }
        }

		// Estimate the models to test from the point selection.
        unsigned int nModels = runKernel( pointsSample1, pointsSample2, models );
       
		// If the model estimation failed then try again... 
		if( nModels <= 0 )
		{
            continue;
		}
		
		// Test each of the computed models.
        for( unsigned int i = 0; i < nModels; i++ )
        {
			// Get the current model.
            Eigen::MatrixXd currentModel( model.rows(), model.cols() );
			currentModel = models.block( i * model.rows(), 0, model.rows(), model.cols() );
	
			// Use the model to divide the points into inliers and outliers.
			double avgError;
            int nInliers = findInliers( points1, points2, currentModel, error, avgError, tmask );

			// If the model was better than any so far then keep it.
			int requiredInliers = std::max( mostInliersFound, m_modelPoints-1 );
            if( nInliers > requiredInliers || ( avgError > bestError && nInliers == requiredInliers ) )
            {
                std::swap( tmask, initialMask );
                model = currentModel;
                mostInliersFound = nInliers;
				bestError = avgError;
                niters = updateNumberOfIterations( double( m_modelPoints - nInliers ) / m_modelPoints, niters );
            }
        }
		c++;
    }
	
	if( mostInliersFound > 0 )
    {
        result = true;
    }
  
	// If we found the best model, refine it using only the inliers that fit it.
	if( result )
	{
		Eigen::MatrixXd inliers1( 2, mostInliersFound );
		Eigen::MatrixXd inliers2( 2, mostInliersFound );
		unsigned int idx = 0;
		for( unsigned int i = 0; i < nPoints; ++i )
		{
			if( initialMask[i] )
			{
				inliers1.col(idx) = points1.col(i);
				inliers2.col(idx) = points2.col(i);
				++idx;
			}
		}

		result = refine( inliers1, inliers2, model, m_maxRefineIters );
	}
	
	return result;
}

}; // namespace Gander

