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
#include "GanderImage/CompoundLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/LayoutContainer.h"
#include "GanderImage/CompoundLayoutContainer.h"

#include "GanderImageTest/CompoundLayoutContainerTest.h"

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

struct CompoundLayoutContainerTest
{
	void testDynamicCompoundLayoutContainer()
	{
		Gander::Image::Detail::CompoundLayoutContainer< DynamicLayout< float > > c;
		
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
	
	void testCompoundLayoutContainer()
	{
		typedef Gander::Image::CompoundLayout< BrothersLayout< float, Brothers_BGRA >, ChannelLayout< int, Chan_Z >, ChannelLayout< int, Chan_V > > L1;
		Gander::Image::Detail::CompoundLayoutContainer< L1 > cc;

		cc.child<0>().channel< Chan_Red >() = 1.;
		cc.child<0>().channel< Chan_Green >() = 2.;
		cc.child<0>().channel< Chan_Blue >() = 3.;
		cc.child<0>().channel< Chan_Alpha >() = 4.;
		cc.child<1>().channel< Chan_Z >() = 5;
		cc.child<2>().channel< Chan_V >() = 6;
		
		BOOST_CHECK_EQUAL( ( cc.child<0>().channel< Chan_Red >() ), 1. );
		BOOST_CHECK_EQUAL( ( cc.child<0>().channel< Chan_Green >() ), 2. );
		BOOST_CHECK_EQUAL( ( cc.child<0>().channel< Chan_Blue >() ), 3. );
		BOOST_CHECK_EQUAL( ( cc.child<0>().channel< Chan_Alpha >() ), 4. );
		BOOST_CHECK_EQUAL( ( cc.child<1>().channel< Chan_Z >() ), 5 );
		BOOST_CHECK_EQUAL( ( cc.child<2>().channel< Chan_V >() ), 6 );
		
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

struct CompoundLayoutContainerTestSuite : public boost::unit_test::test_suite
{
	CompoundLayoutContainerTestSuite() : boost::unit_test::test_suite( "CompoundLayoutContainerTestSuite" )
	{
		boost::shared_ptr<CompoundLayoutContainerTest> instance( new CompoundLayoutContainerTest() );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutContainerTest::testCompoundLayoutContainer, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutContainerTest::testDynamicCompoundLayoutContainer, instance ) );
	}
};

void addCompoundLayoutContainerTest( boost::unit_test::test_suite *test )
{
	test->add( new CompoundLayoutContainerTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

