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

#include "Gander/Box.h"

#include "GanderTest/BoxTest.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Test;
using namespace boost;
using namespace boost::unit_test;

namespace Gander
{

namespace Test
{

struct BoxTest
{
	void testEqualityAndAssignment()
	{
		Gander::Box2d box1( Eigen::Vector2d( 1, 2 ), Eigen::Vector2d( 3, 4 ) );
		Gander::Box2d box2( Eigen::Vector2d( 5, 6 ), Eigen::Vector2d( 7, 8 ) );

		BOOST_CHECK( !box1.equalTo( box2 ) );
		BOOST_CHECK( box1 != box2 );

		box1 = box2;
		BOOST_CHECK( box1.equalTo( box2 ) );
		BOOST_CHECK( box1 == box2 );
	}

	void testEmpty()
	{
		Gander::Box2d box( Eigen::Vector2d( 1, 2 ), Eigen::Vector2d( 3, 4 ) );
		BOOST_CHECK( !box.isEmpty() );

		box.makeEmpty();
		BOOST_CHECK( box.isEmpty() );
		
		BOOST_CHECK( box.min(0) == std::numeric_limits<double>::max() );
		BOOST_CHECK( box.min(1) == std::numeric_limits<double>::max() );
		BOOST_CHECK( box.max(0) == std::numeric_limits<double>::min() );
		BOOST_CHECK( box.max(1) == std::numeric_limits<double>::min() );
	}

	void testBoxConstructors()
	{
		Gander::Box2d box;
		BOOST_CHECK( box.isEmpty() );

		Gander::Box2d box2( Gander::Box2d( Eigen::Vector2d( 1, 2 ), Eigen::Vector2d( 3, 4 ) ) );
		BOOST_CHECK( !box2.isEmpty() );
		BOOST_CHECK_EQUAL( box2.min, Eigen::Vector2d( 1, 2 ) );
		BOOST_CHECK_EQUAL( box2.max, Eigen::Vector2d( 3, 4 ) );
	}
	
	void testQuery()
	{
		Gander::Box2d box( Gander::Box2d( Eigen::Vector2d( 1, 2 ), Eigen::Vector2d( 3, 4 ) ) );

		// Size and center.
		BOOST_CHECK_EQUAL( box.size(), Eigen::Vector2d( 2, 2 ) );
		BOOST_CHECK_EQUAL( box.center(), Eigen::Vector2d( 2, 3 ) );

		// Intersection with a point.
		BOOST_CHECK( box.intersects( Eigen::Vector2d( 1.5, 3 ) ) );
		BOOST_CHECK( box.intersects( Eigen::Vector2d( 1, 3 ) ) );
		BOOST_CHECK( !box.intersects( Eigen::Vector2d( .7, 3 ) ) );

		// Intersection with a box.
		BOOST_CHECK( box.intersects( Gander::Box2d( Eigen::Vector2d( 2, 3 ), Eigen::Vector2d( 5, 7 ) ) ) );
		BOOST_CHECK( box.intersects( Gander::Box2d( Eigen::Vector2d( 3, 4 ), Eigen::Vector2d( 5, 7 ) ) ) );
		BOOST_CHECK( !box.intersects( Gander::Box2d( Eigen::Vector2d( 3.5, 4 ), Eigen::Vector2d( 5, 7 ) ) ) );
	}
	
	void testManipulators()
	{
		Gander::Box2d box( Gander::Box2d( Eigen::Vector2d( 1, 2 ), Eigen::Vector2d( 3, 4 ) ) );

		box.extendBy( Eigen::Vector2d( 0, 1 ) );
		BOOST_CHECK( box == Gander::Box2d( Eigen::Vector2d( 0, 1 ), Eigen::Vector2d( 3, 4 ) ) );
		
		box.extendBy( Gander::Box2d( Eigen::Vector2d( 7, 10 ), Eigen::Vector2d( 8.5, 11 ) ) );
		BOOST_CHECK( box == Gander::Box2d( Eigen::Vector2d( 0, 1 ), Eigen::Vector2d( 8.5, 11 ) ) );

		box.makeInfinite();
		BOOST_CHECK_EQUAL( box.min, Eigen::Vector2d::Constant( std::numeric_limits< double >::min() ) );
		BOOST_CHECK_EQUAL( box.max, Eigen::Vector2d::Constant( std::numeric_limits< double >::max() ) );

		BOOST_CHECK( box.isInfinite() );
	}

};

struct BoxTestSuite : public boost::unit_test::test_suite
{
	BoxTestSuite() : boost::unit_test::test_suite( "BoxTestSuite" )
	{
		boost::shared_ptr<BoxTest> instance( new BoxTest() );
		add( BOOST_CLASS_TEST_CASE( &BoxTest::testBoxConstructors, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BoxTest::testEmpty, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BoxTest::testQuery, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BoxTest::testManipulators, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BoxTest::testEqualityAndAssignment, instance ) );
	}
};

void addBoxTest( boost::unit_test::test_suite *test )
{
	test->add( new BoxTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

