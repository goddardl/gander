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

#include "GanderImage/ChannelLayout.h"
#include "GanderImageTest/ChannelLayoutTest.h"

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

struct ChannelLayoutTest
{
	void testCommonLayoutInterface()
	{
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef ChannelLayout<int, Chan_Z> Layout2;
		typedef ChannelLayout<short, Chan_Red> Layout3;

		Layout1 l1;
		Layout2 l2;
		Layout3 l3;

		BOOST_CHECK( int( l1 == l1 ) );
		BOOST_CHECK( int( l2 == l2 ) );
		BOOST_CHECK( int( l3 == l3 ) );
		BOOST_CHECK_EQUAL( int( l3 == l1 ), 0 );
		BOOST_CHECK_EQUAL( int( l3 == l2 ), 0 );
		BOOST_CHECK_EQUAL( int( l1 == l2 ), 0 );
		
		BOOST_CHECK( int( l1 != l3 ) );
		BOOST_CHECK( int( l2 != l3 ) );
		BOOST_CHECK( int( l3 != l1 ) );

		BOOST_CHECK_EQUAL( l1.channels(), ChannelSet( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l2.channels(), ChannelSet( Mask_Z ) );
		BOOST_CHECK_EQUAL( l3.channels(), ChannelSet( Mask_Red ) );
		
		BOOST_CHECK_EQUAL( l1.numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( l2.numberOfChannels(), 1 );
		BOOST_CHECK_EQUAL( l3.numberOfChannels(), 1 );
		
		BOOST_CHECK_EQUAL( l1.requiredChannels(), ChannelSet( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l2.requiredChannels(), ChannelSet( Mask_Z ) );
		BOOST_CHECK_EQUAL( l3.requiredChannels(), ChannelSet( Mask_Red ) );
	}

	void testChannelTraits()
	{
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef ChannelLayout<int, Chan_Z> Layout2;
		typedef ChannelLayout<short, Chan_Red> Layout3;

		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Alpha>::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Z>::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_Red>::LayoutType, Layout3 >::value ) );
		
		BOOST_CHECK( int( std::is_same< Layout1::ChannelTraits<Chan_Alpha>::StorageType, float >::value ) );
		BOOST_CHECK( int( std::is_same< Layout2::ChannelTraits<Chan_Z>::StorageType, int >::value ) );
		BOOST_CHECK( int( std::is_same< Layout3::ChannelTraits<Chan_Red>::StorageType, short >::value ) );
	}

	void testCommonLayoutAttributes()
	{
		typedef ChannelLayout<float, Chan_Alpha> Layout1;
		typedef ChannelLayout<float, Chan_Z> Layout2;
		typedef ChannelLayout<float, Chan_Red> Layout3;
			
		BOOST_CHECK_EQUAL( int(Layout1::NumberOfChannels), 1 );
		BOOST_CHECK_EQUAL( int(Layout2::NumberOfChannels), 1 );
		BOOST_CHECK_EQUAL( int(Layout3::NumberOfChannels), 1 );

		BOOST_CHECK_EQUAL( int(Layout1::ChannelMask), int( Mask_Alpha ) );
		BOOST_CHECK_EQUAL( int(Layout2::ChannelMask), int( Mask_Z ) );
		BOOST_CHECK_EQUAL( int(Layout3::ChannelMask), int( Mask_Red ) );
	}
};

struct ChannelLayoutTestSuite : public boost::unit_test::test_suite
{
	ChannelLayoutTestSuite() : boost::unit_test::test_suite( "ChannelLayoutTestSuite" )
	{
		boost::shared_ptr<ChannelLayoutTest> instance( new ChannelLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelLayoutTest::testCommonLayoutInterface, instance ) );
	}
};

void addChannelLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new ChannelLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

