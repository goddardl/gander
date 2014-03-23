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

#include "Gander/BitTwiddler.h"
#include "GanderTest/BitTwiddlerTest.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Test;
using namespace boost;
using namespace boost::unit_test;

// This function creates a variable of type T and manipulates it with a BitTwiddler class.
// We write a load of random bits to the variable and then read them back and compare the
// result to the expected result.
template< class T, unsigned WIDTH >
void testBitTwiddler()
{
	unsigned int size = ( sizeof( T ) * 8 ) / WIDTH;
	int8u expectedResults[size];
	memset( &expectedResults[0], 0, sizeof( int8u ) * size );

	T testData = 0;
	BitTwiddler<T, WIDTH> t( testData );
		
	BOOST_CHECK_EQUAL( int( size ), int( t.size() ) );

	/// Run the test 10 times.
	for( unsigned int k = 0; k < 10; ++k )
	{
		/// Randomly write data using the bit twiddler and then read it back.
		for( unsigned int j = 0; j < 200; ++j )
		{
			int8u index = rand() % t.size();
			int8u value = rand() % ( ~( ( ~T(0) ) << WIDTH ) + 1 );
			t[index] = value;
			expectedResults[index] = value;
		}

		for( unsigned int j = 0; j < size; ++j )
		{
			BOOST_CHECK_EQUAL( expectedResults[j], t[j] );
		}
	}
}

// This function creates a variable of type T and manipulates it with a BitTwiddler class.
// We write a randomly inserts and erases sets of WIDTH bits and then reads them back and compares the
// result to the expected result.
template< class T, unsigned WIDTH >
void testBitTwiddlerInsertEraseElement()
{
	unsigned int size = ( sizeof( T ) * 8 ) / WIDTH;
	std::vector<int8u> expectedResults( size, 0 );

	T testData = 0;
	BitTwiddler<T, WIDTH> t( testData );
		
	BOOST_CHECK_EQUAL( int( size ), int( t.size() ) );

	/// Randomly insert and erase data using the bit twiddler before reading it back.
	for( unsigned int j = 0; j < 128; ++j )
	{
		int8u index = rand() % ( t.size() - 1 );
		int8u value = rand() % ( ~( ( ~T(0) ) << WIDTH ) + 1 );
		t.insert( index, value );
		expectedResults.insert( expectedResults.begin() + index, value );
		
		index = rand() % ( t.size() - 1 );
		t.erase( index );
		expectedResults.erase( expectedResults.begin() + index );
	}

	for( unsigned int j = 0; j < size; ++j )
	{
		BOOST_CHECK_EQUAL( expectedResults[j], t[j] );
	}
}

template< class T >
void testBitTwiddlerInsertEraseWithType()
{
	testBitTwiddlerInsertEraseElement<T, 1>();
	testBitTwiddlerInsertEraseElement<T, 2>();
	testBitTwiddlerInsertEraseElement<T, 3>();
	testBitTwiddlerInsertEraseElement<T, 4>();
	testBitTwiddlerInsertEraseElement<T, 5>();
	testBitTwiddlerInsertEraseElement<T, 6>();
	testBitTwiddlerInsertEraseElement<T, 7>();
}

template< class T >
void testBitTwiddlerWithType()
{
	// Test the read and write functionality of the
	// bitTwiddler class on a variable of type T using
	// elements of variable widths.
	testBitTwiddler<T, 1>();
	testBitTwiddler<T, 2>();
	testBitTwiddler<T, 3>();
	testBitTwiddler<T, 4>();
	testBitTwiddler<T, 5>();
	testBitTwiddler<T, 6>();
	testBitTwiddler<T, 7>();
	testBitTwiddler<T, 8>();
}

namespace Gander
{

namespace Test
{

struct BitTwiddlerTest
{
	void testBitTwiddlerReadWrite()
	{
		srand(1);

		// Test the read and write functionality of the
		// bitTwiddler classes using a variety of datatypes
		// and element widths.
		testBitTwiddlerWithType<int64>();
		testBitTwiddlerWithType<int64u>();
		testBitTwiddlerWithType<int32>();
		testBitTwiddlerWithType<int32u>();
		testBitTwiddlerWithType<int16>();
		testBitTwiddlerWithType<int16u>();
	}
	
	void testBitTwiddlerInsert()
	{
		srand(1);

		// Test the insertion of bits into the a variety
		// of different datatypes and element widths.
		testBitTwiddlerInsertEraseWithType< int64 >();
		testBitTwiddlerInsertEraseWithType< int64u >();
		testBitTwiddlerInsertEraseWithType< int32 >();
		testBitTwiddlerInsertEraseWithType< int32u >();
		testBitTwiddlerInsertEraseWithType< int16 >();
		testBitTwiddlerInsertEraseWithType< int16u >();
	}
};

struct BitTwiddlerTestSuite : public boost::unit_test::test_suite
{
	BitTwiddlerTestSuite() : boost::unit_test::test_suite( "BitTwiddlerTestSuite" )
	{
		boost::shared_ptr<BitTwiddlerTest> instance( new BitTwiddlerTest() );
		add( BOOST_CLASS_TEST_CASE( &BitTwiddlerTest::testBitTwiddlerReadWrite, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BitTwiddlerTest::testBitTwiddlerInsert, instance ) );
	}
};

void addBitTwiddlerTest( boost::unit_test::test_suite *test )
{
	test->add( new BitTwiddlerTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

