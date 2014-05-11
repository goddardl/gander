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

#include "GanderImage/Image.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

#include "GanderImageTest/ImageTest.h"

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

struct ImageTest
{
	void testImageConstructor()
	{
		// Create 2x2 image of RGB values.
		float rgb[2][2][3] = {
			{ { 0., 1., 2. }, { 3., 4., 5. } },
			{ { 6., 7., 8. }, { 9., 10., 11. } },
		};
			
		// Just check that the array has been initialized correctly.
		BOOST_CHECK_EQUAL( rgb[0][0][0], 0. );
		BOOST_CHECK_EQUAL( rgb[0][1][1], 4. );
		BOOST_CHECK_EQUAL( rgb[1][1][2], 11. );

		// Create an image with interleaved RGB channels.
		typedef BrothersLayout< float, Brothers_RGB > Layout;
		Gander::Image::Image< Layout > image( 2, 2 );
		
		// The image should not be valid until we attach the channel data.
		BOOST_CHECK( !image.isValid() );
		
		// Check the image attributes.
		BOOST_CHECK_EQUAL( image.channels(), Mask_RGB );
		BOOST_CHECK_EQUAL( image.width(), 2 );
		BOOST_CHECK_EQUAL( image.height(), 2 );
		BOOST_CHECK_EQUAL( image.requiredChannels(), Chan_Red ); // As the channels are interleaved, only the red one is needed.
		BOOST_CHECK_EQUAL( image.numberOfChannelPointers(), 1 );
		BOOST_CHECK_EQUAL( image.isDynamic(), false );

		image.setChannelPointer( Chan_Red, &rgb[0][0][0], 2 );
		BOOST_CHECK( image.isValid() );
	}
};

struct ImageTestSuite : public boost::unit_test::test_suite
{
	ImageTestSuite() : boost::unit_test::test_suite( "ImageTestSuite" )
	{
		boost::shared_ptr<ImageTest> instance( new ImageTest() );
		add( BOOST_CLASS_TEST_CASE( &ImageTest::testImageConstructor, instance ) );
	}
};

void addImageTest( boost::unit_test::test_suite *test )
{
	test->add( new ImageTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

