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

#include "Gander/Tuple.h"
#include "GanderTest/TupleTest.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Test;
using namespace boost;
using namespace boost::unit_test;

template< class T, unsigned N >
void staticTupleTest()
{
	T values[N];
	memset( values, 0, sizeof( T ) * N ); // Clear the array to 0.
	
	typedef Tuple<T, N, false> TestTuple;
	TestTuple tuple;
	tuple.clear(); // Clear the tuple to 0.

	// Test random access.
	for( unsigned int i = 0; i < 100; ++i )
	{
		int index = rand() % N;
		T value = ( rand() % 100 ) / 7;
		values[index] = value;
		tuple[index] = value;
	}

	for( unsigned int i = 0; i < N; ++i )
	{
		BOOST_CHECK( values[i] == tuple[i] );
	}

	// Test the size() method.
	BOOST_CHECK_EQUAL( N, tuple.size() );

	// Test the iterator lengths.
	BOOST_CHECK_EQUAL( N, int( tuple.end() - tuple.begin() ) );

	// Test the iterators.
	for( typename TestTuple::iterator it = tuple.begin(); it != tuple.end(); it++ )
	{
		BOOST_CHECK_EQUAL( (*it), values[ it-tuple.begin() ] );
	}
};

namespace Gander
{

namespace Test
{

struct TupleTest
{
	void testStaticTuple()
	{
		srand(1);

		// Test a range of Tuple sizes and types.
		staticTupleTest< float, 15 >();
		staticTupleTest< double, 30 >();
		staticTupleTest< int, 17 >();
		staticTupleTest< short, 12 >();
	}
};

struct TupleTestSuite : public boost::unit_test::test_suite
{
	TupleTestSuite() : boost::unit_test::test_suite( "TupleTestSuite" )
	{
		boost::shared_ptr<TupleTest> instance( new TupleTest() );
		add( BOOST_CLASS_TEST_CASE( &TupleTest::testStaticTuple, instance ) );
	}
};

void addTupleTest( boost::unit_test::test_suite *test )
{
	test->add( new TupleTestSuite() );
}

} // namespace Test

} // namespace Gander

