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

#include "Gander/Common.h"

#include "GanderTest/CommonTest.h"

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

struct CommonTest
{
	void testDataTypeSizes()
	{
		BOOST_CHECK_EQUAL( int( 1 ), int( sizeof( int8 ) ) );
		BOOST_CHECK_EQUAL( int( 1 ), int( sizeof( int8u ) ) );
		BOOST_CHECK_EQUAL( int( 2 ), int( sizeof( int16 ) ) );
		BOOST_CHECK_EQUAL( int( 2 ), int( sizeof( int16u ) ) );
		BOOST_CHECK_EQUAL( int( 4 ), int( sizeof( int32 ) ) );
		BOOST_CHECK_EQUAL( int( 4 ), int( sizeof( int32u ) ) );
		BOOST_CHECK_EQUAL( int( 8 ), int( sizeof( int64 ) ) );
		BOOST_CHECK_EQUAL( int( 8 ), int( sizeof( int64u ) ) );
	}
};

struct CommonTestSuite : public boost::unit_test::test_suite
{
	CommonTestSuite() : boost::unit_test::test_suite( "CommonTestSuite" )
	{
		boost::shared_ptr<CommonTest> instance( new CommonTest() );
		add( BOOST_CLASS_TEST_CASE( &CommonTest::testDataTypeSizes, instance ) );
	}
};

void addCommonTest( boost::unit_test::test_suite *test )
{
	test->add( new CommonTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

