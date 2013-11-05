//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013, Luke Goddard. All rights reserved.
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
#include "GanderImage/Pixel.h"

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
	void testChannelOrder()
	{
		// This test checks that the internal channel pointers are kept in the same order
		// as the Channels in the ChannelSet.
		int8u i[3] = { 1, 2, 3 };
		Buffer desc;
		desc.data = &i[0];
		desc.width = 1;
		desc.height = 1;

		Gander::Image::Image image( 1, 1 );
		image.addChannel( desc, Chan_Blue );
		BOOST_CHECK_EQUAL( (*(image.channelPtr(0))), 1 );
		
		// Insert a red channel which, as it comes before the blue channel in the ChannelSet,
		// should now be located at index 0.
		desc.data = &i[1];
		image.addChannel( desc, Chan_Red );
		BOOST_CHECK_EQUAL( (*(image.channelPtr(0))), 2 );

		// Insert a green channel which, as it comes before the blue channel but after the red in the ChannelSet,
		// should now be located at index 1.
		desc.data = &i[2];
		image.addChannel( desc, Chan_Green );
		BOOST_CHECK_EQUAL( (*(image.channelPtr(1))), 3 );
	}
};

struct PixelTestSuite : public boost::unit_test::test_suite
{
	PixelTestSuite() : boost::unit_test::test_suite( "PixelTestSuite" )
	{
		boost::shared_ptr<PixelTest> instance( new PixelTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testChannelOrder, instance ) );
	}
};

void addPixelTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

