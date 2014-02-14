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

#include "GanderImage/Channel.h"
#include "GanderImageTest/ChannelTest.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Image;
using namespace Gander::ImageTest;
using namespace boost;
using namespace boost::unit_test;

namespace Gander
{

namespace ImageTest
{

struct ChannelTest
{
	void testChannelTraits()
	{
		BOOST_CHECK_EQUAL( int( ChannelTraits::SizeOfChannelLookupEnumType ), int( sizeof( ChannelTraits::ChannelLookupEnumType ) ) );
		BOOST_CHECK_EQUAL( int( ChannelTraits::NumberOfDefaultChannels ), 11 );
		BOOST_CHECK_EQUAL( int( ChannelTraits::RepresentableBitWidth ), 4 );
		BOOST_CHECK_EQUAL( int( ChannelTraits::MaxNumberOfDefaultChannels ), 16 );
		BOOST_CHECK( int( ChannelTraits::NumberOfDefaultChannels ) <= int( ChannelTraits::MaxNumberOfDefaultChannels ) );
	};

	void testSize()
	{
		ChannelSet rgb( Mask_RGB );
		BOOST_CHECK_EQUAL( rgb.size(), 3 );
		ChannelSet g( Mask_Green );
		BOOST_CHECK_EQUAL( g.size(), 1 );
		ChannelSet none;
		BOOST_CHECK_EQUAL( none.size(), 0 );
	}

	void testClear()
	{
		ChannelSet rgb( Mask_RGB );
		BOOST_CHECK( !rgb.empty() );
		
		rgb.clear();
		BOOST_CHECK( rgb.value() == 0 );
		BOOST_CHECK( rgb.empty() );
	}
	
	void testArithmeticOperators()
	{
		ChannelSet set;
		ChannelSet blueSet( Chan_Blue );
		BOOST_CHECK( !set.contains( Chan_Blue ) );
		set += Chan_Blue;
		BOOST_CHECK( set.contains( Chan_Blue ) );
		set -= Chan_Blue;
		BOOST_CHECK( !set.contains( Chan_Blue ) );
		set += Mask_Blue;
		BOOST_CHECK( set.contains( Chan_Blue ) );
		set -= Mask_Blue;
		BOOST_CHECK( !set.contains( Chan_Blue ) );
		set += blueSet;
		BOOST_CHECK( set.contains( Chan_Blue ) );
		set -= blueSet;
		BOOST_CHECK( !set.contains( Chan_Blue ) );
		
		ChannelSet rgba( Mask_RGBA );
		rgba &= Mask_RGB;
		BOOST_CHECK( rgba == Mask_RGB );
		
		rgba &= Chan_Red;
		BOOST_CHECK( rgba == Chan_Red );

		rgba = ChannelSet( Mask_RGBA );
		rgba &= ChannelSet( Chan_Green );
		BOOST_CHECK( rgba == Chan_Green );

		ChannelSet ga( Mask_Green | Mask_Alpha );
		ChannelSet a( Mask_Alpha );
		ChannelSet g( Mask_Green );
		BOOST_CHECK_EQUAL( a + Chan_Green, ga );
		BOOST_CHECK_EQUAL( a + Mask_Green, ga );
		BOOST_CHECK_EQUAL( a + g, ga );
		BOOST_CHECK_EQUAL( ga - Chan_Green, a );
		BOOST_CHECK_EQUAL( ga - Mask_Green, a );
		BOOST_CHECK_EQUAL( ga - g, a );
	}
	
	void testCombineMasks()
	{
		BOOST_CHECK_EQUAL( ChannelSet( ChannelMask( CombineMasks< Mask_Red, Mask_U >::Value ) ), ChannelSet( Mask_Red | Mask_U ) );
		BOOST_CHECK_EQUAL( ChannelSet( ChannelMask( CombineMasks< Mask_Red, Mask_Z, Mask_Blue, Mask_Alpha, Mask_Green, Mask_U >::Value ) ), ChannelSet( Mask_RGBA | Mask_U | Mask_Z ) );
	}

