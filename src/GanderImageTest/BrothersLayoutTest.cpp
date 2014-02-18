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

#include "GanderImage/BrothersLayout.h"
#include "GanderImageTest/BrothersLayoutTest.h"

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

struct BrothersLayoutTest
{
	void testCommonLayoutInterface()
	{
		typedef BrothersLayout<float, Brothers_BGR> Layout1;
		typedef BrothersLayout<int, Brothers_RGBA> Layout2;
		typedef BrothersLayout<short, Brothers_VU> Layout3;

		Layout1 l1;
		Layout2 l2;
		Layout3 l3;

		BOOST_CHECK( int( l1 == l1 ) );
		BOOST_CHECK( int( l2 == l2 ) );
		BOOST_CHECK( int( l3 == l3 ) );
		BOOST_CHECK_EQUAL( int( l3 == l1 ), 0 );
		BOOST_CHECK_EQUAL( int( l3 == l2 ), 0 );
		BOOST_CHECK_EQUAL( int( l1 == l2 ), 0 );
		
		BOOST_CHECK( int( l1 != l3 ) );
		BOOST_CHECK( int( l2 != l3 ) );
		BOOST_CHECK( int( l3 != l1 ) );

		BOOST_CHECK_EQUAL( l1.channels(), ChannelSet( Mask_RGB ) );
		BOOST_CHECK_EQUAL( l2.channels(), ChannelSet( Mask_RGBA ) );
		BOOST_CHECK_EQUAL( l3.channels(), ChannelSet( Mask_UV ) );
		
		BOOST_CHECK_EQUAL( l1.numberOfChannels(), 3 );
		BOOST_CHECK_EQUAL( l2.numberOfChannels(), 4 );
		BOOST_CHECK_EQUAL( l3.numberOfChannels(), 2 );
		
		BOOST_CHECK_EQUAL( l1.requiredChannels(), ChannelSet( Mask_Blue ) );
		BOOST_CHECK_EQUAL( l2.requiredChannels(), ChannelSet( Mask_Red ) );
		BOOST_CHECK_EQUAL( l3.requiredChannels(), ChannelSet( Mask_V ) );
		
		BOOST_CHECK_EQUAL( l1.contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( l1.contains( Chan_Blue ), true );
		BOOST_CHECK_EQUAL( l1.contains( Chan_Green ), true );
		BOOST_CHECK_EQUAL( l1.contains( Chan_Alpha ), false );
		
		BOOST_CHECK_EQUAL( l2.contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Blue ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Green ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Alpha ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Z ), false );
		
		BOOST_CHECK_EQUAL( l3.contains( Chan_Alpha ), false );
		BOOST_CHECK_EQUAL( l3.contains( Chan_V ), true );
		BOOST_CHECK_EQUAL( l3.contains( Chan_U ), true );
		BOOST_CHECK_EQUAL( l3.contains( Mask_UV ), true );
		
		BOOST_CHECK_EQUAL( l1.step( Chan_Red ), 3 );
		BOOST_CHECK_EQUAL( l1.step( Chan_Blue ), 3 );
		BOOST_CHECK_EQUAL( l1.step( Chan_Green ), 3 );
		BOOST_CHECK_EQUAL( l1.step<Chan_Red>(), 3 );
		BOOST_CHECK_EQUAL( l1.step<Chan_Blue>(), 3 );
		BOOST_CHECK_EQUAL( l1.step<Chan_Green>(), 3 );

		BOOST_CHECK_EQUAL( l2.step( Chan_Red ), 4 );
		BOOST_CHECK_EQUAL( l2.step( Chan_Blue ), 4 );
		BOOST_CHECK_EQUAL( l2.step( Chan_Green ), 4 );
		BOOST_CHECK_EQUAL( l2.step( Chan_Alpha ), 4 );
		BOOST_CHECK_EQUAL( l2.step<Chan_Red>(), 4 );
		BOOST_CHECK_EQUAL( l2.step<Chan_Blue>(), 4 );
		BOOST_CHECK_EQUAL( l2.step<Chan_Green>(), 4 );
		BOOST_CHECK_EQUAL( l2.step<Chan_Alpha>(), 4 );
		
		BOOST_CHECK_EQUAL( l3.step( Chan_U ), 2 );
		BOOST_CHECK_EQUAL( l3.step( Chan_V ), 2 );
		BOOST_CHECK_EQUAL( l3.step<Chan_U>(), 2 );
		BOOST_CHECK_EQUAL( l3.step<Chan_V>(), 2 );
		
		BOOST_CHECK_THROW( l1.step( Chan_Alpha ), std::runtime_error );
		BOOST_CHECK_THROW( l1.step<Chan_Alpha>(), std::runtime_error );
	}

