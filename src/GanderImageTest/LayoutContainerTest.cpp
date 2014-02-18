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

#include "GanderImage/CompoundLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/LayoutContainer.h"
#include "GanderImage/CompoundLayoutContainer.h"

#include "GanderImageTest/LayoutContainerTest.h"

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

struct LayoutContainerTest
{
	void testLayoutContainer()
	{
		Gander::Image::Detail::LayoutContainer< BrothersLayout< float, Brothers_BGRA > > c;

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
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float >( 0 ) ), 3. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float >( 1 ) ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float >( 2 ) ), 1. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float >( 3 ) ), 4. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Red >() ), 1. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Blue >() ), 3. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Green >() ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, Mask_Alpha >() ), 4. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float, Mask_Red >( 0 ) ), 1. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float, Mask_Blue >( 0 ) ), 3. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float, Mask_Green >( 0 ) ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< float, Mask_Alpha >( 0 ) ), 4. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >() ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Green >::Value ) >() ), 4. );
	}
	
	void testDynamicLayoutContainer()
	{
		DynamicLayout< float > dynamicLayout;
		Gander::Image::Detail::LayoutContainer< DynamicLayout< float > > c( dynamicLayout );
		c.addChannels( Mask_Green | Mask_U );
		
		c.channel< Chan_U >() = 1.;
		c.channel< Chan_Green >() = 2.;
		
		c.addChannels( Mask_Blue );
		BOOST_CHECK_EQUAL( ChannelSet( c.layout().channels() ), ChannelSet( Mask_Green | Mask_U | Mask_Blue ) );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 0 >() ), 2. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 1 >() ), 0. );
		BOOST_CHECK_EQUAL( float( c.channelAtIndex< 2 >() ), 1. );
		
		BOOST_CHECK_EQUAL( float( c.channel< Chan_Green >() ), 2. );
		BOOST_CHECK_EQUAL( float( c.channel< Chan_Blue >() ), 0. );
		BOOST_CHECK_EQUAL( float( c.channel< Chan_U >() ), 1. );
	}
};

struct LayoutContainerTestSuite : public boost::unit_test::test_suite
{
	LayoutContainerTestSuite() : boost::unit_test::test_suite( "LayoutContainerTestSuite" )
	{
		boost::shared_ptr<LayoutContainerTest> instance( new LayoutContainerTest() );
		add( BOOST_CLASS_TEST_CASE( &LayoutContainerTest::testLayoutContainer, instance ) );
		add( BOOST_CLASS_TEST_CASE( &LayoutContainerTest::testDynamicLayoutContainer, instance ) );
	}
};

void addLayoutContainerTest( boost::unit_test::test_suite *test )
{
	test->add( new LayoutContainerTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