	void testEquality()
	{
		ChannelSet red( Mask_Red );
		BOOST_CHECK( red != Mask_Blue );
		BOOST_CHECK( red != Chan_Blue );
		BOOST_CHECK( red != ChannelSet( Mask_Blue ) );
		BOOST_CHECK( red == Mask_Red );
		BOOST_CHECK( red == Chan_Red );
		BOOST_CHECK( red == ChannelSet( Chan_Red ) );
		BOOST_CHECK( ChannelSet( Mask_Red ) < ChannelSet( Mask_RGB ) );
		BOOST_CHECK( ChannelSet( Mask_Red ) <= ChannelSet( Mask_RGB ) );
		BOOST_CHECK( ChannelSet( Mask_RGBA ) > ChannelSet( Mask_RGB ) );
		BOOST_CHECK( ChannelSet( Mask_RGBA ) >= ChannelSet( Mask_RGB ) );
		BOOST_CHECK( ChannelSet( Mask_Alpha ) >= ChannelSet( Mask_Red ) );
		BOOST_CHECK( ChannelSet( Mask_Alpha ) <= ChannelSet( Mask_Red ) );
	}

	void testEnumOperators()
	{
		ChannelMask mask1 = Chan_Alpha | Mask_Blue;
		ChannelMask mask2 = Mask_Alpha | Chan_Blue;
		BOOST_CHECK_EQUAL( mask1, mask2 );
		
		mask1 = mask1 & Chan_Blue;
		BOOST_CHECK_EQUAL( mask1, Mask_Blue );
		
		mask2 = Chan_Blue & mask2;
		BOOST_CHECK_EQUAL( mask2, Mask_Blue );

		mask1 = Chan_Green | Chan_Alpha;
		mask2 = Mask_Green | Mask_Alpha;
		BOOST_CHECK_EQUAL( mask1, mask2 );
	}

	void testChannelSetConstructors()
	{
		ChannelSet alphaSet( Chan_Alpha );
		BOOST_CHECK( alphaSet.contains( Chan_Alpha ) );

		ChannelSet blueSet( Mask_Blue );
		BOOST_CHECK( blueSet.contains( Chan_Blue ) );

		ChannelSet copySet( alphaSet );
		BOOST_CHECK( copySet.contains( Chan_Alpha ) );

		ChannelSet rgb( Mask_RGB );
		BOOST_CHECK( rgb.contains( Chan_Red ) );
		BOOST_CHECK( rgb.contains( Chan_Green ) );
		BOOST_CHECK( rgb.contains( Chan_Blue ) );
	}
	
	void testIterator()
	{
		ChannelSet gbm( Mask_Mask | Mask_Green | Mask_Blue );
		ChannelSet::const_iterator it( gbm.begin() );
		ChannelSet::const_iterator end( gbm.end() );
		BOOST_CHECK_EQUAL( *it, Chan_Green );		
		BOOST_CHECK_EQUAL( *it++, Chan_Green );
		BOOST_CHECK_EQUAL( *it, Chan_Blue );
		BOOST_CHECK_EQUAL( *(++it), Chan_Mask );		
		BOOST_CHECK_EQUAL( *(++it), *end );

		it = gbm.end();
		end = gbm.end();
		BOOST_CHECK_EQUAL( *it--, Chan_None );		
		BOOST_CHECK_EQUAL( *it, Chan_Mask );		
		BOOST_CHECK_EQUAL( *(--it), Chan_Blue );		
		BOOST_CHECK_EQUAL( *(it--), Chan_Blue );		
		BOOST_CHECK_EQUAL( *(it--), Chan_Green );		
		BOOST_CHECK_EQUAL( *(it), *end );

		ChannelSet rgb( Mask_RGB );
		it = rgb.begin();
		end = rgb.end();
		BOOST_CHECK_EQUAL( *it, Chan_Red );		
		it = rgb.begin() + 1;
		BOOST_CHECK_EQUAL( *it, Chan_Green );
		it = rgb.begin() + 100;
		BOOST_CHECK_EQUAL( *it, *end );
		it = rgb.end() - 1;
		BOOST_CHECK_EQUAL( *it, Chan_Blue );
		it = rgb.end() - 100;
		BOOST_CHECK_EQUAL( *it, *rgb.begin() );

		it = --(rgb.begin() + 1);
		BOOST_CHECK_EQUAL( *it, Chan_Red );

		it = rgb.begin() + 1;
		rgb.erase( it );
		BOOST_CHECK_EQUAL( rgb, ChannelSet( Chan_Red | Chan_Blue ) );

		BOOST_CHECK_EQUAL( *it, Chan_Red );
	}

