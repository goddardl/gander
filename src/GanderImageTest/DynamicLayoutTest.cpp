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

#include "GanderImage/DynamicLayout.h"
#include "GanderImageTest/DynamicLayoutTest.h"

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

struct DynamicLayoutTest
{
	void testCommonLayoutAttributes()
	{
		typedef DynamicLayout<float> Layout;
			
		BOOST_CHECK_EQUAL( int(Layout::IsDynamic), 1 );

		BOOST_CHECK( int( std::is_same< Layout::LayoutTraits<0>::LayoutType, Layout >::value ) );
		BOOST_CHECK( int( std::is_same< Layout::LayoutTraits<0>::StorageType, float >::value ) );
		BOOST_CHECK( int( std::is_same< Layout::ChannelTraits<Chan_Red>::StorageType, float >::value ) );
	}

	void testAddExistingChannel()
	{
		typedef DynamicLayout< float > Layout;
		Layout l;
		l.addChannels( Mask_Green | Mask_Blue, Brothers_RGB );
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Green | Mask_Blue  ) );

		// We should not be able to add a channel that already exists and an exception 
		// will be raised. Test that it is.
		BOOST_CHECK_THROW( l.addChannels( Chan_Green, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_Red, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Mask_RGB, Brothers_RGB ), std::runtime_error );
	}

	void testChannels()
	{
		typedef DynamicLayout< float > Layout;
		Layout l;
		l.addChannels( Mask_Red | Mask_Blue );
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Red | Mask_Blue ) );
		l.addChannels( Mask_U, Brothers_UV );
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Red | Mask_Blue | Mask_U ) );
	}
	
	void testCommonLayoutInterface()
	{
		DynamicLayout< float > l;
		l.addChannels( Mask_Red | Mask_Green, Brothers_RGB );
		l.addChannels( Mask_Alpha );
		l.addChannels( Mask_UV, Brothers_VU );
		
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Red | Mask_Green | Mask_UV | Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l.contains( ChannelSet( Mask_Red | Mask_Green | Mask_UV | Mask_Alpha ) ), true );
		BOOST_CHECK_EQUAL( l.contains( Chan_Z ), false );
		BOOST_CHECK_EQUAL( l.requiredChannels(), ChannelSet( Mask_Red | Mask_Green | Mask_UV | Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l.numberOfChannels(), 5 );
	}
	
	void testContainerAccess() 
	{
		typedef DynamicLayout< float > Layout;
		Layout layout;
		layout.addChannels( Mask_Blue | Mask_Red );

		Layout::ChannelContainerType c( layout );
		Layout::ChannelPointerContainerType cp( layout );
		
		BOOST_CHECK_EQUAL( cp.size(), layout.numberOfChannelPointers() );
		BOOST_CHECK_EQUAL( c.size(), layout.numberOfChannels() );
		BOOST_CHECK_EQUAL( layout.channels(), ChannelSet( Mask_Blue | Mask_Red ) );

		float br[2] = { 3., 2. };	
		layout.setChannelPointer< Layout::ChannelPointerContainerType >( cp, Chan_Blue, &br[0] );
		layout.setChannelPointer< Layout::ChannelPointerContainerType >( cp, Chan_Red, &br[1] );
		layout.channel< Layout::ChannelContainerType >( c, Chan_Blue ) = 3.;
		layout.channel< Layout::ChannelContainerType >( c, Chan_Red ) = 2.;
		
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType >( c, Chan_Blue ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType >( c, Chan_Red ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelPointerContainerType >( cp, Chan_Blue ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelPointerContainerType >( cp, Chan_Red ) ), 2. );
		
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType >( c, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType >( c, 1 ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 1 ) ), 3. );
		
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Blue >( c, 0 ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Red >( c, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelPointerContainerType, Mask_Blue >( cp, 0 ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelPointerContainerType, Mask_Red >( cp, 0 ) ), 2. );
		
		BOOST_CHECK_THROW( ( layout.channelAtIndex< Layout::ChannelPointerContainerType, Mask_Red >( cp, 1 ) ), std::runtime_error );
		BOOST_CHECK_THROW( ( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Red >( c, 1 ) ), std::runtime_error );

		Layout layout2( layout );
		layout.addChannels< Layout::ChannelContainerType >( c, Chan_U );
		layout.addChannels< Layout::ChannelContainerType >( c, Chan_Green );
		layout.channel< Layout::ChannelContainerType >( c, Chan_Green ) = 1.;
		layout.channel< Layout::ChannelContainerType >( c, Chan_U ) = 7.;
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType >( c, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType >( c, 1 ) ), 1. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType >( c, 2 ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType >( c, 3 ) ), 7. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType, ChannelMask( CombineMasks< Mask_U, Mask_Blue, Mask_Red >::Value ) >( c, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType, ChannelMask( CombineMasks< Mask_U, Mask_Blue, Mask_Red >::Value ) >( c, 1 ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< Layout::ChannelContainerType, ChannelMask( CombineMasks< Mask_U, Mask_Blue, Mask_Red >::Value ) >( c, 2 ) ), 7. );
		BOOST_CHECK_THROW( ( layout.channelAtIndex< Layout::ChannelContainerType, Mask_Red >( c, 3 ) ), std::runtime_error );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType >( c, Chan_Red ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType >( c, Chan_Green ) ), 1. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType >( c, Chan_Blue ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channel< Layout::ChannelContainerType >( c, Chan_U ) ), 7. );
		
		// Test that an exception is thrown if the container has a different number of elements than the layout's number of channels.
		layout.addChannels( Chan_V );
		BOOST_CHECK( c.size() != layout.numberOfChannels() );
		BOOST_CHECK_THROW( ( layout.channel< Layout::ChannelContainerType >( c, Chan_V ) ), std::runtime_error );

		layout2.addChannels< Layout::ChannelPointerContainerType >( cp, Chan_Alpha );
		layout2.addChannels< Layout::ChannelPointerContainerType >( cp, Chan_V );
		float av[2] = { 6., 5. };	
		
		BOOST_CHECK_EQUAL( cp.size(), layout2.numberOfChannelPointers() );
		layout2.setChannelPointer< Layout::ChannelPointerContainerType >( cp, Chan_Alpha, &av[0] );
		layout2.setChannelPointer< Layout::ChannelPointerContainerType >( cp, Chan_V, &av[1] );
		BOOST_CHECK_EQUAL( ( layout2.channel< Layout::ChannelPointerContainerType >( cp, Chan_Red ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout2.channel< Layout::ChannelPointerContainerType >( cp, Chan_Blue ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout2.channel< Layout::ChannelPointerContainerType >( cp, Chan_Alpha ) ), 6. );
		BOOST_CHECK_EQUAL( ( layout2.channel< Layout::ChannelPointerContainerType >( cp, Chan_V ) ), 5. );
		BOOST_CHECK_EQUAL( ( layout2.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 0 ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout2.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 1 ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout2.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 2 ) ), 6. );
		BOOST_CHECK_EQUAL( ( layout2.channelAtIndex< Layout::ChannelPointerContainerType >( cp, 3 ) ), 5. );
		layout2.channel< Layout::ChannelPointerContainerType >( cp, Chan_V ) = 1.;
		BOOST_CHECK_EQUAL( av[1], 1. );
	}

	void testMaskedChannelIndex()
	{
		DynamicLayout< float > l;
		l.addChannels( Mask_Red | Mask_Green, Brothers_BGR );
		l.addChannels( Mask_Alpha );
		l.addChannels( Mask_UV, Brothers_VU );
		BOOST_CHECK_EQUAL( l.maskedChannelIndex( 0 ), 0 );
		BOOST_CHECK_EQUAL( l.maskedChannelIndex( 1 ), 1 );
		BOOST_CHECK_EQUAL( l.maskedChannelIndex( 2 ), 2 );
		BOOST_CHECK_EQUAL( l.maskedChannelIndex( 3 ), 3 );
		BOOST_CHECK_EQUAL( l.maskedChannelIndex( 4 ), 4 );

		BOOST_CHECK_EQUAL( int( l.maskedChannelIndex< ChannelMask( CombineMasks< Mask_UV, Mask_Green >::Value ) >( 0 ) ), 1 );
		BOOST_CHECK_EQUAL( int( l.maskedChannelIndex< ChannelMask( CombineMasks< Mask_UV, Mask_Green >::Value ) >( 1 ) ), 3 );
		BOOST_CHECK_EQUAL( int( l.maskedChannelIndex< ChannelMask( CombineMasks< Mask_UV, Mask_Green >::Value ) >( 2 ) ), 4 );
		BOOST_CHECK_EQUAL( int( l.maskedChannelIndex< ChannelMask( CombineMasks< Mask_UV, Mask_Red >::Value ) >( 0 ) ), 0 );
		BOOST_CHECK_EQUAL( int( l.maskedChannelIndex< Mask_UV>( 1 ) ), 4 );
		BOOST_CHECK_EQUAL( int( l.maskedChannelIndex< Mask_UV>( 0 ) ), 3 );
		BOOST_CHECK_EQUAL( int( l.maskedChannelIndex< Mask_UV>( 1 ) ), 4 );
		BOOST_CHECK_THROW( int( l.maskedChannelIndex< ChannelMask( CombineMasks< Mask_UV, Mask_Green >::Value ) >( 3 ) ), std::runtime_error );
		
	}
	
	void testAddChannelWhichIsNotABrother()
	{
		typedef DynamicLayout< float > Layout;
		Layout l;
		BOOST_CHECK_THROW( l.addChannels( Chan_U, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_V, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_Mask, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_Red, Brothers_UV ), std::runtime_error );
	}

};

struct DynamicLayoutTestSuite : public boost::unit_test::test_suite
{
	DynamicLayoutTestSuite() : boost::unit_test::test_suite( "DynamicLayoutTestSuite" )
	{
		boost::shared_ptr<DynamicLayoutTest> instance( new DynamicLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testAddExistingChannel, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testChannels, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testCommonLayoutInterface, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testAddChannelWhichIsNotABrother, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testMaskedChannelIndex, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testContainerAccess, instance ) );
	}
};

void addDynamicLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new DynamicLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

