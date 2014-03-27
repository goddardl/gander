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
#include <iostream>

#include "unsupported/Eigen/NonLinearOptimization"

#include "Gander/Homography.h"
#include "Gander/RANSAC.h"
#include "Gander/ErrorFunctions.h"

namespace Gander
{

namespace Detail
{

// The Levenberg Marquardt function.
class HomographyLeastSquaresFn : public Gander::ErrorFn
{

public:
	
	HomographyLeastSquaresFn( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2 ) :
		Gander::ErrorFn( points1.cols(), 2 ),
		m_points1( points1 ),
		m_points2( points2 )
	{
	}

	int operator()( const Eigen::VectorXd &x, Eigen::VectorXd &fvec ) const
	{
		// Build the homography matrix that we are going to refine.
		Eigen::Matrix3d H;
		H << x(0), x(3), x(6), x(1), x(4), x(7), x(2), x(5), 1.;
		
		// Test the transformation of each point using the new homography.
		const unsigned int nPoints( m_points1.cols() );
		for( unsigned int i = 0; i < nPoints; i++ )
		{
			Eigen::Vector3d p1 = H * m_points1.col(i).homogeneous();
			Eigen::Vector3d p2 = m_points2.col(i).homogeneous();
			p1 /= p1[2];
			double err = (p1 - p2).norm();
			fvec(i) = err * err;
		}
		return 0;
	}

private :

	const Eigen::MatrixXd &m_points1;
	const Eigen::MatrixXd &m_points2;

};

// The RANSAC estimator class.
class HomographyEstimator : public Gander::RANSAC
{

public :

    HomographyEstimator( int modelPoints = 4 )
		: RANSAC( std::max( modelPoints, 4 ), 1 )
	{
	}

protected :
	
	virtual int runKernel( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model )
	{
		return compute4PointPlaneToPlaneHomography( points1, points2, model ) == true ? 1 : 0;
	}

