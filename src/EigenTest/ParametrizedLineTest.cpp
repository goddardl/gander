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

#include "Gander/Math.h"
#include "Gander/AreClose.h"
#include "Gander/AngleConversion.h"

#include "GanderTest/TestTools.h"

#include "EigenTest/ParametrizedLineTest.h"

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

struct ParametrizedLineTest
{

	/// Returns a ParametrizedLine of the format y = a*x+b.
	Eigen::ParametrizedLine< double, 2 > line( double a, double b ) const
	{
		return Eigen::ParametrizedLine< double, 2 >::Through( Eigen::Vector2d( 1., a * 1. + b ), Eigen::Vector2d( 2., a * 2. + b ) );
	}
	
	/// A simple test to demonstrate how to retrieve the closest point on a line to a another point.
	void testClosestPointOnLine()
	{
		Eigen::ParametrizedLine< double, 2 > line1;

		line1 = line( 1, 2 );
		
		Eigen::Vector2d closestPoint;
		closestPoint = line1.projection( Eigen::Vector2d( 0, 2.5 ) );
		BOOST_CHECK( areClose( closestPoint, Eigen::Vector2d( .25, 2.25 ), 10e-6, 10e-6 ) );
		
		closestPoint = line1.projection( Eigen::Vector2d( 1.5, 2.5 ) );
		BOOST_CHECK( areClose( closestPoint, Eigen::Vector2d( 1, 3 ), 10e-6, 10e-6 ) );
	}
	
	/// A simple test to demonstrate the use of the Eigen::ParametrizedLine class to find the point of intersection between two lines.
	void testParametrizedLineIntersection()
	{
		Eigen::ParametrizedLine< double, 2 > line1;
		Eigen::ParametrizedLine< double, 2 > line2;

		line1 = line( 2, .5 );
		line2 = line( 1.5, 1 );

		Eigen::Vector2d point = line1.pointAt( line1.intersection( Eigen::Hyperplane< double, 2 >( line2 ) ) );
		BOOST_CHECK( areClose( point, Eigen::Vector2d( 1., 2.5 ), 10e-6, 10e-6 ) );
		
		line1 = line( 1.75, -.5 );
		line2 = line( 1.25, 1.1 );

		point = line1.pointAt( line1.intersection( Eigen::Hyperplane< double, 2 >( line2 ) ) );
		BOOST_CHECK( areClose( point, Eigen::Vector2d( 3.2, 5.1 ), 10e-6, 10e-6 ) );
	}
};

struct ParametrizedLineTestSuite : public boost::unit_test::test_suite
{
	ParametrizedLineTestSuite() : boost::unit_test::test_suite( "ParametrizedLineTestSuite" )
	{
		boost::shared_ptr<ParametrizedLineTest> instance( new ParametrizedLineTest() );
		add( BOOST_CLASS_TEST_CASE( &ParametrizedLineTest::testParametrizedLineIntersection, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ParametrizedLineTest::testClosestPointOnLine, instance ) );
	}
};

void addParametrizedLineTest( boost::unit_test::test_suite *test )
{
	test->add( new ParametrizedLineTestSuite( ) );
}

} // namespace EigenTest

