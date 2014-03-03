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

#include "GanderImage/Pixel.h"
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImage/CompoundLayout.h"
#include "GanderImageTest/PixelTest.h"

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

struct PixelTest
{
	void testPixelWithDynamicCompoundLayout()
	{
		typedef BrothersLayout< float, Brothers_BGRA > Layout1;
		typedef ChannelLayout< float, Chan_Z > Layout2;
		typedef DynamicLayout< int > Layout3;
		typedef CompoundLayout< Layout1, Layout2, Layout3 > CompoundLayout;
		
		typedef Gander::Image::Pixel< CompoundLayout > Pixel;
		typedef Gander::Image::PixelAccessor< CompoundLayout > PixelAccessor;
		
		Pixel pixel;
		pixel.channel<Chan_Red>() = 1.;
		pixel.channel<Chan_Green>() = 2.;
		pixel.channel<Chan_Blue>() = 3.;
		pixel.channel<Chan_Alpha>() = 4.;
		pixel.channel<Chan_Z>() = 5.;
		BOOST_CHECK_EQUAL( pixel.channels(), ChannelSet( Mask_RGBA | Mask_Z ) );

		pixel.addChannels( Mask_UV );
		BOOST_CHECK_EQUAL( pixel.channels(), ChannelSet( Mask_RGBA | Mask_Z | Mask_UV ) );
	}

