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

#include "Gander/MurmurHash.h"
#include "GanderTest/MurmurHashTest.h"

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

struct MurmurHashTest
{
	void testConstructor()
	{
		MurmurHash h;
		BOOST_CHECK_EQUAL( std::string( h ), "00000000000000000000000000000000" );
		BOOST_CHECK( h == MurmurHash() );
	}
	
	void testCopyConstructor()
	{
		MurmurHash h;
		h.append( 1 );
		h.append( "hello" );
		BOOST_CHECK( h == MurmurHash( h ) );
		BOOST_CHECK( h != MurmurHash() );
	}

	void testAppend()
	{
		MurmurHash h, h2;
		h.append( "hello" );
		BOOST_CHECK( h != h2 );
		
		h2 = h;
		h.append( 2 );
		BOOST_CHECK( h != h2 );
		
		h2 = h;
		h.append( 2. );
		BOOST_CHECK( h != h2 );
	}
	
	void testKnownHashes()
	{
		// test against hashes generated using the smhasher code directly
		MurmurHash h;
		h.append( "the quick brown fox jumps over the lazy dog" );
		BOOST_CHECK_EQUAL( h.toString(), "bce4e9fee2ad86b30ae2e374406e4b7f" );

		h = MurmurHash();
		h.append( 101 );
		BOOST_CHECK_EQUAL( h.toString(), "1739807a7ecb8d70bbf1c02a2a649b8f" );
	}

	void testStringRepeatability()
	{
		MurmurHash h;
		h.append( "i am a lovely string" );
		
		for( unsigned int i = 0; i < 1000; ++i )
		{
			MurmurHash h2;
			h2.append( "i am a lovely string" );
			BOOST_CHECK( h2 == h );
		}
	}

};

struct MurmurHashTestSuite : public boost::unit_test::test_suite
{
	MurmurHashTestSuite() : boost::unit_test::test_suite( "MurmurHashTestSuite" )
	{
		boost::shared_ptr<MurmurHashTest> instance( new MurmurHashTest() );
		add( BOOST_CLASS_TEST_CASE( &MurmurHashTest::testConstructor, instance ) );
		add( BOOST_CLASS_TEST_CASE( &MurmurHashTest::testCopyConstructor, instance ) );
		add( BOOST_CLASS_TEST_CASE( &MurmurHashTest::testKnownHashes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &MurmurHashTest::testStringRepeatability, instance ) );
		add( BOOST_CLASS_TEST_CASE( &MurmurHashTest::testAppend, instance ) );
	}
};

void addMurmurHashTest( boost::unit_test::test_suite *test )
{
	test->add( new MurmurHashTestSuite( ) );
}

} // namespace Gander

} // namespace Test

