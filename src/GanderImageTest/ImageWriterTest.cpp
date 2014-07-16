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

#include "GanderImage/ImageWriter.h"
#include "GanderImage/ImageReader.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/CompoundLayout.h"

#include "GanderImageTest/ImageWriterTest.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"
#include "boost/filesystem.hpp"

using namespace Gander;
using namespace Gander::Image;
using namespace Gander::ImageTest;
using namespace boost;
using namespace boost::unit_test;

namespace Gander
{

namespace ImageTest
{

struct ImageWriterTest
{
	void testImageWriter()
	{

		typedef Gander::Image::CompoundLayout< ChannelLayout< float, Chan_Red >, ChannelLayout< float, Chan_Green >, ChannelLayout< float, Chan_Blue > > LayoutType;
		typedef Gander::Image::Image< LayoutType > ImageType;

		BOOST_CHECK( !boost::filesystem::exists( "test/GanderImageTest/testImage.exr" ) );
		
		// Create 2x2 image of RGB values.
		float r[2][2] = { { 1., 0. }, { .0, 1. } };
		float g[2][2] = { { 0., 1. }, { 0., 1. } };
		float b[2][2] = { { 0., 0. }, { 1., 1. } };

		ImageType image( 2, 2 );
		image.setDisplayWindow( Gander::Box2i( Eigen::Vector2i( -1, -1 ), Eigen::Vector2i( 1, 1 ) ) );
		image.setDataWindow( Gander::Box2i( Eigen::Vector2i( 1, 1 ), Eigen::Vector2i( 2, 2 ) ) );
		image.setChannelPointer( Chan_Red, &r[0][0], 2 );
		image.setChannelPointer( Chan_Green, &g[0][0], 2 );
		image.setChannelPointer( Chan_Blue, &b[0][0], 2 );
		BOOST_CHECK( image.isValid() );

		writeImage< ImageType >( image, "test/GanderImageTest/testImage.exr" );
		BOOST_CHECK( boost::filesystem::exists( "test/GanderImageTest/testImage.exr" ) );
	
	//	ImageType image2;
	//	Gander::Image::readImage< ImageType >( image2, "test/GanderImageTest/testImage.exr" );
	//	BOOST_CHECK( image2.getDataWindow() == Gander::Box2i( Eigen::Vector2i( 1, 1 ), Eigen::Vector2i( 2, 2 ) ) );
	//	BOOST_CHECK( image2.getDisplayWindow() == Gander::Box2i( Eigen::Vector2i( -1, -1 ), Eigen::Vector2i( 1, 1 ) ) );

	//	BOOST_CHECK( image == image2 );
	}
	
	~ImageWriterTest()
	{
		// Tidy up!
		if( boost::filesystem::exists( "test/GanderImageTest/testImage.exr" ) )
		{
			boost::filesystem::remove( "test/GanderImageTest/testImage.exr" );
		}
	}
};

struct ImageWriterTestSuite : public boost::unit_test::test_suite
{
	ImageWriterTestSuite() : boost::unit_test::test_suite( "ImageWriterTestSuite" )
	{
		boost::shared_ptr<ImageWriterTest> instance( new ImageWriterTest() );
		add( BOOST_CLASS_TEST_CASE( &ImageWriterTest::testImageWriter, instance ) );
	}
};

void addImageWriterTest( boost::unit_test::test_suite *test )
{
	test->add( new ImageWriterTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

