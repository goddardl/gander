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

#include "GanderImage/DynamicLayout.h"
#include "GanderImageTest/DynamicLayoutTest.h"

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

struct DynamicLayoutTest
{
	void testCommonLayoutAttributes()
	{
		typedef DynamicLayout<float> Layout;
			
		BOOST_CHECK_EQUAL( int(Layout::IsDynamic), 1 );

		BOOST_CHECK( int( std::is_same< Layout::LayoutTraits<0>::LayoutType, Layout >::value ) );
		BOOST_CHECK( int( std::is_same< Layout::LayoutTraits<0>::StorageType, float >::value ) );
		BOOST_CHECK( int( std::is_same< Layout::ChannelTraits<Chan_Red>::StorageType, float >::value ) );
	}

	void testAddExistingChannel()
	{
		typedef DynamicLayout< float > Layout;
		Layout l;
		l.addChannels( Mask_Green | Mask_Blue, Brothers_RGB );
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Green | Mask_Blue  ) );

		// We should not be able to add a channel that already exists and an exception 
		// will be raised. Test that it is.
		BOOST_CHECK_THROW( l.addChannels( Chan_Green, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_Red, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Mask_RGB, Brothers_RGB ), std::runtime_error );
	}

	void testChannels()
	{
		typedef DynamicLayout< float > Layout;
		Layout l;
		l.addChannels( Mask_Red | Mask_Blue );
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Red | Mask_Blue ) );
		l.addChannels( Mask_U, Brothers_UV );
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Red | Mask_Blue | Mask_U ) );
	}
	
	void testCommonLayoutInterface()
	{
		DynamicLayout< float > l;
		l.addChannels( Mask_Red | Mask_Green, Brothers_RGB );
		l.addChannels( Mask_Alpha );
		l.addChannels( Mask_UV, Brothers_VU );
		
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_Red | Mask_Green | Mask_UV | Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l.contains( ChannelSet( Mask_Red | Mask_Green | Mask_UV | Mask_Alpha ) ), true );
		BOOST_CHECK_EQUAL( l.contains( Chan_Z ), false );
		BOOST_CHECK_EQUAL( l.requiredChannels(), ChannelSet( Mask_Red | Mask_Green | Mask_UV | Mask_Alpha ) );
		BOOST_CHECK_EQUAL( l.numberOfChannels(), 5 );
		BOOST_CHECK_EQUAL( l.step( Chan_Red ), 3 );
		BOOST_CHECK_EQUAL( l.step( Chan_Green ), 3 );
		BOOST_CHECK_EQUAL( l.step( Chan_U ), 2 );
		BOOST_CHECK_EQUAL( l.step( Chan_V ), 2 );
		BOOST_CHECK_EQUAL( l.step( Chan_Alpha ), 1 );
		BOOST_CHECK_EQUAL( l.step<Chan_Red>(), 3 );
		BOOST_CHECK_EQUAL( l.step<Chan_Green>(), 3 );
		BOOST_CHECK_EQUAL( l.step<Chan_U>(), 2 );
		BOOST_CHECK_EQUAL( l.step<Chan_V>(), 2 );
		BOOST_CHECK_EQUAL( l.step<Chan_Alpha>(), 1 );
		BOOST_CHECK_THROW( l.step<Chan_Z>(), std::runtime_error );
		BOOST_CHECK_THROW( l.step( Chan_Z ), std::runtime_error );
	}

	void testAddChannelWhichIsNotABrother()
	{
		typedef DynamicLayout< float > Layout;
		Layout l;
		BOOST_CHECK_THROW( l.addChannels( Chan_U, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_V, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_Mask, Brothers_RGB ), std::runtime_error );
		BOOST_CHECK_THROW( l.addChannels( Chan_Red, Brothers_UV ), std::runtime_error );
	}

};

struct DynamicLayoutTestSuite : public boost::unit_test::test_suite
{
	DynamicLayoutTestSuite() : boost::unit_test::test_suite( "DynamicLayoutTestSuite" )
	{
		boost::shared_ptr<DynamicLayoutTest> instance( new DynamicLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testAddExistingChannel, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testChannels, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testCommonLayoutInterface, instance ) );
		add( BOOST_CLASS_TEST_CASE( &DynamicLayoutTest::testAddChannelWhichIsNotABrother, instance ) );
	}
};

void addDynamicLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new DynamicLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

