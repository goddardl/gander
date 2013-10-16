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
#include <stdexcept>
#include <cmath>

#include "Gander/RANSAC.h"

namespace Gander
{

RANSAC::RANSAC( int modelPoints, int maxBasicSolutions )
	: m_seed( -1 ),
	m_modelPoints( modelPoints ),
	m_maxBasicSolutions( maxBasicSolutions )
{
}

RANSAC::~RANSAC()
{
}

int RANSAC::getSeed() const
{
	return m_seed;
}

void RANSAC::setSeed( int seed )
{
    m_seed = seed;
	srand( m_seed );
}
	
int RANSAC::random( int maxValue ) const
{
	return rand() % maxValue;
}

int RANSAC::findInliers(
		const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2,
		const Eigen::MatrixXd &model, Eigen::VectorXd &error,
		std::vector<bool> &mask, double threshold
	)
{
    int goodCount = 0;
	int nPoints = points1.cols();

    computeReprojError( points1, points2, model, error );

    threshold *= threshold;
    for( int i = 0; i < nPoints; i++ )
	{
        goodCount += mask[i] = error[i] <= threshold;
	}

    return goodCount;
}

int RANSAC::updateNumberOfIterations( double p, double ep, int model_points, int max_iters )
{
    if( model_points <= 0 )
	{
		throw std::runtime_error( "RANSAC: The number of model points should be positive." );
	}

    p = std::max( std::min( p, 1. ), 0. );
    ep = std::max( std::min( ep, 1. ), 0. );

    // Remove inf's & nan's
    double num = std::max( 1. - p, std::numeric_limits<double>::min() );
    double denom = 1. - pow( 1. - ep, model_points );
	
    if( denom < std::numeric_limits<double>::min() )
	{
        return 0;
	}

    num = log( num );
    denom = log( denom );

    return denom >= 0 || -num >= max_iters * ( -denom ) ? max_iters : (int)( round( num / denom ) );
}

double RANSAC::round( double num ) const
{
	return (num > 0.0) ? floor( num + 0.5 ) : ceil( num - 0.5 );
}

bool RANSAC::checkSubset( const Eigen::MatrixXd &pointSamples, int nSamples )
{
    if( nSamples <= 2 )
	{
        return true;
	}
	int i = 0;
    for( i = 0; i <= nSamples - 1; ++i )
    {
        // Check that the i-th selected point does not belong
        // to a line connecting some previously selected points
		int j = 0;
        for( ; j < i; ++j )
        {
            double dx1 = pointSamples(0,j) - pointSamples(0,i);
            double dy1 = pointSamples(1,j) - pointSamples(1,i);
			int k = 0;
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

bool RANSAC::getSubset( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &pointsSample1, Eigen::MatrixXd &pointsSample2, int maxAttempts )
{
	int nPoints = points1.cols();
    std::vector<int> selectionIndices( m_modelPoints );
		
	int i = 0, iters = 0;
    for( ; iters < maxAttempts; iters++)
    {
        for( i = 0; i < m_modelPoints && iters < maxAttempts; )
        {
			int currentIndex = random( nPoints ); // Get a new random index from the available points.
            selectionIndices[i] = currentIndex;

			bool pointHasBeenSampledAlready = false;
            for( int j = 0; j < i; ++j ) // Check that this point hasn't been sampled already.
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

bool RANSAC::run( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model, std::vector<bool> &initialMask, double threshold, double confidence, int maxIters )
{
	int maxGoodCount = 0;
	int niters = maxIters;
	int nPoints = points1.cols();
    bool result = false;
	
	if( points1.rows() != points2.rows() || points1.cols() != points2.cols() )
	{
		throw std::runtime_error( "RANSAC: Point lists have different numbers of elements." );
	}
	
	/// Test if there are enough points to proceed. 
	if( nPoints < m_modelPoints )
	{
        return false;
	}
	
	if( initialMask.empty() )
	{
		initialMask.resize( nPoints, true );
	}

	Eigen::MatrixXd pointsSample1, pointsSample2; // Holds the points currently being tested.
	std::vector<bool> mask( initialMask );
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
		
	for( int iter = 0; iter < niters; ++iter )
    {
		// Get a subset of the points to test.
        if( isOverdetermind )
        {
            bool found = getSubset( points1, points2, pointsSample1, pointsSample2, 300 );
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
        int nModels = runKernel( pointsSample1, pointsSample2, models );
       
		// If the model estimation failed then try again... 
		if( nModels <= 0 )
		{
            continue;
		}
		
		// Test each of the computed models.
        for( int i = 0; i < nModels; i++ )
        {
			// Get the current model.
            Eigen::MatrixXd currentModel( model.rows(), model.cols() );
			currentModel = models.block( i * model.rows(), 0, model.rows(), model.cols() );
	
			// Use the model to divide the points into inliers and outliers.
            int goodCount = findInliers( points1, points2, currentModel, error, tmask, threshold );

			// If the model was better than any so far then keep it.
            if( goodCount > std::max( maxGoodCount, m_modelPoints-1 ) )
            {
                std::swap( tmask, mask );
                model = currentModel;
                maxGoodCount = goodCount;
                niters = updateNumberOfIterations( confidence, double( m_modelPoints - goodCount ) / m_modelPoints, m_modelPoints, niters );
            }
        }
    }
	
	if( maxGoodCount > 0 )
    {
		initialMask = mask;
        result = true;
    }
    
	return result;
}

}; // namespace Gander