	void testChannelTraits()
	{
		typedef BrothersLayout<float, Brothers_BGR> Layout1;
		typedef BrothersLayout<int, Brothers_RGBA> Layout2;
		typedef BrothersLayout<short, Brothers_VU> Layout3;

		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Red>::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Green>::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Blue>::LayoutType, Layout1 >::value ) );
		
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Red>::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Green>::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Blue>::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Alpha>::LayoutType, Layout2 >::value ) );
		
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_U>::LayoutType, Layout3 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_V>::LayoutType, Layout3 >::value ) );
		
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Red>::StorageType, float >::value ) );
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Green>::StorageType, float >::value ) );
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Blue>::StorageType, float >::value ) );

		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Red>::StorageType, int >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Green>::StorageType, int >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Blue>::StorageType, int >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Alpha>::StorageType, int >::value ) );
		
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_U>::StorageType, short >::value ) );
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_V>::StorageType, short >::value ) );
	};

	void testCommonLayoutAttributes()
	{
		typedef BrothersLayout<float, Brothers_BGR> Layout1;
		typedef BrothersLayout<int, Brothers_RGBA> Layout2;
		typedef BrothersLayout<short, Brothers_VU> Layout3;
			
		BOOST_CHECK_EQUAL( int(Layout1::NumberOfChannels), 3 );
		BOOST_CHECK_EQUAL( int(Layout2::NumberOfChannels), 4 );
		BOOST_CHECK_EQUAL( int(Layout3::NumberOfChannels), 2 );

		BOOST_CHECK_EQUAL( int(Layout1::ChannelMask), int( Mask_RGB ) );
		BOOST_CHECK_EQUAL( int(Layout2::ChannelMask), int( Mask_RGBA ) );
		BOOST_CHECK_EQUAL( int(Layout3::ChannelMask), int( Mask_UV ) );
	}
	
	void testMaskedChannelIndex()
	{
		{
			typedef BrothersLayout< float, Brothers_UV > Layout;
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_UV >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, Mask_UV >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_U >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_V >::Value ), 1 );
		}

		{
			typedef BrothersLayout< float, Brothers_BGRA > Layout;
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_Red >::Value ), 2 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_Green >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_Blue >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_Alpha >::Value ), 3 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Red, Mask_Green >::Value ) >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Red, Mask_Green >::Value ) >::Value ), 2 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Blue, Mask_Green >::Value ) >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Blue, Mask_Green >::Value ) >::Value ), 1 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >::Value ), 3 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Blue >::Value ) >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Blue >::Value ) >::Value ), 3 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Red >::Value ) >::Value ), 2 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Red >::Value ) >::Value ), 3 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green, Mask_Red >::Value ) >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green, Mask_Red >::Value ) >::Value ), 2 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 2, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green, Mask_Red >::Value ) >::Value ), 3 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green, Mask_Blue >::Value ) >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green, Mask_Blue >::Value ) >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 2, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green, Mask_Blue >::Value ) >::Value ), 3 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Red, Mask_Green, Mask_Blue >::Value ) >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Red, Mask_Green, Mask_Blue >::Value ) >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 2, ChannelMask( CombineMasks< Mask_Red, Mask_Green, Mask_Blue >::Value ) >::Value ), 2 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, ChannelMask( CombineMasks< Mask_Red, Mask_Alpha, Mask_Blue >::Value ) >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, ChannelMask( CombineMasks< Mask_Red, Mask_Alpha, Mask_Blue >::Value ) >::Value ), 2 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 2, ChannelMask( CombineMasks< Mask_Red, Mask_Alpha, Mask_Blue >::Value ) >::Value ), 3 );

			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 0, Mask_RGBA >::Value ), 0 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 1, Mask_RGBA >::Value ), 1 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 2, Mask_RGBA >::Value ), 2 );
			BOOST_CHECK_EQUAL( int( Layout::MaskedChannelIndex< 3, Mask_RGBA >::Value ), 3 );
		}

	}
	
	void testContainerAccess()
	{
		typedef BrothersLayout< float, Brothers_BGRA > Layout;
		Layout layout;
		Layout::ChannelContainerType c;
		Layout::ChannelPointerContainerType cp;
		
		BOOST_CHECK_EQUAL( layout.channels(), ChannelSet( Mask_RGBA ) );
		
		layout.channel< Layout::ChannelContainerType, Chan_Red >( c ) = 1.;
		layout.channel< Layout::ChannelContainerType, Chan_Green >( c ) = 2.;
		layout.channel< Layout::ChannelContainerType, Chan_Blue >( c ) = 3.;
		layout.channel< Layout::ChannelContainerType, Chan_Alpha >( c ) = 4.;
	
		float bgra[4] = { 3., 2., 1., 4. };	
		layout.setChannelPointer< Layout::ChannelPointerContainerType >( cp, Chan_Blue, &bgra[0] );
		
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType, Chan_Red >( c ) ), 1. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType, Chan_Green >( c ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType, Chan_Blue >( c ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType, Chan_Alpha >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 0 >( c ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 1 >( c ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 2 >( c ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 3 >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType >( c, 0 ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType >( c, 1 ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType >( c, 2 ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType >( c, 3 ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 0, Mask_Red >( c ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 0, Mask_Blue >( c ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 0, Mask_Green >( c ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 0, Mask_Alpha >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Red >( c, 0 ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Blue >( c, 0 ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Green >( c, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Alpha >( c, 0 ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >( c ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType, 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >( c ) ), 4. );

		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelPointerContainerType, Chan_Red >( cp ) ), 1. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelPointerContainerType, Chan_Green >( cp ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelPointerContainerType, Chan_Blue >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelPointerContainerType, Chan_Alpha >( cp ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 0 >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 1 >( cp ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 2 >( cp ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 3 >( cp ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 0 ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 1 ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 2 ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 3 ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 0, Mask_Red >( cp ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 0, Mask_Blue >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 0, Mask_Green >( cp ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 0, Mask_Alpha >( cp ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, Mask_Red >( cp, 0 ) ), 1. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, Mask_Blue >( cp, 0 ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, Mask_Green >( cp, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, Mask_Alpha >( cp, 0 ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >( cp ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType, 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >( cp ) ), 4. );
	}
};

struct BrothersLayoutTestSuite : public boost::unit_test::test_suite
{
	BrothersLayoutTestSuite() : boost::unit_test::test_suite( "BrothersLayoutTestSuite" )
	{
		boost::shared_ptr<BrothersLayoutTest> instance( new BrothersLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testContainerAccess, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testCommonLayoutInterface, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testMaskedChannelIndex, instance ) );
	}
};

void addBrothersLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new BrothersLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

