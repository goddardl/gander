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
		BOOST_CHECK( pixel == pixel2 );

		std::cerr << forEachChannel( pixel, pixel2, AreEqual(), AndAccumulate<AreEqual>() ) << std::endl;

		todo:

		Make the struct that loops over the channels work for dynamic layouts too.
		Sort out the op structure. Perhaps the base class can be the iterator and the Channel ops derive from it?
		Add the pixel comparison to the pixel class using a AreEqual op.
		Add a runtime channel() method to the compoundLayout and make the return type a template parameter which is set to the first layouts ReferenceType by default.

	}
};

struct PixelTestSuite : public boost::unit_test::test_suite
{
	PixelTestSuite() : boost::unit_test::test_suite( "PixelTestSuite" )
	{
		boost::shared_ptr<PixelTest> instance( new PixelTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testPixelInterface, instance ) );
	}
};

void addPixelTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

