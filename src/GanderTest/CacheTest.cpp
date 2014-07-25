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
#include "GanderTest/CacheTest.h"

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

struct CacheTest
{
	void testMemoryInterfaceTypeIds()
	{
		// Check that the IDs were registered correctly.
		const std::vector<int> &typeIds = MemoryInterface::memoryInterfaceTypeIds();
		BOOST_CHECK( std::find( typeIds.begin(), typeIds.end(), Detail::TestMemoryInterfaceTypeId1 ) != typeIds.end() );
		BOOST_CHECK( std::find( typeIds.begin(), typeIds.end(), Detail::TestMemoryInterfaceTypeId2 ) != typeIds.end() );
		BOOST_CHECK( std::find( typeIds.begin(), typeIds.end(), Detail::TestMemoryInterfaceTypeId3 ) != typeIds.end() );
	}

	void testMemoryInterfaceNames()
	{
		// Check that the names were registered correctly.
		BOOST_CHECK_EQUAL( MemoryInterface::name( Detail::TestMemoryInterfaceTypeId1 ), "Test1" );
		BOOST_CHECK_EQUAL( MemoryInterface::name( Detail::TestMemoryInterfaceTypeId2 ), "Test2" );
		BOOST_CHECK_EQUAL( MemoryInterface::name( Detail::TestMemoryInterfaceTypeId3 ), "Test3" );
	}

	void testMemoryInterfaceCreators()
	{
		MemoryInterface *testInterface1 = MemoryInterface::create( Detail::TestMemoryInterfaceTypeId1, 3 );
		MemoryInterface *testInterface2 = MemoryInterface::create( Detail::TestMemoryInterfaceTypeId2 );
		MemoryInterface *testInterface3 = MemoryInterface::create( Detail::TestMemoryInterfaceTypeId3, 2 );

		BOOST_CHECK_EQUAL( testInterface1->deviceTypeId(), Detail::TestMemoryInterfaceTypeId1 );
		BOOST_CHECK_EQUAL( testInterface2->deviceTypeId(), Detail::TestMemoryInterfaceTypeId2 );
		BOOST_CHECK_EQUAL( testInterface3->deviceTypeId(), Detail::TestMemoryInterfaceTypeId3 );
		
		BOOST_CHECK_EQUAL( testInterface1->instanceId(), 3 );
		BOOST_CHECK_EQUAL( testInterface2->instanceId(), 0 );
		BOOST_CHECK_EQUAL( testInterface3->instanceId(), 2 );
		
		BOOST_CHECK_EQUAL( testInterface1->name(), "Test1" );
		BOOST_CHECK_EQUAL( testInterface2->name(), "Test2" );
		BOOST_CHECK_EQUAL( testInterface3->name(), "Test3" );
	}
};

struct CacheTestSuite : public boost::unit_test::test_suite
{
	CacheTestSuite() : boost::unit_test::test_suite( "CacheTestSuite" )
	{
		boost::shared_ptr<CacheTest> instance( new CacheTest() );
		add( BOOST_CLASS_TEST_CASE( &CacheTest::testMemoryInterfaceCreators, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CacheTest::testMemoryInterfaceTypeIds, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CacheTest::testMemoryInterfaceNames, instance ) );
	}
};

void addCacheTest( boost::unit_test::test_suite *test )
{
	test->add( new CacheTestSuite( ) );
}

namespace Detail
{

// Register the test device memory interfaces.
TestMemoryInterface1::MemoryInterfaceRegistration< TestMemoryInterface1 > TestMemoryInterface1::m_registration( TestMemoryInterfaceTypeId1, "Test1" );
TestMemoryInterface2::MemoryInterfaceRegistration< TestMemoryInterface2 > TestMemoryInterface2::m_registration( TestMemoryInterfaceTypeId2, "Test2" );
TestMemoryInterface3::MemoryInterfaceRegistration< TestMemoryInterface3 > TestMemoryInterface3::m_registration( TestMemoryInterfaceTypeId3, "Test3" );

} // namespace Detail

} // namespace Test

} // namespace Gander

