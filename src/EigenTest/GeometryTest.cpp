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

#include "Gander/Math.h"
#include "Gander/AreClose.h"
#include "Gander/AngleConversion.h"

#include "GanderTest/TestTools.h"

#include "EigenTest/GeometryTest.h"

#include "Eigen/Geometry"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace EigenTest;
using namespace boost;
using namespace boost::unit_test;
using namespace Eigen;
using namespace Gander::Test;
using namespace Gander;

namespace EigenTest
{

struct GeometryTest
{
	void testAngleAxis()
	{
		srand(1);
		
		for( int i = 0; i < 10; ++i )
		{
			Matrix3d rx, ry, rz;
			
			Eigen::Vector3d xyz( degreesToRadians( randomNumber( 0, 89.99 ) ), degreesToRadians( randomNumber( 0, 89.99 ) ), degreesToRadians( randomNumber( 0, 89.99 ) ) );
			rx = AngleAxisd( xyz[0], Vector3d::UnitX() );
			ry = AngleAxisd( xyz[1], Vector3d::UnitY() );
			rz = AngleAxisd( xyz[2], Vector3d::UnitZ() );
			
			Eigen::Matrix3d rotation;
			rotation = rz * ry * rx;

			// Test the extraction of Euler angles and the construction of a rotation matrix.	
			Eigen::Vector3d zyx;
			zyx = rotation.eulerAngles( 2, 1, 0 ); // Extract the angles in zyx order into a vector, ordered respectively.
			
			Eigen::Matrix3d newRotation;
			newRotation = Eigen::AngleAxisd( zyx[0], Eigen::Vector3d::UnitZ() )
				* Eigen::AngleAxisd( zyx[1], Eigen::Vector3d::UnitY() )
				* Eigen::AngleAxisd( zyx[2], Eigen::Vector3d::UnitX() );
			
			BOOST_CHECK( areClose( newRotation, rotation, 10e-10, 10e-10 ) );
			BOOST_CHECK( areClose( ( Eigen::Vector3d() << zyx[2], zyx[1], zyx[0] ).finished(), xyz, 10e-6, 10e-6 ) );
		}
	}
};

struct GeometryTestSuite : public boost::unit_test::test_suite
{
	GeometryTestSuite() : boost::unit_test::test_suite( "GeometryTestSuite" )
	{
		boost::shared_ptr<GeometryTest> instance( new GeometryTest() );
		add( BOOST_CLASS_TEST_CASE( &GeometryTest::testAngleAxis, instance ) );
	}
};

void addGeometryTest( boost::unit_test::test_suite *test )
{
	test->add( new GeometryTestSuite( ) );
}

} // namespace EigenTest

