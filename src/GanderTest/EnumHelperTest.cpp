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

#include "Gander/EnumHelper.h"
#include "Gander/EnumHelper.h"

#include "GanderTest/EnumHelperTest.h"

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

struct EnumHelperTest
{
	void testEnumArrayHelperStaticElementAccessor()
	{
		// Test the access of an element from an array that is 2 bits wide.
		{
		typedef EnumArrayHelper<0x53C, 2> Array;
		BOOST_CHECK_EQUAL( int( Array::element(0) ), int( 0 ) );
		BOOST_CHECK_EQUAL( int( Array::element(1) ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Array::element(2) ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Array::element(3) ), int( 0 ) );
		BOOST_CHECK_EQUAL( int( Array::element(4) ), int( 1 ) );
		BOOST_CHECK_EQUAL( int( Array::element(5) ), int( 1 ) );
		}
		// Test the access of an element from an array that is 2 bits wide.
		{
		typedef EnumArrayHelper<0x53C, 3> Array;
		BOOST_CHECK_EQUAL( int( Array::element(0) ), int( 4 ) );
		BOOST_CHECK_EQUAL( int( Array::element(1) ), int( 7 ) );
		BOOST_CHECK_EQUAL( int( Array::element(2) ), int( 4 ) );
		BOOST_CHECK_EQUAL( int( Array::element(3) ), int( 2 ) );
		}
	};

	void testEnumArrayHelperInsertElement()
	{
		// Test the insertion of an element into an array that is 2 bits wide.
		{
		typedef EnumArrayHelper<0x53C, 2> Array;
		BOOST_CHECK_EQUAL( int( Array::InsertElement<0, 2>::Value ), int( 5362 ) );
		BOOST_CHECK_EQUAL( int( Array::InsertElement<1, 1>::Value ), int( 5364 ) );
		BOOST_CHECK_EQUAL( int( Array::InsertElement<2, 0>::Value ), int( 5324 ) );
		BOOST_CHECK_EQUAL( int( Array::InsertElement<3, 3>::Value ), int( 5372 ) );
		}
		// Test the insertion of an element into an array that is 3 bits wide.
		{
		typedef EnumArrayHelper<0x53C, 3> Array;
		BOOST_CHECK_EQUAL( int( Array::InsertElement<0, 2>::Value ), int( 10722 ) );
		BOOST_CHECK_EQUAL( int( Array::InsertElement<1, 6>::Value ), int( 10740 ) );
		BOOST_CHECK_EQUAL( int( Array::InsertElement<2, 7>::Value ), int( 10748 ) );
		BOOST_CHECK_EQUAL( int( Array::InsertElement<3, 3>::Value ), int( 10044 ) );
		}
	}

