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

	void testManyDynamicPixels()
	{
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGRA >, ChannelLayout< float, Chan_Z >, BrothersLayout< float, Brothers_VU >, ChannelLayout< float, Chan_Backward > > CompoundLayout1;
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGRA >, ChannelLayout< float, Chan_Z >, BrothersLayout< float, Brothers_VU >, DynamicLayout< float > > CompoundLayout2;
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGRA >, ChannelLayout< float, Chan_Z >, ChannelLayout< float, Chan_U >, DynamicLayout< float > > CompoundLayout3;
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGRA >, ChannelLayout< float, Chan_Z >, DynamicLayout< float > > CompoundLayout4;
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGRA >, DynamicLayout< float > > CompoundLayout5;
		typedef CompoundLayout< BrothersLayout< float, Brothers_RGB >, DynamicLayout< float > > CompoundLayout6;

		Gander::Image::Pixel< CompoundLayout1 > pixel1;
		Detail::initPixel( pixel1 );
		
		Gander::Image::Pixel< CompoundLayout2 > pixel2;
		pixel2.addChannels( Mask_Backward );
		Detail::initPixel( pixel2 );

		Gander::Image::Pixel< CompoundLayout3 > pixel3;
		pixel3.addChannels( Mask_Backward | Mask_V );
		Detail::initPixel( pixel3 );
		
		Gander::Image::Pixel< CompoundLayout4 > pixel4;
		pixel4.addChannels( Mask_Backward | Mask_UV );
		Detail::initPixel( pixel4 );

		Gander::Image::Pixel< CompoundLayout5 > pixel5;
		pixel5.addChannels( Mask_Backward | Mask_UV | Mask_Z );
		Detail::initPixel( pixel5 );
		
		Gander::Image::Pixel< CompoundLayout6 > pixel6;
		pixel6.addChannels( Mask_Backward | Mask_UV | Mask_Z | Mask_Alpha );
		Detail::initPixel( pixel6 );
	
		float bgra[4] = { 3., 2., 1., 4. };	
		float z = 5.;
		float vu[2] = { 7., 6. };	
		float backward = 8.;
		float rgb[3] = { 1., 2., 3. };	
		Gander::Image::PixelAccessor< CompoundLayout1 > pixelAccessor1;
		pixelAccessor1.setChannelPointer( Chan_Blue, &bgra[0] );
		pixelAccessor1.setChannelPointer( Chan_Z, &z );
		pixelAccessor1.setChannelPointer( Chan_V, &vu[0] );
		pixelAccessor1.setChannelPointer( Chan_Backward, &backward );
		
		Gander::Image::PixelAccessor< CompoundLayout2 > pixelAccessor2;
		pixelAccessor2.addChannels( Mask_Backward );
		pixelAccessor2.setChannelPointer( Chan_Blue, &bgra[0] );
		pixelAccessor2.setChannelPointer( Chan_Z, &z );
		pixelAccessor2.setChannelPointer( Chan_V, &vu[0] );
		pixelAccessor2.setChannelPointer( Chan_Backward, &backward );

		Gander::Image::PixelAccessor< CompoundLayout3 > pixelAccessor3;
		pixelAccessor3.addChannels( Mask_Backward | Mask_V );
		pixelAccessor3.setChannelPointer( Chan_Blue, &bgra[0] );
		pixelAccessor3.setChannelPointer( Chan_Z, &z );
		pixelAccessor3.setChannelPointer( Chan_U, &vu[1] );
		pixelAccessor3.setChannelPointer( Chan_V, &vu[0] );
		pixelAccessor3.setChannelPointer( Chan_Backward, &backward );
		
		Gander::Image::PixelAccessor< CompoundLayout4 > pixelAccessor4;
		pixelAccessor4.addChannels( Mask_Backward | Mask_UV );
		pixelAccessor4.setChannelPointer( Chan_Blue, &bgra[0] );
		pixelAccessor4.setChannelPointer( Chan_Z, &z );
		pixelAccessor4.setChannelPointer( Chan_U, &vu[1] );
		pixelAccessor4.setChannelPointer( Chan_V, &vu[0] );
		pixelAccessor4.setChannelPointer( Chan_Backward, &backward );

		Gander::Image::PixelAccessor< CompoundLayout5 > pixelAccessor5;
		pixelAccessor5.addChannels( Mask_Backward | Mask_UV | Mask_Z );
		pixelAccessor5.setChannelPointer( Chan_Blue, &bgra[0] );
		pixelAccessor5.setChannelPointer( Chan_Z, &z );
		pixelAccessor5.setChannelPointer( Chan_U, &vu[1] );
		pixelAccessor5.setChannelPointer( Chan_V, &vu[0] );
		pixelAccessor5.setChannelPointer( Chan_Backward, &backward );
		
		Gander::Image::PixelAccessor< CompoundLayout6 > pixelAccessor6;
		pixelAccessor6.addChannels( Mask_Backward | Mask_UV | Mask_Z | Mask_Alpha );
		pixelAccessor6.setChannelPointer( Chan_Red, &rgb[0] );
		pixelAccessor6.setChannelPointer( Chan_Alpha, &bgra[3] );
		pixelAccessor6.setChannelPointer( Chan_Z, &z );
		pixelAccessor6.setChannelPointer( Chan_U, &vu[1] );
		pixelAccessor6.setChannelPointer( Chan_V, &vu[0] );
		pixelAccessor6.setChannelPointer( Chan_Backward, &backward );
		
		BOOST_CHECK( pixel1 == pixel2 );
		BOOST_CHECK( pixel1 == pixel3 );
		BOOST_CHECK( pixel1 == pixel4 );
		BOOST_CHECK( pixel1 == pixel5 );
		BOOST_CHECK( pixel1 == pixel6 );
		BOOST_CHECK( pixel2 == pixel3 );
		BOOST_CHECK( pixel2 == pixel4 );
		BOOST_CHECK( pixel2 == pixel5 );
		BOOST_CHECK( pixel2 == pixel6 );
		BOOST_CHECK( pixel3 == pixel4 );
		BOOST_CHECK( pixel3 == pixel5 );
		BOOST_CHECK( pixel3 == pixel6 );
		BOOST_CHECK( pixel4 == pixel5 );
		BOOST_CHECK( pixel4 == pixel6 );
		BOOST_CHECK( pixel5 == pixel6 );
		
		BOOST_CHECK( pixelAccessor1 == pixelAccessor2 );
		BOOST_CHECK( pixelAccessor1 == pixelAccessor3 );
		BOOST_CHECK( pixelAccessor1 == pixelAccessor4 );
		BOOST_CHECK( pixelAccessor1 == pixelAccessor5 );
		BOOST_CHECK( pixelAccessor1 == pixelAccessor6 );
		BOOST_CHECK( pixelAccessor2 == pixelAccessor3 );
		BOOST_CHECK( pixelAccessor2 == pixelAccessor4 );
		BOOST_CHECK( pixelAccessor2 == pixelAccessor5 );
		BOOST_CHECK( pixelAccessor2 == pixelAccessor6 );
		BOOST_CHECK( pixelAccessor3 == pixelAccessor4 );
		BOOST_CHECK( pixelAccessor3 == pixelAccessor5 );
		BOOST_CHECK( pixelAccessor3 == pixelAccessor6 );
		BOOST_CHECK( pixelAccessor4 == pixelAccessor5 );
		BOOST_CHECK( pixelAccessor4 == pixelAccessor6 );
		BOOST_CHECK( pixelAccessor5 == pixelAccessor6 );
		
		BOOST_CHECK( pixel1 == pixelAccessor2 );
		BOOST_CHECK( pixel1 == pixelAccessor3 );
		BOOST_CHECK( pixel1 == pixelAccessor4 );
		BOOST_CHECK( pixel1 == pixelAccessor5 );
		BOOST_CHECK( pixel1 == pixelAccessor6 );
		BOOST_CHECK( pixel2 == pixelAccessor3 );
		BOOST_CHECK( pixel2 == pixelAccessor4 );
		BOOST_CHECK( pixel2 == pixelAccessor5 );
		BOOST_CHECK( pixel2 == pixelAccessor6 );
		BOOST_CHECK( pixel3 == pixelAccessor4 );
		BOOST_CHECK( pixel3 == pixelAccessor5 );
		BOOST_CHECK( pixel3 == pixelAccessor6 );
		BOOST_CHECK( pixel4 == pixelAccessor5 );
		BOOST_CHECK( pixel4 == pixelAccessor6 );
		BOOST_CHECK( pixel5 == pixelAccessor6 );

		pixel1.channel< Chan_U >() = 10.;
		BOOST_CHECK( pixel1 == pixel1 );
		BOOST_CHECK( pixel1 != pixel2 );
		BOOST_CHECK( pixel1 != pixelAccessor1 );
		BOOST_CHECK( pixel1 != pixelAccessor2 );
		BOOST_CHECK( pixel1 != pixelAccessor6 );
		pixelAccessor6 = pixel1;
		BOOST_CHECK( pixel1 == pixelAccessor6 );
		
		BOOST_CHECK_THROW( pixelAccessor5.addChannels( Chan_Blue ), std::runtime_error );
	}

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

		Pixel pixel2( pixel );
		BOOST_CHECK( pixel == pixel2 );
		
		float bgra[4] = { 3., 2., 1., 4. };	
		float z = 5.;
		PixelAccessor pixelAccessor;
		BOOST_CHECK_EQUAL( pixelAccessor.channels(), ChannelSet( Mask_RGBA | Mask_Z ) );
		BOOST_CHECK_EQUAL( pixelAccessor.requiredChannels(), ChannelSet( Mask_Blue | Mask_Z ) );
		
		BOOST_CHECK_THROW( pixelAccessor.setChannelPointer( Chan_Red, &bgra[0] ), std::runtime_error );
		BOOST_CHECK_THROW( pixelAccessor.setChannelPointer( Chan_Blue, 0 ), std::runtime_error );
		pixelAccessor.setChannelPointer( Chan_Blue, &bgra[0] );
		pixelAccessor.setChannelPointer( Chan_Z, &z );
		
		BOOST_CHECK( pixel == pixelAccessor );
	
		pixel.addChannels( Mask_UV );
		pixel.channel<Chan_U>() = 6;
		pixel.channel<Chan_V>() = 7;
		
		BOOST_CHECK_EQUAL( pixel.channels(), ChannelSet( Mask_RGBA | Mask_Z | Mask_UV ) );
		BOOST_CHECK( pixel != pixel2 );
		BOOST_CHECK( pixel != pixelAccessor );

		Pixel pixel3( pixel2 );
		BOOST_CHECK( pixel3 == pixel2 );
		
		int uv[2] = { 6, 7 };
		pixelAccessor.addChannels( Mask_UV );
		pixelAccessor.setChannelPointer( Chan_U, &uv[0] );
		pixelAccessor.setChannelPointer( Chan_V, &uv[1] );

		BOOST_CHECK_EQUAL( pixel.channels(), ChannelSet( Mask_RGBA | Mask_Z | Mask_UV ) );
		BOOST_CHECK_EQUAL( pixelAccessor.requiredChannels(), ChannelSet( Mask_Blue | Mask_Z | Mask_UV ) );
		BOOST_CHECK( pixel == pixelAccessor );
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
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testManyDynamicPixels, instance ) );
	}
};

void addPixelTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