	void testIndex()
	{
		ChannelSet gbm( Mask_Mask | Mask_Green | Mask_Blue );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Green ), 0 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Blue ), 1 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Mask ), 2 );
		
		gbm += Chan_Red;	
		BOOST_CHECK_EQUAL( gbm.index( Chan_Red ), 0 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Green ), 1 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Blue ), 2 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Mask ), 3 );
		
		gbm += Chan_Alpha;	
		BOOST_CHECK_EQUAL( gbm.index( Chan_Red ), 0 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Green ), 1 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Blue ), 2 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Alpha ), 3 );
		BOOST_CHECK_EQUAL( gbm.index( Chan_Mask ), 4 );
	}

	void testNewFlags()
	{
		Channel red( ChannelSet::flag("red") );
		BOOST_CHECK_EQUAL( red, Chan_Red );
		
		Channel green( ChannelSet::flag("green") );
		BOOST_CHECK_EQUAL( green, Chan_Green );
		
		Channel f( ChannelSet::flag("new") );
		BOOST_CHECK_EQUAL( f, 11 );		
		
		Channel f2( ChannelSet::flag("new2") );
		BOOST_CHECK_EQUAL( f2, 12 );
		BOOST_CHECK_EQUAL( f, ChannelSet::findFlag("new") );
		BOOST_CHECK_EQUAL( f2, ChannelSet::findFlag("new2") );
		BOOST_CHECK_EQUAL( Chan_None, ChannelSet::findFlag("wrong") );
	}

	void testBracketAccessor()
	{
		ChannelSet set( Mask_Green | Mask_Blue | Mask_Alpha | Mask_Z );
		BOOST_CHECK_EQUAL( set[0], Chan_Green );		
		BOOST_CHECK_EQUAL( set[1], Chan_Blue );		
		BOOST_CHECK_EQUAL( set[2], Chan_Alpha );		
		BOOST_CHECK_EQUAL( set[3], Chan_Z );		
	}

	void testIoStream()
	{
		ChannelSet rgba( Mask_RGBA );
		std::stringstream s;
		s << rgba;
		BOOST_CHECK_EQUAL( s.str(), std::string( "red green blue alpha " ) );

		std::stringstream t;
		t << Chan_Blue;
		BOOST_CHECK_EQUAL( t.str(), std::string( "blue" ) );
		
		std::stringstream r;
		r << Mask_RGB;
		BOOST_CHECK_EQUAL( r.str(), std::string( "red green blue " ) );
	}

	void testIntersection()
	{
		ChannelSet rgba( Mask_RGBA );
		ChannelSet ga( Mask_Green | Mask_Alpha );
		BOOST_CHECK_EQUAL( rgba.intersection( ga ), ga);		
		BOOST_CHECK_EQUAL( rgba, ChannelSet( Mask_RGBA) );		
	}
	
	void testChannelMasks()
	{
		Channel red( Chan_Red );
		BOOST_CHECK( red == Chan_Red );

		Channel blue( Chan_Blue );
		blue = red;
		BOOST_CHECK( blue == Chan_Red );

		red = Chan_Green;
		BOOST_CHECK( red == Chan_Green );
	}
};

struct ChannelTestSuite : public boost::unit_test::test_suite
{
	ChannelTestSuite() : boost::unit_test::test_suite( "ChannelTestSuite" )
	{
		boost::shared_ptr<ChannelTest> instance( new ChannelTest() );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testClear, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testBracketAccessor, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testSize, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testChannelMasks, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testChannelSetConstructors, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testEnumOperators, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testEquality, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testArithmeticOperators, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testIntersection, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testIterator, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testIoStream, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testNewFlags, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testCombineMasks, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testIndex, instance ) );
	}
};

void addChannelTest( boost::unit_test::test_suite *test )
{
	test->add( new ChannelTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

