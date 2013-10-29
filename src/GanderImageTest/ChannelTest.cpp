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

#include "GanderImage/Channel.h"
#include "GanderImageTest/ChannelTest.h"

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

struct ChannelTest
{
	void testChannelSetConstructors()
	{
		ChannelSet alphaSet( Chan_Alpha );
		BOOST_CHECK( alphaSet.contains( Chan_Alpha ) );

		ChannelSet blueSet( Mask_Blue );
		BOOST_CHECK( blueSet.contains( Chan_Blue ) );

		ChannelSet copySet( alphaSet );
		BOOST_CHECK( copySet.contains( Chan_Alpha ) );

		ChannelSet rgb( Mask_RGB );
		BOOST_CHECK( rgb.contains( Chan_Red ) );
		BOOST_CHECK( rgb.contains( Chan_Green ) );
		BOOST_CHECK( rgb.contains( Chan_Blue ) );
		
		ChannelSet rgba( Mask_RGB | Mask_Alpha );
		BOOST_CHECK( rgba.contains( Chan_Red ) );
		BOOST_CHECK( rgba.contains( Chan_Green ) );
		BOOST_CHECK( rgba.contains( Chan_Blue ) );
		BOOST_CHECK( rgba.contains( Chan_Alpha ) );
	}

	void testChannelMasks()
	{
		try
		{
			{
				// Test the assignment constructor.
				Channel red( Chan_Red );
				BOOST_CHECK( red == Chan_Red );

				// Test the copy constructor.
				Channel blue( Chan_Blue );
				blue = red;
				BOOST_CHECK( blue == Chan_Red );

				// Test the assignement operator.
				red = Chan_Green;
				BOOST_CHECK( red == Chan_Green );

				//red = Mask_RGB; // This shouldn't be allowed! This is allowed because the flags class can accept uint32 as a construcotr..
//				std::cerr << red.value() << std::endl;
			}


		}
		catch ( std::exception &e ) 
		{
		}
	}
};

struct ChannelTestSuite : public boost::unit_test::test_suite
{
	ChannelTestSuite() : boost::unit_test::test_suite( "ChannelTestSuite" )
	{
		boost::shared_ptr<ChannelTest> instance( new ChannelTest() );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testChannelMasks, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelTest::testChannelSetConstructors, instance ) );
	}
};

void addChannelTest( boost::unit_test::test_suite *test )
{
	test->add( new ChannelTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

