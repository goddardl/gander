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

		ChannelSet gb( Mask_Green | Mask_Blue );
		ChannelSet b( Mask_Blue );
		ChannelSet g( Mask_Green );
		BOOST_CHECK_EQUAL( b + Chan_Green, gb );
		BOOST_CHECK_EQUAL( b + Mask_Green, gb );
		BOOST_CHECK_EQUAL( b + g, gb );
		BOOST_CHECK_EQUAL( gb - Chan_Green, b );
		BOOST_CHECK_EQUAL( gb - Mask_Green, b );
		BOOST_CHECK_EQUAL( gb - g, b );
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
		ChannelSet gba( Mask_Alpha | Mask_Green | Mask_Blue );
		ChannelSet::const_iterator it( gba.begin() );
		ChannelSet::const_iterator end( gba.end() );
		BOOST_CHECK_EQUAL( *it, Chan_Green );		
		BOOST_CHECK_EQUAL( *it++, Chan_Green );
		BOOST_CHECK_EQUAL( *it, Chan_Blue );
		BOOST_CHECK_EQUAL( *(++it), Chan_Alpha );		
		BOOST_CHECK_EQUAL( *(++it), *end );

		it = gba.end();
		end = gba.end();
		BOOST_CHECK_EQUAL( *it--, Chan_Unused );		
		BOOST_CHECK_EQUAL( *it, Chan_Alpha );		
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

	void testNewFlags()
	{
		Channel red( ChannelSet::flag("red") );
		BOOST_CHECK_EQUAL( red, Chan_Red );
		
		Channel green( ChannelSet::flag("green") );
		BOOST_CHECK_EQUAL( green, Chan_Green );
		
		Channel f( ChannelSet::flag("new") );
		BOOST_CHECK_EQUAL( f, 9 );		
		
		Channel f2( ChannelSet::flag("new2") );
		BOOST_CHECK_EQUAL( f2, 10 );
		BOOST_CHECK_EQUAL( f, ChannelSet::findFlag("new") );
		BOOST_CHECK_EQUAL( f2, ChannelSet::findFlag("new2") );
		BOOST_CHECK_EQUAL( Chan_Unused, ChannelSet::findFlag("wrong") );
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
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testClear, instance ) );
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
	}
};

void addChannelTest( boost::unit_test::test_suite *test )
{
	test->add( new ChannelTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

