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

#include "GanderImage/Layout.h"
#include "GanderImage/PixelLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"

#include "GanderImageTest/LayoutTest.h"

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

struct LayoutTest
{
	void testChannelContainer()
	{
		Gander::Image::Detail::ChannelContainer< BrothersLayout< float, Brothers_BGRA > > c;

		BOOST_CHECK_EQUAL( c.layout().channels(), ChannelSet( Mask_RGBA ) );
		c.channel< Chan_Red >() = 1.;
		c.channel< Chan_Green >() = 2.;
		c.channel< Chan_Blue >() = 3.;
		c.channel< Chan_Alpha >() = 4.;
		
		BOOST_CHECK_EQUAL( c.channel< Chan_Red >(), 1. );
		BOOST_CHECK_EQUAL( c.channel< Chan_Green >(), 2. );
		BOOST_CHECK_EQUAL( c.channel< Chan_Blue >(), 3. );
		BOOST_CHECK_EQUAL( c.channel< Chan_Alpha >(), 4. );
		
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0 >() ), 3. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 1 >() ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 2 >() ), 1. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 3 >() ), 4. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Red >() ), 1. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Blue >() ), 3. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Green >() ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Alpha >() ), 4. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >() ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >() ), 4. );
	}
	
	void testCompoundChannelContainer()
	{
		typedef Gander::Image::PixelLayout< BrothersLayout< float, Brothers_BGRA >, ChannelLayout< int, Chan_Z >, ChannelLayout< int, Chan_V > > L1;
		Gander::Image::Detail::CompoundChannelContainer< L1 > cc;

		cc.container<0>().channel< Chan_Red >() = 1.;
		cc.container<0>().channel< Chan_Green >() = 2.;
		cc.container<0>().channel< Chan_Blue >() = 3.;
		cc.container<0>().channel< Chan_Alpha >() = 4.;
		cc.container<1>().channel< Chan_Z >() = 5;
		cc.container<2>().channel< Chan_V >() = 6;
		
		BOOST_CHECK_EQUAL( ( cc.container<0>().channel< Chan_Red >() ), 1. );
		BOOST_CHECK_EQUAL( ( cc.container<0>().channel< Chan_Green >() ), 2. );
		BOOST_CHECK_EQUAL( ( cc.container<0>().channel< Chan_Blue >() ), 3. );
		BOOST_CHECK_EQUAL( ( cc.container<0>().channel< Chan_Alpha >() ), 4. );
		BOOST_CHECK_EQUAL( ( cc.container<1>().channel< Chan_Z >() ), 5 );
		BOOST_CHECK_EQUAL( ( cc.container<2>().channel< Chan_V >() ), 6 );
		
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 0 >() ), 3. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 1 >() ), 2. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 2 >() ), 1. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 3 >() ), 4. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 4 >() ), 5 );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 5 >() ), 6 );
		
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 0, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >() ), 2. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >() ), 4. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< 2, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >() ), 6 );
		
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< float, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( 0 ) ), 2. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< float, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( 1 ) ), 4. );
		BOOST_CHECK_EQUAL( ( cc.channelAtIndex< int, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( 2 ) ), 6 );
		BOOST_CHECK_THROW( ( cc.channelAtIndex< float, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( 2 ) ), std::runtime_error );
		
		BOOST_CHECK_EQUAL( ( cc.channel< Chan_Green, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >() ), 2. );
		BOOST_CHECK_EQUAL( ( cc.channel< Chan_Alpha, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >() ), 4. );
		BOOST_CHECK_EQUAL( ( cc.channel< Chan_V, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >() ), 6 );
		
		BOOST_CHECK_EQUAL( ( cc.channel< float, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( Chan_Green ) ), 2. );
		BOOST_CHECK_EQUAL( ( cc.channel< float, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( Chan_Alpha ) ), 4. );
		BOOST_CHECK_EQUAL( ( cc.channel< int, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( Chan_V ) ), 6 );
		BOOST_CHECK_THROW( ( cc.channel< float, ChannelMask( CombineMasks< Mask_Green, Mask_V, Mask_Alpha >::Value ) >( Chan_V ) ), std::runtime_error );
	}
};

struct LayoutTestSuite : public boost::unit_test::test_suite
{
	LayoutTestSuite() : boost::unit_test::test_suite( "LayoutTestSuite" )
	{
		boost::shared_ptr<LayoutTest> instance( new LayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &LayoutTest::testChannelContainer, instance ) );
		add( BOOST_CLASS_TEST_CASE( &LayoutTest::testCompoundChannelContainer, instance ) );
	}
};

void addLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new LayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

