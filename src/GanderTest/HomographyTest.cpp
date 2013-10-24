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

#include <iostream>
#include <cstdlib>

#include "GanderTest/HomographyTest.h"
#include "Gander/Math.h"
#include "Gander/Homography.h"

#include "Eigen/Geometry"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace GanderTest;
using namespace boost;
using namespace boost::unit_test;

namespace GanderTest
{

void testMatrices( Eigen::MatrixXd &matrix1, Eigen::MatrixXd &matrix2, int inliers, int outliers, bool noise, Eigen::Transform<double, 2, Eigen::Affine> &transform )
{
	srand(1);

	int totalPoints( inliers + outliers );
	matrix1.resize( 2, totalPoints );
	matrix2.resize( 2, totalPoints );

	for( int i = 0; i < inliers; ++i )
	{
		Eigen::Vector2d p1( rand() % 10,  rand() % 10 );
		matrix1.col(i) = p1;
		matrix2.col(i) = transform * p1;
	}

	for( int i = 0; i < outliers; ++i )
	{
		Eigen::Vector2d p1( rand() % 10,  rand() % 10 );
		Eigen::Vector2d p2( rand() % 10,  rand() % 10 );
		matrix1.col(inliers+i) = p1;
		matrix2.col(inliers+i) = p2;
	}

	if( noise )
	{
		for( int i = 0; i < totalPoints; ++i )
		{
			matrix1(0,i) = matrix1(0,i) + ( rand() % 10 ) / 10. - .5;
			matrix1(1,i) = matrix1(1,i) + ( rand() % 10 ) / 10. - .5;
			matrix2(0,i) = matrix2(0,i) + ( rand() % 10 ) / 10. - .5;
			matrix2(1,i) = matrix2(1,i) + ( rand() % 10 ) / 10. - .5;
		}
	}
}

struct HomographyTest
{
	void testFourPointHomography()
	{
		try
		{
			double angleInRadians = 45 * 0.0174532925;
			Eigen::Rotation2D<double> rotation( angleInRadians );
			Eigen::Translation2d translation( 1., .7 );
			Eigen::Transform<double, 2, Eigen::Affine> transform( translation * rotation );

			// Build a homography from a set of 4 point correspondences.
			Eigen::MatrixXd points1, points2, H;
			Eigen::VectorXd error;

			// Create the test matrices with 4 inliers and no outliers.
			testMatrices( points1, points2, 4, 0, false, transform );
			compute4PointPlaneToPlaneHomography( points1, points2, H );
			
			// Check that the homography is the same as the transform we used to construct the points.
			for( int j = 0; j < H.cols(); ++j )
			{
				for( int i = 0; i < H.rows(); ++i )
				{
					BOOST_CHECK_SMALL( fabs( H(i,j) - transform.matrix()(i,j) ), 10e-10 );
				}
			}

			double avgError = computePlaneToPlaneHomographyError( points1, points2, H, error );
			BOOST_CHECK_SMALL( avgError, 10e-10 );
		}
		catch ( std::exception &e ) 
		{
			BOOST_WARN( !e.what() );
			BOOST_CHECK( !"Exception thrown during HomographyTest." );
		}
	}
	
	void testHomographyRefinement()
	{
		try
		{
			double angleInRadians = 40 * 0.0174532925;
			Eigen::Rotation2D<double> rotation( angleInRadians );
			Eigen::Translation2d translation( .5, .7 );
			Eigen::Transform<double, 2, Eigen::Affine> transform( translation * rotation );

			// Build a homography from a set of 4 point correspondences.
			Eigen::MatrixXd points1, points2, H;
			Eigen::VectorXd error;
		
			//  Create the test matrices with 4 inliers and no outliers but a small amount of noise.
			testMatrices( points1, points2, 4, 0, true, transform );
			compute4PointPlaneToPlaneHomography( points1, points2, H );
			double avgError = computePlaneToPlaneHomographyError( points1, points2, H, error );
			BOOST_CHECK( avgError < 10e-10 ); // There should be no error with the Homography if we created it from 4 points.
	
			// Now create a set of points that also have a little noise and check that the homography
			// has an expected amount of error when trying to transform one set to the other.
			unsigned int inliers = 8;
			testMatrices( points1, points2, inliers, 0, true, transform );
			double originalError = computePlaneToPlaneHomographyError( points1, points2, H, error );
			BOOST_CHECK( originalError > 10e-3 ); // We expect error because of the noise in the points

			// Refine the Homography using Levenberg Marquardt. As the points that we are using to 
			// refine it have no noise, the result should be a Homography which is almost identical
			// to the original transform.
			refineHomography( points1, points2, H, 15 ); // Use a maximum of 15 iterations.
			double refinedError = computePlaneToPlaneHomographyError( points1, points2, H, error );
			BOOST_CHECK( refinedError < originalError );
		}
		catch ( std::exception &e ) 
		{
			BOOST_WARN( !e.what() );
			BOOST_CHECK( !"Exception thrown during HomographyTest." );
		}
	}
	
	// Test the computation of a Homography in with the presence of outliers.
	void testHomographyRANSAC()
	{
		try
		{
			// Build a transform to test.
			double angleInRadians = 40 * 0.0174532925;
			Eigen::Rotation2D<double> rotation( angleInRadians );
			Eigen::Translation2d translation( -1.1, 3.2 );
			Eigen::Transform<double, 2, Eigen::Affine> transform( translation * rotation );
			
			// Create the test matrices with 10 inliers, 9 outliers and a small amount of noise.
			Eigen::MatrixXd points1, points2, H;
			Eigen::VectorXd error;
			testMatrices( points1, points2, 20, 9, true, transform );
		
			// Compute the homography and then check to see if all of the outliers were discarded.	
			std::vector<bool> mask;
			computePlaneToPlaneHomography( points1, points2, H, mask, 1 );
			double avgError = computePlaneToPlaneHomographyError( points1, points2, H, error );
			double ransacError = 0.;
			int nPoints = 0;
			bool outlierExcluded = true;
			for( unsigned int i = 0; i < points1.cols(); ++i )
			{
				if( mask[i] )
				{
					++nPoints;
					ransacError += error[i];
					if( i >= 20 )
					{
						outlierExcluded = false;
					}
				}
			}
			ransacError /= double( nPoints );
			
			BOOST_CHECK( outlierExcluded  ); // We should not find any outliers in our sample if RANSAC worked correctly.
			BOOST_CHECK( ransacError < 2. ); // And the error of the RANSAC should be less than our error tolerance that we set. 
			BOOST_CHECK( avgError > ransacError ); // Check that the ransac result is better than when we include all suspected outliers.
		}
		catch ( std::exception &e ) 
		{
			BOOST_WARN( !e.what() );
			BOOST_CHECK( !"Exception thrown during HomographyTest." );
		}
	}
};

struct HomographyTestSuite : public boost::unit_test::test_suite
{
	HomographyTestSuite() : boost::unit_test::test_suite( "HomographyTestSuite" )
	{
		boost::shared_ptr<HomographyTest> instance( new HomographyTest() );
		add( BOOST_CLASS_TEST_CASE( &HomographyTest::testFourPointHomography, instance ) );
		add( BOOST_CLASS_TEST_CASE( &HomographyTest::testHomographyRefinement, instance ) );
		add( BOOST_CLASS_TEST_CASE( &HomographyTest::testHomographyRANSAC, instance ) );
	}
};

void addHomographyTest( boost::unit_test::test_suite *test )
{
	test->add( new HomographyTestSuite( ) );
}

} // namespace GanderTest

