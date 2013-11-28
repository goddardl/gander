//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
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

#include "GanderTest/DecomposeRQ3x3Test.h"
#include "GanderTest/TestTools.h"

#include "Eigen/Jacobi"
#include "Eigen/Geometry"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace GanderTest;
using namespace boost;
using namespace boost::unit_test;

void decomposeTest( const Eigen::Vector3d &xyz, const Eigen::Matrix3d &C )
{
	// Create a rotation matrix using individual rotational components.
	Eigen::Matrix3d rx;
	rx = Eigen::AngleAxisd( xyz[0], Eigen::Vector3d( 1, 0, 0 ) );

	Eigen::Matrix3d ry;
	ry = Eigen::AngleAxisd( xyz[1], Eigen::Vector3d( 0, 1, 0 ) );

	Eigen::Matrix3d rz;
	rz = Eigen::AngleAxisd( xyz[2], Eigen::Vector3d( 0, 0, 1 ) );

	Eigen::Matrix3d rotation;
	rotation = rz * ry * rx;

	// Combine the calibration and rotation matrix.
	Eigen::Matrix3d A;
	A = rotation * C;

	// Decompose using Givens RQ decomposition.
	Eigen::Matrix3d R, Q, Qx, Qy, Qz;
	givensDecomposeRQ3x3( A, R, Q, Qx, Qy, Qz );

	BOOST_CHECK( areClose( Q, rotation, 10e-10, 10e-10 ) );
	BOOST_CHECK( areClose( R, C, 10e-10, 10e-10 ) );
	BOOST_CHECK( areClose( A, Q*R, 10e-10, 10e-10 ) );

	// Decompose into euler angles and check the values against
	// the angles initially used to create the rotation matrix.	
	Eigen::Vector3d angles = Q.eulerAngles( 2, 1, 0 ); // Extract the angles in zyx order into a vector, ordered respectively.

	Eigen::Matrix3d newRotation;
	newRotation = Eigen::AngleAxisd( angles[0], Eigen::Vector3d::UnitZ() )
		* Eigen::AngleAxisd( angles[1], Eigen::Vector3d::UnitY() )
		* Eigen::AngleAxisd( angles[2], Eigen::Vector3d::UnitX() );

	BOOST_CHECK( areClose( Q, newRotation, 10e-10, 10e-10 ) );
}

namespace GanderTest
{
	struct DecomposeRQ3x3Test
	{
		void testDecomposeRQ3x3()
		{
			try
			{
				srand(1);
				
				decomposeTest(
					( Eigen::Vector3d() << degreesToRadians( -90. ), degreesToRadians( 24.5916 ), degreesToRadians( -90. ) ).finished(),
					( Eigen::Matrix3d() << 1.94444, 0, 0, 0, 2.1875, 0, 0.0141111, 0.127, 1. ).finished()
				);
				
				for( unsigned int i = 0; i < 100; ++i )
				{
					decomposeTest(
						( Eigen::Vector3d() << randomAngle( -M_PI*.5, M_PI*.5 ), randomAngle( -M_PI*.5, M_PI*.5 ), randomAngle( -M_PI*.5, M_PI*.5 ) ).finished(),
						( Eigen::Matrix3d() << 1.94444, 0, 0, 0, 2.1875, 0, 0.0141111, 0.127, 1. ).finished()
					);
				}
			}
			catch ( std::exception &e ) 
			{
				BOOST_WARN( !e.what() );
				BOOST_CHECK( !"Exception thrown during DecomposeRQ3x3Test." );
			}
		}
	};

	struct DecomposeRQ3x3TestSuite : public boost::unit_test::test_suite
	{
		DecomposeRQ3x3TestSuite() : boost::unit_test::test_suite( "DecomposeRQ3x3TestSuite" )
		{
			boost::shared_ptr<DecomposeRQ3x3Test> instance( new DecomposeRQ3x3Test() );
			add( BOOST_CLASS_TEST_CASE( &DecomposeRQ3x3Test::testDecomposeRQ3x3, instance ) );
		}
	};

	void addDecomposeRQ3x3Test( boost::unit_test::test_suite *test )
	{
		test->add( new DecomposeRQ3x3TestSuite( ) );
	}

} // namespace GanderTest

