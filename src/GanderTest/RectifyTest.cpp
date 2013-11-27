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

#include "Gander/Math.h"

#include "GanderTest/RectifyTest.h"

#include "Eigen/Geometry"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace GanderTest;
using namespace boost;
using namespace boost::unit_test;

Eigen::Vector3d opticalCenter( const Eigen::MatrixXd &pose )
{
	Eigen::MatrixXd d = pose.block( 0, 0, 3, 3 );
	return - d.inverse() * pose.col(3);
}

namespace GanderTest
{

struct RectifyTest
{
	void testRectify()
	{
		Eigen::MatrixXd pose1( 3, 4 );
		pose1 << 9.765e2, 5.382e1, -2.398e2, 3.875e5,
		9.849e1, 9.333e2, 1.574e2, 2.428e5,
		5.790e-1, 1.108e-1, 8.077e-1, 1.118e3;

		Eigen::MatrixXd pose2( 3, 4 );
		pose2 << 9.767e2, 5.376e1, -2.400e2, 4.003e4,
		9.868e1, 9.310e2, 1.567e2, 2.517e5,
		5.766e-1, 1.141e-1, 8.089e-1, 1.174e3;

		// Compute the optical centers.
		Eigen::Vector3d c1, c2;
		c1 = opticalCenter( pose1 );
		c2 = opticalCenter( pose2 );

		// Create a new X axis which is the same as the direction
		// of the baseline.
		Eigen::Vector3d v1 = c1 - c2;
		// New Y axis which is orthogonal to the new x and old z.
		Eigen::Vector3d v2 = R1.
		// New Z axis which is orthogonal to the baseline and Y.
		Eigen::Vector3d v3 = v1.cross( v2 );
	}
};

struct RectifyTestSuite : public boost::unit_test::test_suite
{
	RectifyTestSuite() : boost::unit_test::test_suite( "RectifyTestSuite" )
	{
		boost::shared_ptr<RectifyTest> instance( new RectifyTest() );
		add( BOOST_CLASS_TEST_CASE( &RectifyTest::testRectify, instance ) );
	}
};

void addRectifyTest( boost::unit_test::test_suite *test )
{
	test->add( new RectifyTestSuite( ) );
}

} // namespace GanderTest