	void testPixelWithCompoundLayout()
	{
		typedef BrothersLayout< float, Brothers_BGRA > Layout1;
		typedef ChannelLayout< float, Chan_Z > Layout2;
		typedef BrothersLayout< int, Brothers_VU > Layout3;
		typedef CompoundLayout< Layout1, Layout2, Layout3 > CompoundLayout;

		typedef Gander::Image::Pixel< CompoundLayout > Pixel;
		typedef Gander::Image::PixelAccessor< CompoundLayout > PixelAccessor;
		
		Pixel pixel;
		pixel.channel<Chan_Red>() = 1.;
		pixel.channel<Chan_Green>() = 2.;
		pixel.channel<Chan_Blue>() = 3.;
		pixel.channel<Chan_Alpha>() = 4.;
		pixel.channel<Chan_Z>() = 5.;
		pixel.channel<Chan_U>() = 6;
		pixel.channel<Chan_V>() = 7;
		
		Pixel pixel2( pixel );

		BOOST_CHECK_EQUAL( pixel.channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_Alpha>(), 4. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_Z>(), 5. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_U>(), 6. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_V>(), 7. );

		BOOST_CHECK_EQUAL( pixel.channelAtIndex<0>(), 1. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<1>(), 2. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<2>(), 3. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<3>(), 4. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<4>(), 5. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<5>(), 6. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<6>(), 7. );
	
		float bgra[4] = { 3., 2., 1., 4. };	
		float z = 5.;
		int vu[2] = { 7, 6 };
		
		PixelAccessor pixelAccessor;
		BOOST_CHECK_EQUAL( pixelAccessor.channels(), ChannelSet( Mask_RGBA | Mask_Z | Mask_UV ) );
		BOOST_CHECK_EQUAL( pixelAccessor.requiredChannels(), ChannelSet( Mask_Blue | Mask_V | Mask_Z ) );
		BOOST_CHECK_THROW( pixelAccessor.setChannelPointer( Chan_Red, &bgra[0] ), std::runtime_error );
		BOOST_CHECK_THROW( pixelAccessor.setChannelPointer( Chan_Blue, 0 ), std::runtime_error );
		
		pixelAccessor.setChannelPointer( Chan_Blue, &bgra[0] );
		pixelAccessor.setChannelPointer( Chan_Z, &z );
		pixelAccessor.setChannelPointer( Chan_V, &vu[0] );

		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Alpha>(), 4. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Z>(), 5. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_U>(), 6. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_V>(), 7. );

		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<0>(), 1. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<1>(), 2. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<2>(), 3. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<3>(), 4. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<4>(), 5. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<5>(), 6. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<6>(), 7. );
	
		// These the comparison and assignment operators.
		BOOST_CHECK( pixel == pixel2 );
		BOOST_CHECK( pixelAccessor == pixel2 );
		pixel2.channel<Chan_Z>() = 20.;
		BOOST_CHECK( pixel != pixel2 );
		BOOST_CHECK( pixelAccessor != pixel2 );
		BOOST_CHECK( pixelAccessor == pixel );
		pixelAccessor = pixel2;
		BOOST_CHECK( pixelAccessor == pixel2 );
		BOOST_CHECK( pixelAccessor.channel<Chan_Z>() == 20. );
		BOOST_CHECK( z == 20. );
	}

	void testPixelInterface()
	{
		typedef BrothersLayout< float, Brothers_BGRA > Layout;
		typedef Gander::Image::Pixel< Layout > Pixel;
		typedef Gander::Image::PixelAccessor< Layout > PixelAccessor;
		
		Pixel pixel;
		pixel.channel<Chan_Red>() = 1.;
		pixel.channel<Chan_Green>() = 2.;
		pixel.channel<Chan_Blue>() = 3.;
		pixel.channel<Chan_Alpha>() = 4.;
	
		Pixel pixel2;
		pixel2.channel<Chan_Red>() = 1.;
		pixel2.channel<Chan_Green>() = 2.;
		pixel2.channel<Chan_Blue>() = 3.;
		pixel2.channel<Chan_Alpha>() = 4.;

		// Test the common interface to all pixels.
		BOOST_CHECK_EQUAL( pixel.channels(), ChannelSet( Mask_RGBA ) );
		BOOST_CHECK_EQUAL( pixel.numberOfChannels(), ChannelSet( Mask_RGBA ).size() );
		BOOST_CHECK_EQUAL( pixel.isDynamic(), false );
	
		// Test that we can access the LayoutType using the Traits structs.	
		BOOST_CHECK( ( std::is_same< Pixel::LayoutTraits<0>::LayoutType, Layout >::value ) );
		BOOST_CHECK( ( std::is_same< Pixel::ChannelTraits< Chan_Red >::LayoutType, Layout >::value ) );
		BOOST_CHECK( ( std::is_same< Pixel::ChannelTraits< Chan_Green >::LayoutType, Layout >::value ) );
		BOOST_CHECK( ( std::is_same< Pixel::ChannelTraits< Chan_Blue >::LayoutType, Layout >::value ) );
		BOOST_CHECK( ( std::is_same< Pixel::ChannelTraits< Chan_Alpha >::LayoutType, Layout >::value ) );

		BOOST_CHECK_EQUAL( pixel.channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( pixel.channel<Chan_Alpha>(), 4. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<0>(), 1. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<1>(), 2. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<2>(), 3. );
		BOOST_CHECK_EQUAL( pixel.channelAtIndex<3>(), 4. );
		
		// Create a PixelAccessor with the same values as the pixel.
		float bgra[4] = { 3., 2., 1., 4. };
		PixelAccessor pixelAccessor;
		BOOST_CHECK_EQUAL( pixelAccessor.channels(), ChannelSet( Mask_RGBA ) );
		BOOST_CHECK_EQUAL( pixelAccessor.requiredChannels(), ChannelSet( Mask_Blue ) );
		BOOST_CHECK_THROW( pixelAccessor.setChannelPointer( Chan_Red, &bgra[0] ), std::runtime_error );
		BOOST_CHECK_THROW( pixelAccessor.setChannelPointer( Chan_Blue, 0 ), std::runtime_error );
		
		pixelAccessor.setChannelPointer( Chan_Blue, &bgra[0] );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( pixelAccessor.channel<Chan_Alpha>(), 4. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<0>(), 1. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<1>(), 2. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<2>(), 3. );
		BOOST_CHECK_EQUAL( pixelAccessor.channelAtIndex<3>(), 4. );
		
		// Test the equality operator.
		BOOST_CHECK( pixel == pixel2 );
		pixel2.channel<Chan_Green>() = 7.;
		BOOST_CHECK( pixel != pixel2 );
		BOOST_CHECK( pixel == pixelAccessor );
		BOOST_CHECK( pixel2 != pixelAccessor );

		// Test the assignment operator of both the Pixel class and the PixelAccessor class.
		pixel = pixel2;
		BOOST_CHECK( pixel == pixel2 );
		BOOST_CHECK( pixel != pixelAccessor );

		pixelAccessor = pixel;
		BOOST_CHECK( pixelAccessor == pixel2 );
		BOOST_CHECK( pixelAccessor == pixel );
		BOOST_CHECK( pixelAccessor.channel<Chan_Green>() == 7. );
		BOOST_CHECK( bgra[1] == 7. );
	}
};

struct PixelTestSuite : public boost::unit_test::test_suite
{
	PixelTestSuite() : boost::unit_test::test_suite( "PixelTestSuite" )
	{
		boost::shared_ptr<PixelTest> instance( new PixelTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testPixelInterface, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testPixelWithCompoundLayout, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testPixelWithDynamicCompoundLayout, instance ) );
	}
};

void addPixelTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

