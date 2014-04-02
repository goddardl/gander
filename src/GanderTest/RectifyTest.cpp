//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014, Luke Goddard. All rights reserved.
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

#include "Gander/DecomposeRQ3x3.h"
#include "Gander/AngleConversion.h"
#include "Gander/AreClose.h"
#include "Gander/Assert.h"

#include "GanderTest/RectifyTest.h"
#include "GanderTest/TestTools.h"

#include "Eigen/Geometry"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Test;
using namespace boost;
using namespace boost::unit_test;

void decomposeProjection( const Eigen::MatrixXd &P, Eigen::Matrix3d &C, Eigen::Matrix3d &R, Eigen::Vector3d &T )
{
	// Extract the translation and the affine transformation.
	Eigen::Matrix3d A = P.block( 0, 0, 3, 3 );
	T = P.col(3).head(3);
	
	// Decompose using Givens RQ decomposition.
	Eigen::Matrix3d Qx, Qy, Qz;
	givensDecomposeRQ3x3( A, C, R, Qx, Qy, Qz );
}

void testProjectionMatrix( Eigen::MatrixXd &P, const Eigen::Vector3d &rxyz, const Eigen::Vector3d &txyz )
{
	/// \todo We should assert that the projection matrix is a 3x4 matrix here.

	// Create a rotation matrix using individual rotational components.
	Eigen::Matrix3d rx;
	rx = Eigen::AngleAxisd( rxyz[0], Eigen::Vector3d( 1, 0, 0 ) );

	Eigen::Matrix3d ry;
	ry = Eigen::AngleAxisd( rxyz[1], Eigen::Vector3d( 0, 1, 0 ) );

	Eigen::Matrix3d rz;
	rz = Eigen::AngleAxisd( rxyz[2], Eigen::Vector3d( 0, 0, 1 ) );

	Eigen::Matrix3d calibration;
	calibration << 2.1875, 0, -2.1875 * .5, 0, 2.1875, -2.1875 * .5, 0, 0, 1.;
	
	Eigen::Matrix3d rotation;
	rotation = ( rz * ry * rx ).inverse();
	P << calibration * rotation, -rotation * txyz;
}

/// Creates a calibration matrix.
/// The focal length and aperture should be specified in mm.
void calibrationMatrix( Eigen::Matrix3d &C, double focalLength, const Eigen::Vector2d &aperture, const Eigen::Vector2d &resolution )
{
	const double pixelWidth = aperture[0] / resolution[0];
	const double pixelHeight = aperture[1] / resolution[1];
	C << focalLength / pixelWidth, 0., resolution[0] * .5, 0, focalLength / pixelHeight, resolution[1] * .5, 0, 0, 1.;
}

void projectionMatrix( Eigen::MatrixXd &projection, const Eigen::Matrix3d &calibration, const Eigen::Vector3d &XYZ, const Eigen::Vector3d &T )
{
	GANDER_ASSERT( projection.cols() == 4 && projection.rows() == 3, "Matrix P must be of size 3x4." );

	// Create a rotation matrix using individual rotational components.
	Eigen::Matrix3d rx;
	rx = Eigen::AngleAxisd( XYZ[0], Eigen::Vector3d( 1, 0, 0 ) );

	Eigen::Matrix3d ry;
	ry = Eigen::AngleAxisd( XYZ[1], Eigen::Vector3d( 0, 1, 0 ) );

	Eigen::Matrix3d rz;
	rz = Eigen::AngleAxisd( XYZ[2], Eigen::Vector3d( 0, 0, 1 ) );

	Eigen::Matrix3d rotation;
	rotation = ( rz * ry * rx ).inverse();
	projection << calibration * rotation, -rotation * XYZ;
}

Eigen::Vector3d worldToPixel( const Eigen::MatrixXd &projection, const Eigen::Vector3d &point )
{
	Eigen::Vector3d suv = projection * ( Eigen::Vector4d() << point, 1. ).finished();
	return suv / suv[2];
}

