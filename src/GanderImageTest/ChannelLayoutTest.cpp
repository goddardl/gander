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

	void testTest()
	{
		typedef TestChannelLayout< float, Chan_Red > Layout1;
		typedef TestChannelLayout< short, Chan_Alpha > Layout2;
		typedef TestCompoundLayout< Layout1, Layout2 > Layout;
//		typename Layout::ChannelContainerType c;

		
		BOOST_CHECK_EQUAL( short(Layout::ChannelToLayoutIndex<Chan_Red>::Value), 0 );
		BOOST_CHECK_EQUAL( short(Layout::ChannelToLayoutIndex<Chan_Alpha>::Value), 1 );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Alpha >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::LayoutTraits< 0 >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::LayoutTraits< 1 >::LayoutType, Layout2 >::value ) );

		// If channels of different sizes are used, they should be aligned correctly in memory. This means that the final size of the ChannelContainerType
		// may be larger than the sum of it's parts. For example, combining a short with a float - 6 bytes - will be aligned to 8 bytes. 
		BOOST_CHECK( sizeof( typename Layout::ChannelContainerType ) >= sizeof( Layout1::ChannelContainerType ) + sizeof( Layout2::ChannelContainerType ) ); 
		Layout::ChannelContainerType container;
		Layout::channel< Chan_Red >( &container ) = 1.25;
		Layout::channel< Chan_Alpha >( &container ) = 1.25;
		
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Alpha>( &container ), 1 ); // The channel is cast to an short because that is the type that the alpha layout uses.
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Red>( &container ), 1.25 ); // There is no change to the value of the red channel because it's layout uses a float.
	
		Layout1::ChannelContainerType container2;
		Layout1::channel< Chan_Red >( &container2 ) = 1.25;

		const Layout::ChannelContainerType *constContainer = &container;
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Red>( constContainer ), 1.25 ); 
		
		const Layout1::ChannelContainerType *constContainer2 = &container2;
		BOOST_CHECK_EQUAL( Layout1::channel<Chan_Red>( constContainer2 ), 1.25 ); 
		
		Layout1::ChannelPointerContainerType ptrContainer;
		ptrContainer = new float[3];
		ptrContainer[0] = 3;
		ptrContainer[1] = 2;
		ptrContainer[2] = 1;
		BOOST_CHECK_EQUAL( ptrContainer[0], 3 ); 
		BOOST_CHECK_EQUAL( ptrContainer[1], 2 ); 
		BOOST_CHECK_EQUAL( ptrContainer[2], 1 ); 
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &ptrContainer ), 3 ); 
	
		Layout1::increment( &ptrContainer, 1 );
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &ptrContainer ), 2 ); 
		
		Layout1::increment( &ptrContainer, 1 );
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &ptrContainer ), 1 ); 
		
		Layout1::increment( &ptrContainer, -2 );
		BOOST_CHECK_EQUAL( Layout1::channel< Chan_Red >( &ptrContainer ), 3 ); 
		
		// Create a compound layout poshorter container.
		typename Layout::ChannelPointerContainerType ptrContainer2;

		// Test the accessor using an index.
		ptrContainer2[0] = new float [3];
		ptrContainer2[1] = new short [3];
		
		Layout::channel<Chan_Red>( &ptrContainer2 ) = 5.25;
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Red>( &ptrContainer2 ), 5.25 );
		
		Layout::channel<Chan_Alpha>( &ptrContainer2 ) = 5.25;
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Alpha>( &ptrContainer2 ), 5 );
		
		const Layout::ChannelPointerContainerType &constPtrContainer2( ptrContainer2 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Alpha>( &constPtrContainer2 ), 5 );
		
		((float*)ptrContainer2[0])[0] = 1.5; 
		((float*)ptrContainer2[0])[1] = 2.5; 
		((float*)ptrContainer2[0])[2] = 3.5; 
		((short*)ptrContainer2[1])[0] = 1.5; 
		((short*)ptrContainer2[1])[1] = 2.5; 
		((short*)ptrContainer2[1])[2] = 3.5; 
		
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Red>( &ptrContainer2 ), 1.5 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Alpha>( &ptrContainer2 ), 1 );

		Layout::increment( &ptrContainer2, 1 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Red>( &ptrContainer2 ), 2.5 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Alpha>( &ptrContainer2 ), 2 );
		
		Layout::increment( &ptrContainer2, 1 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Red>( &ptrContainer2 ), 3.5 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Alpha>( &ptrContainer2 ), 3 );
		
		Layout::increment( &ptrContainer2, -2 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Red>( &ptrContainer2 ), 1.5 );
		BOOST_CHECK_EQUAL( Layout::channel<Chan_Alpha>( &ptrContainer2 ), 1 );
		

		/*
		*ptrContainer2.get<0>() = 1.25;
		*ptrContainer2.get<1>() = 1.25;
		BOOST_CHECK_EQUAL( *ptrContainer2.get<0>(), 1.25 ); 
		BOOST_CHECK_EQUAL( *ptrContainer2.get<1>(), 1 );
		
		// Test the const accessor using an index.	
		const Layout::ChannelPointerContainerType &constPtrContainer2( ptrContainer2 );
		BOOST_CHECK_EQUAL( *constPtrContainer2.get<0>(), 1.25 ); 
		BOOST_CHECK_EQUAL( *constPtrContainer2.get<1>(), 1 );
		
		// Test the accessor using a channel.	
		*ptrContainer2.poshorterForChannel<Chan_Red>() = 3.25;
		*ptrContainer2.poshorterForChannel<Chan_Alpha>() = 3.25;
		BOOST_CHECK_EQUAL( *ptrContainer2.poshorterForChannel<Chan_Red>(), 3.25 ); 
		BOOST_CHECK_EQUAL( *ptrContainer2.poshorterForChannel<Chan_Alpha>(), 3 ); 
		
		// Test the const accessor using a channel.	
		BOOST_CHECK_EQUAL( *constPtrContainer2.poshorterForChannel<Chan_Red>(), 3.25 ); 
		BOOST_CHECK_EQUAL( *constPtrContainer2.poshorterForChannel<Chan_Alpha>(), 3 ); 
		
		// Test the channel method using a ChannelPointerContainer.
		Layout::channel<Chan_Red>( &ptrContainer2 );*/
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

