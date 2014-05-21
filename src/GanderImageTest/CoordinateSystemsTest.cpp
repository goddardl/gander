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

#include "GanderTest/TestTools.h"

#include "GanderImage/CoordinateSystems.h"

#include "GanderImageTest/CoordinateSystemsTest.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Image;
using namespace Gander::ImageTest;
using namespace boost;
using namespace boost::unit_test;

namespace Gander
{

namespace ImageTest
{

struct CoordinateSystemsTest
{
	void testCoordinateSystems()
	{
		/// Tests the coordinate system transforms.
		Gander::Box2i box( Gander::Box2i( Eigen::Vector2i( -100, -200 ), Eigen::Vector2i( 500, 300 ) ) );

		BOOST_CHECK_EQUAL( yDownToDisplaySpace( box, Eigen::Vector2i( -100, -200 ) ), Eigen::Vector2i( -100, 300 ) );
		BOOST_CHECK_EQUAL( yDownToDisplaySpace( box, Eigen::Vector2i( -100, 300 ) ), Eigen::Vector2i( -100, -200 ) );

		BOOST_CHECK_EQUAL( displayToYDownSpace( box, Eigen::Vector2i( -100, -200 ) ), Eigen::Vector2i( -100, 300 ) );
		BOOST_CHECK_EQUAL( displayToYDownSpace( box, Eigen::Vector2i( -100, 300 ) ), Eigen::Vector2i( -100, -200 ) );

		for( unsigned int i = 0; i < 1000; ++i )
		{
			Eigen::Vector2i p( int( Gander::Test::randomNumber( -500, 500 ) ), int( Gander::Test::randomNumber( -500, 500 ) ) );
			Eigen::Vector2i yDown = displayToYDownSpace( box, p );
			BOOST_CHECK_EQUAL( yDownToDisplaySpace( box, yDown ), p );
		}
	}
};

struct CoordinateSystemsTestSuite : public boost::unit_test::test_suite
{
	CoordinateSystemsTestSuite() : boost::unit_test::test_suite( "CoordinateSystemsTestSuite" )
	{
		boost::shared_ptr<CoordinateSystemsTest> instance( new CoordinateSystemsTest() );
		add( BOOST_CLASS_TEST_CASE( &CoordinateSystemsTest::testCoordinateSystems, instance ) );
	}
};

void addCoordinateSystemsTest( boost::unit_test::test_suite *test )
{
	test->add( new CoordinateSystemsTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