	virtual double computeModelError( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, const Eigen::MatrixXd &model, Eigen::VectorXd &error )
	{
		return computePlaneToPlaneHomographyError( points1, points2, model, error );
	}

};

} // namespace Detail

bool refineHomography( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &H, int maxIters )
{
	Detail::HomographyLeastSquaresFn functor( points1, points2 );
	ForwardDifferenceJacobian< Detail::HomographyLeastSquaresFn, double > fn( functor );
	Eigen::LevenbergMarquardt< ForwardDifferenceJacobian< Detail::HomographyLeastSquaresFn, double >, double > lm( fn );

	Eigen::VectorXd x(8);
	x << H(0,0), H(1,0), H(2,0), H(0,1), H(1,1), H(2,1), H(0,2), H(1,2);
	
	lm.parameters.ftol = 10e-6;
	lm.parameters.xtol = 10e-6;
	lm.parameters.maxfev = maxIters;
	lm.minimize(x);

	H << x(0), x(3), x(6), x(1), x(4), x(7), x(2), x(5), 1.;
	
	return true;
}

double computePlaneToPlaneHomographyError(
		const Eigen::MatrixXd &points1,
		const Eigen::MatrixXd &points2,
		const Eigen::MatrixXd &H,
		Eigen::VectorXd &error
	)
{
	double sum = 0;
	unsigned int nPoints = points1.cols();
	error.resize( nPoints );

	for( unsigned int i = 0; i < nPoints; i++ )
	{
		Eigen::Vector3d p1 = H * points1.col(i).homogeneous();
		Eigen::Vector3d p2 = points2.col(i).homogeneous();
		p1 /= p1[2];
		double err = (p1 - p2).norm();
		sum += error[i] = err * err;
	}
	return sum / double( nPoints );
}

bool computePlaneToPlaneHomography( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &H,
	std::vector<bool> &mask, double reprojectionErrorThreshold )
{
	if( points1.size() != points2.size() )
	{
		throw std::runtime_error( "computePlaneToPlaneHomography: Point lists must be of the same length." );
	}

	if( H.cols() != 3 || H.rows() != 3 )
	{
		H.resize( 3, 3 );
	}

	bool result = false;
		
	// If we have exactly 4 points, compute the homography.
	if( points1.size() == 4 )
	{
		result = compute4PointPlaneToPlaneHomography( points1, points2, H );
	}
	else
	{
		// Otherwise, use RANSAC to remove the outliers.
		Detail::HomographyEstimator estimator(4);
		estimator.setThreshold( reprojectionErrorThreshold );
		result = estimator( points1, points2, H, mask );
	}

	return result;
}

// Compute the homography matrix using a Homogeneous solution and minimize using SVD.
// See the following link for details...
// http://www.robots.ox.ac.uk/~vgg/presentations/bmvc97/criminispaper/node3.html
bool compute4PointPlaneToPlaneHomography( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &H )
{
	if( points1.cols() != points2.cols() || points1.rows() != points2.rows() )
	{
		throw std::runtime_error( "computePlaneToPlaneHomography: Point lists must be of the same length." );
	}
	
	if( points1.rows() != 2 )
	{
		throw std::runtime_error( "computePlaneToPlaneHomography: Only 2D point correspondences may be specified.\
			Make sure that matrix points1 and points2 only have 2 rows." );
	}
	
	const unsigned int nPoints = points1.cols();
	if( nPoints != 4 )
	{
		throw std::runtime_error( "computePlaneToPlaneHomography: Only 4 point correspondences may be specified." );
	}
	
	if( H.cols() != 3 || H.rows() != 3 )
	{
		H.resize( 3, 3 );
	}

	// To aid the decomposition we normalize the two point clouds by moving their centroid to the origin and then scaling them so the average distance
	// to the origin is sqrt(s).
	Eigen::Vector2d centroid1( 0., 0. );
	Eigen::Vector2d centroid2( 0., 0. );
	for( unsigned int i = 0; i < nPoints; ++i )
	{
		centroid1 += points1.col(i);
		centroid2 += points2.col(i);
	}
	centroid1 /= double( nPoints );
	centroid2 /= double( nPoints );

	Eigen::Vector2d sm1( 0., 0. );
	Eigen::Vector2d sm2( 0., 0. );
    for( unsigned int i = 0; i < nPoints; ++i )
    {
        sm1[0] += fabs( points1(0,i) - centroid1[0] );
        sm2[0] += fabs( points2(0,i) - centroid2[0] );
        sm1[1] += fabs( points1(1,i) - centroid1[1] );
        sm2[1] += fabs( points2(1,i) - centroid2[1] );
    }
    
	if( fabs( sm1[0] ) < std::numeric_limits<double>::epsilon() 
			|| fabs( sm1[1] ) < std::numeric_limits<double>::epsilon() 
			|| fabs( sm2[0] ) < std::numeric_limits<double>::epsilon() 
			|| fabs( sm2[1] ) < std::numeric_limits<double>::epsilon()
		)
	{
        return false;
	}

	sm1[0] = double( nPoints ) / sm1[0];
	sm1[1] = double( nPoints ) / sm1[1];
	sm2[0] = double( nPoints ) / sm2[0];
	sm2[1] = double( nPoints ) / sm2[1];

	// Build the matrices that can normalize and un-normalize the points.	
	Eigen::MatrixXd invHnorm(3,3), Hnorm2(3,3);
	invHnorm << 1. / sm2[0], 0., 0., 0., 1. / sm2[1], 0., centroid2[0], centroid2[1], 1.;
	Hnorm2 << sm1[0], 0., 0., 0., sm1[1], 0., -centroid1[0]*sm1[0], -centroid1[1]*sm1[1], 1.;
	
	Eigen::MatrixXd A = Eigen::MatrixXd::Zero(9, 9);
	for( unsigned int i = 0; i < nPoints; ++i )
	{
		const Eigen::Vector2d &p1( points1.col(i) );
		const Eigen::Vector2d &p2( points2.col(i) );
        
        double x1 = ( p1[0] - centroid1[0] ) * sm1[0], y1 = ( p1[1] - centroid1[1] ) * sm1[1];
        double x2 = ( p2[0] - centroid2[0] ) * sm2[0], y2 = ( p2[1] - centroid2[1] ) * sm2[1];
        double Lx[] = { x1, y1, 1, 0, 0, 0, -x2*x1, -x2*y1, -x2 };
        double Ly[] = { 0, 0, 0, x1, y1, 1, -y2*x1, -y2*y1, -y2 };
        for( int j = 0; j < 9; j++ )
		{
            for( int k = j; k < 9; k++ )
			{
                A(j,k) += Lx[j]*Lx[k] + Ly[j]*Ly[k];
			}
		}
	}

	///\todo: Use the eigen SelfAdjointEigenSolver instead of SVD to solve the eigen vectors of the symteric matrix
	/// so that we don't have to make it symetric here...
	for( int j = 0; j < 9; ++j )
	{
		for( int k = j; k < 9; ++k )
		{
			A(k,j) = A(j,k);
		}
	}
	Eigen::JacobiSVD<Eigen::MatrixXd> svdOfA( A, Eigen::ComputeFullV );

	const Eigen::MatrixXd V = svdOfA.matrixV();
	for( int i = 0, k = 0; i < 3; ++i )
	{
		for( int j = 0; j < 3; ++j, ++k )
		{
			H(j,i) = svdOfA.matrixV().col(8).coeffRef(k);
		}
	}

	H = Hnorm2 * H * invHnorm;
	H /= H(2,2);
	H.transposeInPlace();

	return true;
}

}; // namespace Gander

