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

#include "GanderTest/TestTools.h"

#include "GanderImage/ImageReader.h"
#include "GanderImage/ImageWriter.h"
#include "GanderImage/Row.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/CompoundLayout.h"

#include "GanderImageTest/ImageReaderTest.h"

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

struct ImageReaderTest
{
	void testSimpleRead()
	{
		typedef Gander::Image::CompoundLayout< ChannelLayout< float, Chan_Red >, ChannelLayout< float, Chan_Green >, ChannelLayout< float, Chan_Blue > > LayoutType;
		typedef Gander::Image::Image< LayoutType > ImageType;
		ImageType image;

		readImage< ImageType >( image, "data/images/blueWithDataWindow.100x100.exr" );
		
		BOOST_CHECK( image.isValid() );
		BOOST_CHECK( image.getDisplayWindow() == Gander::Box2i( Eigen::Vector2i( -1, -1 ), Eigen::Vector2i( 1, 1 ) ) );
		BOOST_CHECK( image.getDataWindow() == Gander::Box2i( Eigen::Vector2i( 1, 1 ), Eigen::Vector2i( 2, 2 ) ) );
			
		BOOST_CHECK_EQUAL( image[1][1].channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( image[1][2].channel<Chan_Red>(), 0. );
		BOOST_CHECK_EQUAL( image[2][1].channel<Chan_Red>(), 0. );
		BOOST_CHECK_EQUAL( image[2][2].channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( image[1][1].channel<Chan_Green>(), .0 );
		BOOST_CHECK_EQUAL( image[1][2].channel<Chan_Green>(), 1. );
		BOOST_CHECK_EQUAL( image[2][1].channel<Chan_Green>(), 0. );
		BOOST_CHECK_EQUAL( image[2][2].channel<Chan_Green>(), 1. );
		BOOST_CHECK_EQUAL( image[1][1].channel<Chan_Blue>(), .0 );
		BOOST_CHECK_EQUAL( image[1][2].channel<Chan_Blue>(), .0 );
		BOOST_CHECK_EQUAL( image[2][1].channel<Chan_Blue>(), 1. );
		BOOST_CHECK_EQUAL( image[2][2].channel<Chan_Blue>(), 1. );

		std::cerr << "Things appear to work when using a bitmap... Just use the reader to try to read an EXR and just sample the raw data make sure it is as expected.." << std::endl;
		std::cerr << "Get the image reader working. It is almost there but it may be flipped etc." << std::endl;
		std::cerr << "Get the image writer working" << std::endl;
		std::cerr << "Get the image reader and writer working with different channel types such as half and int" << std::endl;
		std::cerr << "Write a thorough set of tests" << std::endl;
	}

	void testImageReader()
	{
		typedef Gander::Image::CompoundLayout< ChannelLayout< float, Chan_Red >, ChannelLayout< float, Chan_Green >, ChannelLayout< float, Chan_Blue >, ChannelLayout< float, Chan_Alpha > > LayoutType;
		typedef Gander::Image::Image< LayoutType > ImageType;
		ImageType image;
		readImage< ImageType >( image, "data/images/blueWithDataWindow.100x100.exr" );
		
		BOOST_CHECK( image.isValid() );
		BOOST_CHECK( image.getDataWindow() == Gander::Box2i( Eigen::Vector2i( 30, 30 ), Eigen::Vector2i( 79, 79 ) ) );
		BOOST_CHECK( image.getDisplayWindow() == Gander::Box2i( Eigen::Vector2i( 0, 0 ), Eigen::Vector2i( 99, 99 ) ) );

		// The image is just a blue window so we can just check that every pixel is blue...
		ImageType::Pixel expectedResult;
		expectedResult.channel<Chan_Red>() = .0;
		expectedResult.channel<Chan_Green>() = .0;
		expectedResult.channel<Chan_Blue>() = .5;
		expectedResult.channel<Chan_Alpha>() = .5;
		for( int i = image.getDataWindow().min(1); i <= image.getDataWindow().max(1); ++i )
		{
			for( int j = image.getDataWindow().min(0); j <= image.getDataWindow().max(0); ++j )
			{
				BOOST_CHECK( image[i][j] == expectedResult );
			}
		}

		writeImage< ImageType >( image, "test/GanderImageTest/testImage2.exr" );
		
		ImageType image2;
		readImage< ImageType >( image2, "test/GanderImageTest/testImage2.exr" );

		BOOST_CHECK( image2 == image );
	}
};

struct ImageReaderTestSuite : public boost::unit_test::test_suite
{
	ImageReaderTestSuite() : boost::unit_test::test_suite( "ImageReaderTestSuite" )
	{
		boost::shared_ptr<ImageReaderTest> instance( new ImageReaderTest() );
		add( BOOST_CLASS_TEST_CASE( &ImageReaderTest::testImageReader, instance ) );
//		add( BOOST_CLASS_TEST_CASE( &ImageReaderTest::testSimpleRead, instance ) );
	}
};

void addImageReaderTest( boost::unit_test::test_suite *test )
{
	test->add( new ImageReaderTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

