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
		
		BOOST_CHECK_EQUAL( l1.contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( l1.contains( Chan_Blue ), true );
		BOOST_CHECK_EQUAL( l1.contains( Chan_Green ), true );
		BOOST_CHECK_EQUAL( l1.contains( Chan_Alpha ), false );
		
		BOOST_CHECK_EQUAL( l2.contains( Chan_Red ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Blue ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Green ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Alpha ), true );
		BOOST_CHECK_EQUAL( l2.contains( Chan_Z ), false );
		
		BOOST_CHECK_EQUAL( Layout3::contains( Chan_Alpha ), false );
		BOOST_CHECK_EQUAL( Layout3::contains( Chan_V ), true );
		BOOST_CHECK_EQUAL( Layout3::contains( Chan_U ), true );
		BOOST_CHECK_EQUAL( Layout3::contains( Mask_UV ), true );
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
		
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Red>::ChannelType, float >::value ) );
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Green>::ChannelType, float >::value ) );
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Blue>::ChannelType, float >::value ) );

		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Red>::ChannelType, int >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Green>::ChannelType, int >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Blue>::ChannelType, int >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Alpha>::ChannelType, int >::value ) );
		
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_U>::ChannelType, short >::value ) );
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_V>::ChannelType, short >::value ) );
	};
	
	void testAccessors()
	{
		typedef BrothersLayout<float, Brothers_BGR> Layout1;
		typedef BrothersLayout<int, Brothers_RGBA> Layout2;
		typedef BrothersLayout<short, Brothers_VU> Layout3;
		
		Layout1::ChannelContainerType bgrContainer;
		Layout2::ChannelContainerType rgbaContainer;
		Layout2::ChannelContainerType vuContainer;
	
		std::cerr << "Implement the BrothersLayoutTest::testAccessors()" << std::endl;
		/*
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
		*/
	}

	void testCommonLayoutAttributes()
	{
		typedef BrothersLayout<float, Brothers_BGR> Layout1;
		typedef BrothersLayout<int, Brothers_RGBA> Layout2;
		typedef BrothersLayout<short, Brothers_VU> Layout3;
			
		BOOST_CHECK_EQUAL( Layout1::numberOfChannels(), 3 );
		BOOST_CHECK_EQUAL( Layout2::numberOfChannels(), 4 );
		BOOST_CHECK_EQUAL( Layout3::numberOfChannels(), 2 );

		BOOST_CHECK_EQUAL( Layout1::channels(), Mask_RGB );
		BOOST_CHECK_EQUAL( Layout2::channels(), Mask_RGBA );
		BOOST_CHECK_EQUAL( Layout3::channels(), Mask_UV );
	}
	
};

struct BrothersLayoutTestSuite : public boost::unit_test::test_suite
{
	BrothersLayoutTestSuite() : boost::unit_test::test_suite( "BrothersLayoutTestSuite" )
	{
		boost::shared_ptr<BrothersLayoutTest> instance( new BrothersLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testAccessors, instance ) );
		add( BOOST_CLASS_TEST_CASE( &BrothersLayoutTest::testCommonLayoutInterface, instance ) );
	}
};

void addBrothersLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new BrothersLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

