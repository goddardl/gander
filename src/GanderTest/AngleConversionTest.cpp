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

#include "Gander/AngleConversion.h"
#include "GanderTest/AngleConversionTest.h"

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

struct AngleConversionTest
{
	void testDegreesToRadians()
	{
		BOOST_CHECK_EQUAL( degreesToRadians( 180. ), M_PI );
		BOOST_CHECK_EQUAL( radiansToDegrees( M_PI ), 180. );
		BOOST_CHECK_EQUAL( degreesToRadians( -180. ), -M_PI );
		BOOST_CHECK_EQUAL( radiansToDegrees( -M_PI ), -180. );
		BOOST_CHECK_EQUAL( degreesToRadians( 45. ), M_PI/4. );
		BOOST_CHECK_EQUAL( radiansToDegrees( M_PI/4. ), 45. );
		BOOST_CHECK_EQUAL( degreesToRadians( 360. ), M_PI*2. );
		BOOST_CHECK_EQUAL( radiansToDegrees( M_PI*2. ), 360. );
		BOOST_CHECK_EQUAL( degreesToRadians( 720. ), M_PI*4. );
	}
};

struct AngleConversionTestSuite : public boost::unit_test::test_suite
{
	AngleConversionTestSuite() : boost::unit_test::test_suite( "AngleConversionTestSuite" )
	{
		boost::shared_ptr<AngleConversionTest> instance( new AngleConversionTest() );
		add( BOOST_CLASS_TEST_CASE( &AngleConversionTest::testDegreesToRadians, instance ) );
	}
};

void addAngleConversionTest( boost::unit_test::test_suite *test )
{
	test->add( new AngleConversionTestSuite( ) );
}

} // namespace Gander

} // namespace Test