	void testEnumArrayHelperElement()
	{
		// Test the access of elements from an array that is 2 bits wide.
		{
		typedef EnumArrayHelper<0x34D, 2> Array;
		BOOST_CHECK_EQUAL( int( Array::Element<0>::Value ), int( 1 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<1>::Value ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<2>::Value ), int( 0 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<3>::Value ), int( 1 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<4>::Value ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<5>::Value ), int( 0 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<6>::Value ), int( 0 ) );
		}
		// Test the access of elements from an array that is 3 bits wide.
		{
		typedef EnumArrayHelper<0x34D, 3> Array;
		BOOST_CHECK_EQUAL( int( Array::Element<0>::Value ), int( 5 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<1>::Value ), int( 1 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<2>::Value ), int( 5 ) );
		BOOST_CHECK_EQUAL( int( Array::Element<3>::Value ), int( 1 ) );
		}
	}
	/// Test the EnumArrayHelper::RemoveElement enum values.
	/// This helpful little struct will allow you to take an enum which we interpret as an
	/// array of X bits and remove a single element by supplying it's index.
	/// For example, we can interpret the value of 0x34D as an array of sets of 2 bits using
	/// the EnumArrayHelper struct:
	/// 
	/// typedef EnumArrayHelper<0x34D, 2> Array
	/// 
	/// This interprets 0x34D as an array of 2 bit values which would look like:
	/// Index:   4   3   2   1   0
	/// Value:{ 11, 01, 00, 11, 01 } = 0x34D
	/// 
	/// If we wanted to remove element 3, we simply use the RemoveElement struct :
	/// Array::RemoveElement<3>::Value
	/// 
	/// The Value enum is:
	/// Index:   3   2   1   0
	/// Value:{ 11, 00, 11, 01 } = 0x205
	///
	void testEnumArrayHelperRemoveElement()
	{
		// Test the removal of elements from an array that is 2 bits wide.
		{
		typedef EnumArrayHelper<0x34D, 2> Array;
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<0>::Value ), int( 211 ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<1>::Value ), int( 209 ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<2>::Value ), int( 221 ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<3>::Value ), int( 205 ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<4>::Value ), int( 77 ) );
		}
		
		// Test the removal of elements that are near the end or over the end of the array datatype.
		{
		typedef EnumArrayHelper<0xF300000000000000ull, 4> Array;
		BOOST_CHECK_EQUAL( EnumType( Array::RemoveElement<14>::Value ), EnumType( 0xF00000000000000ull ) );
		BOOST_CHECK_EQUAL( EnumType( Array::RemoveElement<15>::Value ), EnumType( 0x300000000000000ull ) );
		BOOST_CHECK_EQUAL( EnumType( Array::RemoveElement<16>::Value ), EnumType( 0xF300000000000000ull ) );
		}

		// Test the removal of elements from an array that is 1 bit wide.
		{
		typedef EnumArrayHelper<0x25A, 1> Array;
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<0>::Value ), int( 0x12D ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<1>::Value ), int( 0x12C ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<2>::Value ), int( 0x12E ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<3>::Value ), int( 0x12A ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<4>::Value ), int( 0x12A ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<5>::Value ), int( 0x13A ) );
		BOOST_CHECK_EQUAL( int( Array::RemoveElement<6>::Value ), int( 0x11A ) );
		}
	}

	/// Test the EnumArrayHelper::RemoveElements enum values.
	/// This helpful little struct will allow you to take an enum which we interpret as an
	/// array of X bits and remove several elements by supplying a mask of the elements 
	/// to keep.
	/// For example, we can interpret the value of 0x34D as an array of sets of 2 bits using
	/// the EnumArrayHelper struct:
	/// 
	/// typedef EnumArrayHelper<0x34D, 2> Array
	/// 
	/// This interprets 0x34D as an array of 2 bit values which would look like:
	/// Index:   4   3   2   1   0
	/// Value:{ 11, 01, 00, 11, 01 } = 0x34D
	/// 
	/// If we wanted to remove elements 0 and 3, we could supply the mask 10110 (22) to the RemoveElements struct :
	/// Array::RemoveElements<22>::Value
	/// 
	/// The Value enum is:
	/// Index:   2   1   0
	/// Value:{ 11, 00, 11 } = 51
	///
	void testEnumArrayHelper()
	{
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x34D, 2>::RemoveElements<22>::Value ), int( 51 ) );
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x34D, 2>::RemoveElements<27>::Value ), int( 0xDD ) );
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x34D, 2>::RemoveElements<10>::Value ), int( 7 ) );
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x31E, 2>::RemoveElements<58>::Value ), int( 51 ) );
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x64B, 2>::RemoveElements<41>::Value ), int( 23 ) );
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x64B, 2>::RemoveElements<24>::Value ), int( 9 ) );
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x64B, 2>::RemoveElements<0xFFFF>::Value ), int( 0x64B ) );
		BOOST_CHECK_EQUAL( int( EnumArrayHelper<0x64B, 2>::RemoveElements<0>::Value ), int( 0 ) );
	}

	/// Test the EnumArrayHelper::PadElements enum values.
	/// This helpful little struct will allow you to take an enum which we interpret as an
	/// array of Y elements, X bits wide and insert several empty elements between them by supplying a mask
	/// that specifies the distribution of the elements.
	///
	/// For example, we can interpret the value of 0x34D as an array of sets of 2 bits using
	/// the EnumArrayHelper struct:
	/// 
	/// typedef EnumArrayHelper<0x4D, 2> Array
	/// 
	/// This interprets 0x34D as an array of 2 bit values which would look like:
	/// Index:   3   2   1   0
	/// Value:{ 01, 00, 11, 01 } = 0x4D
	/// Mask : 1111
	/// 
	/// If we wanted to redistribute the elements by inserting 2 empty elements between elements 0 and 3,
	/// we could supply the mask 110011 (51) to the PadElements struct :
	/// Array::PadElements<51>::Value
	/// 
	/// The Value enum is:
	/// Index:   5   4   3   2   1   0
	/// Value:{ 01, 00, 00, 00, 11, 01 } = 1037
	///
	void testEnumArrayHelperPadElements()
	{
		{
		typedef EnumArrayHelper<0xF, 2> Array;
		BOOST_CHECK_EQUAL( int( Array::PadElements<0xC>::Value ), int( 240 ) );
		BOOST_CHECK_EQUAL( int( Array::PadElements<0xA>::Value ), int( 204 ) );
		BOOST_CHECK_EQUAL( int( Array::PadElements<0x6>::Value ), int( 60 ) );
		BOOST_CHECK_EQUAL( int( Array::PadElements<0x9>::Value ), int( 195 ) );
		}

		{
		typedef EnumArrayHelper<0x4D, 2> Array;
		BOOST_CHECK_EQUAL( int( Array::PadElements<51>::Value ), int( 1037 ) );
		}
		
		{
		typedef EnumArrayHelper<713, 4> Array;
		BOOST_CHECK_EQUAL( int( Array::PadElements<38>::Value ), int( 2100368 ) );
		}
	}

	/// Test the EnumHelper struct can provide the smallest number of
	/// bits required to represent a particular number.
	void testEnumHelperMinimumBitWidth()
	{
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<1>::MinimumRequiredBitWidth ), int( 1 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<2>::MinimumRequiredBitWidth ), int( 2 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<3>::MinimumRequiredBitWidth ), int( 2 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<4>::MinimumRequiredBitWidth ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<5>::MinimumRequiredBitWidth ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<6>::MinimumRequiredBitWidth ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<7>::MinimumRequiredBitWidth ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<8>::MinimumRequiredBitWidth ), int( 4 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<14>::MinimumRequiredBitWidth ), int( 4 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<15>::MinimumRequiredBitWidth ), int( 4 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<271>::MinimumRequiredBitWidth ), int( 9 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<6447>::MinimumRequiredBitWidth ), int( 13 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<1055023>::MinimumRequiredBitWidth ), int( 21 ) );
	}

	/// Test the EnumHelper struct can count the number
	/// of set bits in an integer value.
	void testEnumHelperNumberOfSetBits()
	{
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<57>::NumberOfSetBits ), int( 4 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x22758EF3>::NumberOfSetBits ), int( 17 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x57492CDC>::NumberOfSetBits ), int( 16 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0xFFFFFFFF>::NumberOfSetBits ), int( 32 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x0>::NumberOfSetBits ), int( 0 ) );
	}
	
	/// Test the EnumHelper struct can return the position
	/// of the last set bit in an integer value.
	void testEnumHelperLastSetBit()
	{
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0>::LastSetBit ), int( 0 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<1>::LastSetBit ), int( 0 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<7>::LastSetBit ), int( 2 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<8>::LastSetBit ), int( 3 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x12>::LastSetBit ), int( 4 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x4ED78>::LastSetBit ), int( 18 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x4ED7845>::LastSetBit ), int( 26 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x4ED78455>::LastSetBit ), int( 30 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0xFFFFFFFF>::LastSetBit ), int( 31 ) );
	}

	/// Test the EnumHelper struct can return the position
	/// of the first set bit in an integer value.
	void testEnumHelperFirstSetBit()
	{
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0>::FirstSetBit ), int( 0 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x80000000>::FirstSetBit ), int( 31 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x10000>::FirstSetBit ), int( 16 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x22A48020>::FirstSetBit ), int( 5 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x22A48000>::FirstSetBit ), int( 15 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x22248000>::FirstSetBit ), int( 15 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x22240000>::FirstSetBit ), int( 18 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x22200000>::FirstSetBit ), int( 21 ) );
		BOOST_CHECK_EQUAL( int( Gander::EnumHelper<0x22000000>::FirstSetBit ), int( 25 ) );
	}
};

struct EnumHelperTestSuite : public boost::unit_test::test_suite
{
	EnumHelperTestSuite() : boost::unit_test::test_suite( "EnumHelperTestSuite" )
	{
		boost::shared_ptr<EnumHelperTest> instance( new EnumHelperTest() );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumHelperNumberOfSetBits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumHelperFirstSetBit, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumHelperLastSetBit, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumHelperMinimumBitWidth, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumArrayHelper, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumArrayHelperElement, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumArrayHelperRemoveElement, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumArrayHelperInsertElement, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumArrayHelperStaticElementAccessor, instance ) );
		add( BOOST_CLASS_TEST_CASE( &EnumHelperTest::testEnumArrayHelperPadElements, instance ) );
	}
};

void addEnumHelperTest( boost::unit_test::test_suite *test )
{
	test->add( new EnumHelperTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

