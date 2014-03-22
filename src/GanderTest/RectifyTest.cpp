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

#include "GanderTest/RectifyTest.h"
#include "GanderTest/TestTools.h"

#include "Eigen/Geometry"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace GanderTest;
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

	Eigen::Matrix3d rotation;
	rotation = rz * ry * rx;

	Eigen::Matrix3d calibration;
	calibration << 2.1875, 0, 0.0141111, 0, 2.1875, 0.127, 0, 0, 1.;

	// Construct the projection matrix.
	P << calibration * rotation, txyz;
}

namespace GanderTest
{
	struct RectifyTest
	{
		void testDecomposeRQ3x3()
		{
			try
			{
				srand(1);

				Eigen::MatrixXd P( 3, 4 );
				testProjectionMatrix( P, Eigen::Vector3d( 0., 10., 5. ), Eigen::Vector3d( 0, 0, -1 ) );

				// Decompose the projection matrix into it's basic components.
				Eigen::Matrix3d C, R;
				Eigen::Vector3d T;

				decomposeProjection( P, C, R, T );
				
				Eigen::Vector3d p( 0, 0, 0 );
				p = P * p;
				std::cerr << p << std::endl;
				
				// Get the optical center.
				Eigen::Vector3d c1;
				c1 = -R * T;
				std::cerr << c1 << std::endl;
			
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
		}
	};

	void addRectifyTest( boost::unit_test::test_suite *test )
	{
		test->add( new RectifyTestSuite( ) );
	}

} // namespace GanderTest

