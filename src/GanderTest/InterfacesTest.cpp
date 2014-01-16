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

#include "GanderTest/InterfacesTest.h"

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

struct InterfacesTest
{
	void testEquality()
	{
		/// Test that the equality operators work as expected.
		InterfaceTest<int> i1;
		i1.data = 1;
		InterfaceTest<int> i2;
		i2.data = 2;

		BOOST_CHECK_EQUAL( i1 == i2, false );
		BOOST_CHECK_EQUAL( i1 != i2, true );

		i1.data = 2;
		
		BOOST_CHECK_EQUAL( i1 == i2, true );
		BOOST_CHECK_EQUAL( i1 != i2, false );
	};
	
	void testIncrementAndDecrement()
	{
		/// Test that the integer arithmetic operators work as expected.
		InterfaceTest<int> i;
		InterfaceTest<int> i1;
		InterfaceTest<int> i2;
		i.data = 1;
		i1.data = 1;
		i2.data = 2;

		// Increment and Decrment operators.
		BOOST_CHECK_EQUAL( i == i1, true );
		BOOST_CHECK_EQUAL( (i+1) == i2, true );
		BOOST_CHECK_EQUAL( i++ == i1, true );
		BOOST_CHECK_EQUAL( i == i2, true );
		BOOST_CHECK_EQUAL( --i == i1, true );
		BOOST_CHECK_EQUAL( ++i == i2, true );
		BOOST_CHECK_EQUAL( i-- == i2, true );
		BOOST_CHECK_EQUAL( i == i1, true );
	};
	
	void testIntegerArithmetic()
	{
		/// Test that the integer arithmetic operators work as expected.
		InterfaceTest<int> i;
		InterfaceTest<int> i1;
		InterfaceTest<int> i2;
		i.data = 1;
		i1.data = 1;
		i2.data = 2;

		// Addition and subtraction.
		BOOST_CHECK_EQUAL( ( i2 - 1 ) == i1, true );
		BOOST_CHECK_EQUAL( ( i1 + 1 ) == i2, true );

		// Assignment operators.
		BOOST_CHECK_EQUAL( ( i += 1 ) == i2, true );
		BOOST_CHECK_EQUAL( ( i -= 1 ) == i1, true );

		// Assignment
		i = i1 + 1;
		BOOST_CHECK_EQUAL( i == i2, true );
	};
};

struct InterfacesTestSuite : public boost::unit_test::test_suite
{
	InterfacesTestSuite() : boost::unit_test::test_suite( "InterfacesTestSuite" )
	{
		boost::shared_ptr<InterfacesTest> instance( new InterfacesTest() );
		add( BOOST_CLASS_TEST_CASE( &InterfacesTest::testEquality, instance ) );
		add( BOOST_CLASS_TEST_CASE( &InterfacesTest::testIntegerArithmetic, instance ) );
		add( BOOST_CLASS_TEST_CASE( &InterfacesTest::testIncrementAndDecrement, instance ) );
	}
};

void addInterfacesTest( boost::unit_test::test_suite *test )
{
	test->add( new InterfacesTestSuite() );
}

} // namespace Test

} // namespace Gander