namespace Gander
{

namespace Test
{
	struct RectifyTest
	{
		void testProjection()
		{
			Eigen::Vector2d aperture( 1.417 * 25.4, 0.945 * 25.4 ); // Aperture in mm.
			Eigen::Vector2d resolution( 640., 480. ); // Resolution in pixels.
			double f = 34; // Focal length in mm.
		
			// Build a calibration matrix.
			Eigen::Matrix3d C;
			calibrationMatrix( C, f, aperture, resolution );

			// Build a projection matrix without any transformation.
			Eigen::MatrixXd P( 3, 4 );
			projectionMatrix( P, C, Eigen::Vector3d( 0, 0, 0 ), Eigen::Vector3d( 0, 0, 0 ) );

			// Check some basic projections of points to the image plane.
			BOOST_CHECK( areClose( worldToPixel( P, Eigen::Vector3d( 0, 0, f ) ), Eigen::Vector3d( resolution[0] * .5, resolution[1] * .5, 1. ), 10e-8, 10e-8 ) );
			BOOST_CHECK( areClose( worldToPixel( P, Eigen::Vector3d( -aperture[0] * .5, -aperture[1] * .5, f ) ), Eigen::Vector3d( 0., 0., 1. ), 10e-8, 10e-8 ) );
			BOOST_CHECK( areClose( worldToPixel( P, Eigen::Vector3d( aperture[0] * .5, aperture[1] * .5, f ) ), Eigen::Vector3d( resolution[0], resolution[1], 1. ), 10e-8, 10e-8 ) );
			
			projectionMatrix( P, C, Eigen::Vector3d( Gander::degreesToRadians( 30. ), Gander::degreesToRadians( -10. ), Gander::degreesToRadians( 5. ) ), Eigen::Vector3d( 1., -.5, 2. ) );
			test the transforms
		}

		void testDecomposeRQ3x3()
		{
			try
			{
				srand(1);

				Eigen::MatrixXd P1( 3, 4 ), P2( 3, 4 ), Pn1( 3, 4 ), Pn2( 3, 4 );
				testProjectionMatrix( P1, Eigen::Vector3d( Gander::degreesToRadians( 1.5 ), Gander::degreesToRadians( 5. ), Gander::degreesToRadians( 0. ) ), Eigen::Vector3d( 1.25, .01, -0.025 ) );
				testProjectionMatrix( P2, Eigen::Vector3d( Gander::degreesToRadians( -1. ), Gander::degreesToRadians( -4. ), Gander::degreesToRadians( 0. ) ), Eigen::Vector3d( -1., .3, -.1 ) );

				// Decompose the projection matrix into it's basic components.
				Eigen::Matrix3d C1, C2, R1, R2, R, C, Q1, Q2;
				Eigen::Vector3d T1, T2;

				decomposeProjection( P1, C1, R1, T1 );
				decomposeProjection( P2, C2, R2, T2 );

				// Get the optical centers.
				Eigen::Vector3d c1, c2;
				c1 = R1.inverse() * -T1;
				BOOST_CHECK( areClose( c1, Eigen::Vector3d( 1.25, .01, -0.025 ), 10e-8, 10e-8 ) );				
				
				c2 = R2.inverse() * -T2;
				BOOST_CHECK( areClose( c2, Eigen::Vector3d( -1., .3, -0.1 ), 10e-8, 10e-8 ) );				
		
				// Calculate the new X axis (the direction of the baseline).
				Eigen::Vector3d v1 = c1 - c2;
				
				// Calculate the new Y axis (orthogonal to the new X and the old Z).
				Eigen::Vector3d v2 = R1.row(2).transpose().cross( v1 );

				// Calculate the new Z axes (orthogonal to the baseline and Y).
				Eigen::Vector3d v3 = v1.cross(v2);

				// New extrinsic parameters (translation is left unchanged).
				R << ( v1.adjoint() / v1.norm() ), ( v2.adjoint() / v2.norm() ), ( v3.adjoint() / v3.norm() );

				// New intrinsic parameters (arbitrary).
				C = ( C1 + C2 ) / 2.;
				C( 0, 1 ) = 0.; // No skew.

				// New projection matrices.
				Pn1 = C * ( Eigen::MatrixXd( 3, 4 ) << R, -R*c1 ).finished();
				Pn2 = C * ( Eigen::MatrixXd( 3, 4 ) << R, -R*c2 ).finished();

				// Rectifying image transformation.
				Q1 = Pn1.block( 0, 0, 2, 2 ) * P1.block( 0, 0, 2, 2 ).inverse();
				Q2 = Pn2.block( 0, 0, 2, 2 ) * P2.block( 0, 0, 2, 2 ).inverse();
			}
			catch ( std::exception &e ) 
			{
				BOOST_WARN( !e.what() );
				BOOST_CHECK( !"Exception thrown during RectifyTest." );
			}
		}
	};

	struct RectifyTestSuite : public boost::unit_test::test_suite
	{
		RectifyTestSuite() : boost::unit_test::test_suite( "RectifyTestSuite" )
		{
			boost::shared_ptr<RectifyTest> instance( new RectifyTest() );
			add( BOOST_CLASS_TEST_CASE( &RectifyTest::testDecomposeRQ3x3, instance ) );
			add( BOOST_CLASS_TEST_CASE( &RectifyTest::testProjection, instance ) );
		}
	};

	void addRectifyTest( boost::unit_test::test_suite *test )
	{
		test->add( new RectifyTestSuite( ) );
	}

} // namespace Gander

} // namespace Test

