//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
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

#include "GanderImage/Layout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImageTest/ChannelLayoutTest.h"

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

struct ChannelLayoutTest
{
	void testCommonLayoutInterface()
	{
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef ChannelLayout<short, Chan_Z> Layout2;
		typedef ChannelLayout<short, Chan_Red> Layout3;

		Layout1 l1;
		Layout2 l2;
		Layout3 l3;

		BOOST_CHECK( short( l1 == l1 ) );
		BOOST_CHECK( short( l2 == l2 ) );
		BOOST_CHECK( short( l3 == l3 ) );
		BOOST_CHECK_EQUAL( short( l3 == l1 ), 0 );
		BOOST_CHECK_EQUAL( short( l3 == l2 ), 0 );
		BOOST_CHECK_EQUAL( short( l1 == l2 ), 0 );
		
		BOOST_CHECK( short( l1 != l3 ) );
		BOOST_CHECK( short( l2 != l3 ) );
		BOOST_CHECK( short( l3 != l1 ) );
		
		BOOST_CHECK_EQUAL( l1.contains( Chan_Alpha ), true );
		BOOST_CHECK_EQUAL( l1.contains( Chan_Red ), false );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Z ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Alpha ), false );
		BOOST_CHECK_EQUAL( l3.contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( l3.contains( Chan_Z ), false );

		BOOST_CHECK_EQUAL( l1.channels(), ChannelSet( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l2.channels(), ChannelSet( Mask_Z ) );
		BOOST_CHECK_EQUAL( l3.channels(), ChannelSet( Mask_Red ) );
		
		BOOST_CHECK_EQUAL( l1.numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( l2.numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( l3.numberOfChannels(), 1 );
		
		BOOST_CHECK_EQUAL( l1.requiredChannels(), ChannelSet( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l2.requiredChannels(), ChannelSet( Mask_Z ) );
		BOOST_CHECK_EQUAL( l3.requiredChannels(), ChannelSet( Mask_Red ) );
	}

	void testTestChannelLayoutAccessors()
	{
		typedef TestChannelLayout< float, Chan_Red > Layout1;
		typedef TestChannelLayout< short, Chan_Alpha > Layout2;
		Layout1::ChannelContainerType redContainer;
		Layout2::ChannelContainerType alphaContainer;
	
		// Test the layouts have the correct channels.	
		BOOST_CHECK_EQUAL( Layout1::numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( Layout1::channels(), ChannelSet( Mask_Red ) );
		BOOST_CHECK_EQUAL( Layout1::contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( Layout2::numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( Layout2::channels(), ChannelSet( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( Layout2::contains( Chan_Alpha ), true );

		// Test the static and runtime channel accessors.
		Layout1::channel< Chan_Red >( &redContainer ) = 1.25;
		Layout2::channel< Chan_Alpha >( &alphaContainer ) = 1.25;
		
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &redContainer ), 1.25 );
		BOOST_CHECK_EQUAL( Layout1::getChannel( &redContainer, Chan_Red ), 1.25 );
		BOOST_CHECK_EQUAL( Layout1::getChannel< int >( &redContainer, Chan_Red ), 1 );

		Layout1::setChannel( &redContainer, Chan_Red, 2.5 );
		BOOST_CHECK_EQUAL( redContainer[0], 2.5 );

		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( &alphaContainer ), 1 ); // The result is cropped because the channel is an int.
		BOOST_CHECK_EQUAL( Layout2::getChannel( &alphaContainer, Chan_Alpha ), 1 );
		
		// Test the static and runtime const channel accessors.
		const Layout1::ChannelContainerType *constRedContainer = &redContainer;
		const Layout2::ChannelContainerType *constAlphaContainer = &alphaContainer;
		
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( constRedContainer ), 2.5 );
		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( constAlphaContainer ), 1. );
		BOOST_CHECK_EQUAL( Layout1::getChannel( constRedContainer, Chan_Red ), 2.5 );
		BOOST_CHECK_EQUAL( Layout2::getChannel( constAlphaContainer, Chan_Alpha ), 1. );

		// Test the static and runtime channel pointer accessors.
		Layout1::ChannelPointerContainerType redPtrContainer;
		Layout2::ChannelPointerContainerType alphaPtrContainer;
		
		float red[3] = { 1.5, 9.25, 8.5 };
		short alpha[3] = { 2, 1, 3 };
		redPtrContainer[0] = red;	
		alphaPtrContainer[0] = alpha;
		
		Layout1::channel< Chan_Red >( &redPtrContainer ) = 1.5;
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &redPtrContainer ), 1.5 );
		BOOST_CHECK_EQUAL( red[0], 1.5 );
		Layout1::setChannel( &redPtrContainer, Chan_Red, 1.25 );
		BOOST_CHECK_EQUAL( Layout1::getChannel( &redPtrContainer, Chan_Red ), 1.25 );
		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( &alphaPtrContainer ), 2 );
		BOOST_CHECK_EQUAL( alpha[0], 2 );

		// Test the const channel pointer accessors.
		const Layout1::ChannelPointerContainerType *constRedPtrContainer = &redPtrContainer;
		const Layout2::ChannelPointerContainerType *constAlphaPtrContainer = &alphaPtrContainer;
		Layout1::setChannel( &redPtrContainer, Chan_Red, 1.25 );
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( constRedPtrContainer ), 1.25 );
		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( constAlphaPtrContainer ), 2 );
	
		// Test that we can increment the channel pointers.	
		Layout1::increment( &redPtrContainer );
		Layout2::increment( &alphaPtrContainer );
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &redPtrContainer ), 9.25 );
		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( &alphaPtrContainer ), 1 );
		
		Layout1::increment( &redPtrContainer, 1 );
		Layout2::increment( &alphaPtrContainer, 1 );
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &redPtrContainer ), 8.5 );
		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( &alphaPtrContainer ), 3 );
		
		Layout2::channel< Chan_Alpha >( &alphaPtrContainer ) = 10;
		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( &alphaPtrContainer ), 10 );
		BOOST_CHECK_EQUAL( alpha[2], 10 );
		
		Layout1::increment( &redPtrContainer, -2 );
		Layout2::increment( &alphaPtrContainer, -2 );
		
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &redPtrContainer ), 1.25 );
		BOOST_CHECK_EQUAL( Layout2::channel< Chan_Alpha >( &alphaPtrContainer ), 2 );
	}

	void testTest()
	{
		typedef TestChannelLayout< float, Chan_Red > Layout1;
		typedef TestChannelLayout< short, Chan_Alpha > Layout2;
		typedef TestCompoundLayout< Layout1, Layout2 > Layout;
		
		// Test the layouts have the correct channels.	
		BOOST_CHECK_EQUAL( Layout1::numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( Layout1::channels(), ChannelSet( Mask_Red ) );
		BOOST_CHECK_EQUAL( Layout1::contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( Layout2::numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( Layout2::channels(), ChannelSet( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( Layout2::contains( Chan_Alpha ), true );
		BOOST_CHECK_EQUAL( Layout::numberOfChannels(), 2 );
		BOOST_CHECK_EQUAL( Layout::channels(), ChannelSet( Mask_Red | Mask_Alpha ) );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Alpha ), true );
		
		// If channels of different sizes are used, they should be aligned correctly in memory. This means that the final size of the ChannelContainerType
		// may be larger than the sum of it's parts. For example, combining a short with a float - 6 bytes - will be aligned to 8 bytes. 
		BOOST_CHECK( sizeof( typename Layout::ChannelContainerType ) >= sizeof( Layout1::ChannelContainerType ) + sizeof( Layout2::ChannelContainerType ) ); 
	
		// Test the static and runtime channel accessors using the compound channel container.
		Layout::ChannelContainerType raContainer;
		raContainer.child<0>()[0] = 1.;
		raContainer.child<1>()[0] = 2.5;

		BOOST_CHECK_EQUAL( raContainer.child<0>()[0], 1. );
		BOOST_CHECK_EQUAL( raContainer.child<1>()[0], 2 ); // The result is rounded as it is an int.

		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( &raContainer ), 1. );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( &raContainer ), 2 );

		Layout::setChannel( &raContainer, Chan_Red, 4.5 );
		BOOST_CHECK_EQUAL( Layout::getChannel( &raContainer, Chan_Red ), 4.5 );
		Layout::setChannel<short>( &raContainer, Chan_Alpha, 6.5 );
		BOOST_CHECK_EQUAL( Layout::getChannel( &raContainer, Chan_Alpha ), 6 );
		BOOST_CHECK_THROW( Layout::getChannel( &raContainer, Chan_Blue ), std::runtime_error );
		
		Layout::setChannel( &raContainer, Chan_Alpha, 3.75 );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( &raContainer ), 3 );
		Layout::setChannel( &raContainer, Chan_Alpha, 4.5 );
		BOOST_CHECK_EQUAL( Layout::getChannel( &raContainer, Chan_Alpha ), 4 );
		Layout::channel<Chan_Red>( &raContainer ) = 3.125;
		BOOST_CHECK_EQUAL( raContainer.child<0>()[0], 3.125 );
		
		Layout::channel< Chan_Alpha >( &raContainer ) = 6;
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( &raContainer ), 6 );
		
		// Test the const channel accessor using the compound channel container.
		const Layout::ChannelContainerType *constRaContainer = &raContainer;
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( constRaContainer ), 3.125 );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( constRaContainer ), 6 );
		BOOST_CHECK_EQUAL( Layout::getChannel( constRaContainer, Chan_Alpha ), 6 );
		
		// Test the static and runtime channel accessors using the compound channel pointer container.
		Layout::ChannelPointerContainerType raPtrContainer;
		float red[3] = { 1.5, 3., 5. };
		raPtrContainer.child<0>()[0] = red;
		
		short alpha[3] = { 3, 9, 1 };
		raPtrContainer.child<1>()[0] = alpha;
		
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( &raPtrContainer ), 1.5 );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( &raPtrContainer ), 3 );
		
		Layout::channel< Chan_Red >( &raPtrContainer ) = 4.25;
		Layout::channel< Chan_Alpha >( &raPtrContainer ) = 4.25;
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( &raPtrContainer ), 4.25 );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( &raPtrContainer ), 4 );
		
		// Test the static and runtime channel accessors using a const compound channel pointer container.
		const Layout::ChannelPointerContainerType *constRaPtrContainer = &raPtrContainer;
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( constRaPtrContainer ), 4.25 );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( constRaPtrContainer ), 4 );

		// Test that we can increment channel pointers.
		Layout::increment( &raPtrContainer );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( constRaPtrContainer ), 3. );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( constRaPtrContainer ), 9 );
		
		Layout::increment( &raPtrContainer );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( constRaPtrContainer ), 5. );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( constRaPtrContainer ), 1 );
		
		Layout::increment( &raPtrContainer, -2 );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Red >( constRaPtrContainer ), 4.25 );
		BOOST_CHECK_EQUAL( Layout::channel< Chan_Alpha >( constRaPtrContainer ), 4 );
	}

	void testChannelTraits()
	{
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef ChannelLayout<short, Chan_Z> Layout2;
		typedef ChannelLayout<short, Chan_Red> Layout3;

		BOOST_CHECK( short( std::is_same< Layout1::ChannelTraits<Chan_Alpha>::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( short( std::is_same< Layout2::ChannelTraits<Chan_Z>::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( short( std::is_same< Layout3::ChannelTraits<Chan_Red>::LayoutType, Layout3 >::value ) );
		
		BOOST_CHECK( short( std::is_same< Layout1::ChannelTraits<Chan_Alpha>::StorageType, float >::value ) );
		BOOST_CHECK( short( std::is_same< Layout2::ChannelTraits<Chan_Z>::StorageType, short >::value ) );
		BOOST_CHECK( short( std::is_same< Layout3::ChannelTraits<Chan_Red>::StorageType, short >::value ) );
	}

	void testContainerAccess() 
	{
		typedef ChannelLayout< float, Chan_Green > Layout;
		Layout layout;
		Layout::ChannelContainerType c( layout );
		Layout::ChannelPointerContainerType cp( layout );
		
		BOOST_CHECK_EQUAL( cp.size(), layout.numberOfChannelPointers() );
		BOOST_CHECK_EQUAL( c.size(), layout.numberOfChannels() );
		
		BOOST_CHECK_EQUAL( layout.channels(), ChannelSet( Mask_Green ) );
		
		layout.setChannel< Layout::ChannelContainerType >( c, Chan_Green, 2. );
		
		BOOST_CHECK_EQUAL( ( layout.getChannel< Layout::ChannelContainerType, float >( c, Chan_Green ) ), 2. );
		BOOST_CHECK_THROW( ( layout.getChannel< Layout::ChannelContainerType, float >( c, Chan_Red ) ), std::runtime_error );

		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType >( c, 0 ) ), 2. );
		BOOST_CHECK_THROW( ( layout.channelAtIndex< Layout::ChannelContainerType >( c, 1 ) ), std::runtime_error );

		BOOST_CHECK_THROW( ( layout.getChannel< Layout::ChannelContainerType, float >( c, Chan_Red ) ), std::runtime_error );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelContainerType >( c, 0 ) ), 2. );
		BOOST_CHECK_THROW( layout.channelAtIndex< Layout::ChannelContainerType >( c, 1 ), std::runtime_error );
	
		float green = 3.;	
		layout.setChannelPointer< Layout::ChannelPointerContainerType >( cp, Chan_Green, &green );
		BOOST_CHECK_EQUAL( ( layout.getChannel< Layout::ChannelPointerContainerType, float >( cp, Chan_Green ) ), 3. );
		BOOST_CHECK_THROW( ( layout.getChannel< Layout::ChannelPointerContainerType, float >( cp, Chan_Blue ) ), std::runtime_error );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 0 ) ), 3. );
		BOOST_CHECK_THROW( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 1 ), std::runtime_error );
		layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 0 ) = 1.;
		BOOST_CHECK_EQUAL( green, 1. );
	}

	void testCommonLayoutAttributes()
	{
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef ChannelLayout<float, Chan_Z> Layout2;
		typedef ChannelLayout<float, Chan_Red> Layout3;
			
		BOOST_CHECK_EQUAL( short(Layout1::NumberOfChannels), 1 );
		BOOST_CHECK_EQUAL( short(Layout2::NumberOfChannels), 1 );
		BOOST_CHECK_EQUAL( short(Layout3::NumberOfChannels), 1 );

		BOOST_CHECK_EQUAL( short(Layout1::ChannelMask), short( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( short(Layout2::ChannelMask), short( Mask_Z ) );
		BOOST_CHECK_EQUAL( short(Layout3::ChannelMask), short( Mask_Red ) );
	}
};

struct ChannelLayoutTestSuite : public boost::unit_test::test_suite
{
	ChannelLayoutTestSuite() : boost::unit_test::test_suite( "ChannelLayoutTestSuite" )
	{
		boost::shared_ptr<ChannelLayoutTest> instance( new ChannelLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testTest, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testTestChannelLayoutAccessors, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testCommonLayoutInterface, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testContainerAccess, instance ) );
	}
};

void addChannelLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new ChannelLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

