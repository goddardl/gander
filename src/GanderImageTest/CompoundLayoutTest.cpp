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

#include "GanderImage/CompoundLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImageTest/CompoundLayoutTest.h"

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

struct CompoundLayoutTest
{
	void testTraits()
	{
		/// Create 4 test storage types that we can use to assert that access to the LayoutTypes works correctly.
		typedef BrothersLayout<int, Brothers_RGB> Layout1;
		typedef ChannelLayout< short, Chan_Alpha > Layout2;
		typedef ChannelLayout< short, Chan_Z > Layout3;
		typedef BrothersLayout<short, Brothers_VU> Layout4;

		typedef CompoundLayout< Layout1, Layout2, Layout3, Layout4 > Layout;
		BOOST_CHECK_EQUAL( int(Layout::NumberOfLayouts), 4 );
		BOOST_CHECK( ( std::is_same< Layout::LayoutTraits< 0 >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::LayoutTraits< 1 >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::LayoutTraits< 2 >::LayoutType, Layout3 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::LayoutTraits< 3 >::LayoutType, Layout4 >::value ) );

		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Red >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Green >::LayoutType,  Layout1 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Blue >::LayoutType,  Layout1 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Alpha >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Z >::LayoutType, Layout3 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_V >::LayoutType, Layout4 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_U >::LayoutType, Layout4 >::value ) );
	};
	
	void testAccessors()
	{
		typedef ChannelLayout< float, Chan_Red > Layout1;
		typedef ChannelLayout< short, Chan_Alpha > Layout2;
		typedef CompoundLayout< Layout1, Layout2 > Layout;
	
		std::cerr << "Add a BrothersLayout test to the CompoundLayoutTest::testAccessors()" << std::endl;	

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

	void testContains()
	{
		typedef BrothersLayout<float, Brothers_RGB> Layout0;
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef BrothersLayout<float, Brothers_VU> Layout2;
		
		typedef CompoundLayout< Layout0, Layout1, Layout2 > Layout;
		Layout l;
		
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_UV | Mask_RGB | Mask_Alpha ) );
		BOOST_CHECK_EQUAL( int( l.numberOfChannels() ), 6 );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Alpha ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Blue ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Red ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Green ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_U ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_V ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Z ) ), false );
	}

	void testCommonLayoutAttributes()
	{
		typedef BrothersLayout<float, Brothers_RGB> Layout0;
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef ChannelLayout<float, Chan_Z> Layout2;
		typedef BrothersLayout<float, Brothers_VU> Layout3;
			
		typedef CompoundLayout< Layout0, Layout1, Layout2, Layout3 > Layout;
		BOOST_CHECK_EQUAL( int(Layout::NumberOfLayouts), 4 );
		BOOST_CHECK_EQUAL( int(Layout::NumberOfChannels), 7 );
		BOOST_CHECK_EQUAL( int(Layout::ChannelMask), int( Mask_RGB | Mask_Alpha | Mask_Z | Mask_UV ) );

		Layout l;
		BOOST_CHECK_EQUAL( int( l.numberOfChannels() ), 7 );
		
		BOOST_CHECK_EQUAL( Layout() == Layout(), true );
		BOOST_CHECK_EQUAL( Layout() == Layout0(), false );
		BOOST_CHECK_EQUAL( Layout() == Layout2(), false );
		BOOST_CHECK_EQUAL( Layout() != Layout2(), true );
		BOOST_CHECK_EQUAL( Layout() != Layout(), false );
		BOOST_CHECK_EQUAL( Layout() != l, false );
		
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Alpha ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Blue ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Green ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_U ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_V ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Z ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Mask_Z | Mask_U | Mask_Red ), true );
		BOOST_CHECK_EQUAL( Layout::contains( Mask_Z | Mask_U | Mask_Mask ), false );
		BOOST_CHECK_EQUAL( Layout::contains( Chan_Mask ), false );
	}
};

struct CompoundLayoutTestSuite : public boost::unit_test::test_suite
{
	CompoundLayoutTestSuite() : boost::unit_test::test_suite( "CompoundLayoutTestSuite" )
	{
		boost::shared_ptr<CompoundLayoutTest> instance( new CompoundLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testContains, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testAccessors, instance ) );
	}
};

void addCompoundLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new CompoundLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

