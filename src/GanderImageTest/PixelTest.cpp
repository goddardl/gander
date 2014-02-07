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

#include "GanderImage/Pixel.h"
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImageTest/PixelTest.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Image;
using namespace Gander::ImageTest;
using namespace boost;
using namespace boost::unit_test;

template< EnumType ID, ChannelDefault S > struct TestLayout
{
	typedef float StorageType;
	enum
	{
		IsDynamic = false,
		NumberOfChannels = 1,
		ChannelMask = ChannelToMask<S>::Value,
		Id = ID,
	};
		
	template< ChannelDefault >
	struct ChannelTraits
	{
		typedef TestLayout< ID, S > LayoutType;
		typedef typename LayoutType::StorageType StorageType;
	};
};

namespace Gander
{

namespace ImageTest
{

struct PixelTest
{
	void testChannels()
	{
		typedef ChannelLayout< float, Chan_Red > Layout1;
		typedef BrothersLayout< float, Brothers_UV > Layout2;
		typedef PixelLayout< Layout1, Layout2 > Layout3;
		typedef Pixel< Layout3 > Base;
		Base base;
		base.channel< float >( Chan_Red ) = 1.5;
	}

	void testChannelAtIndex()
	{
		/*
		 * 1) Test the channelAtIndex code with Pixels that use ChannelLayout and BrothersLayout the Layout.
		 * 2) Make the test cases more robust.
		 * 3) Add support for dynamic channels.
		 * 4) Make the Pixel take the Tuple type from the Layout. That way we can make the Layout dictate the type of container required and therefore add support for access to channel
		 *    values when there are more than one in an int. How would this be implemented?
		 * 5) Work out how we are going to access pointers etc. maybe a different method?
		 */
		typedef ChannelLayout< float, Chan_Red > Layout1;
		typedef BrothersLayout< float, Brothers_UV > Layout2;
		typedef PixelLayout< Layout1, Layout2 > Layout3;
		typedef Pixel< Layout3 > Base;
		Base base;
		base.channelAtIndex< float >( 0 ) = 1.5;
		base.channelAtIndex< float >( 1 ) = 2.5;
		base.channelAtIndex< float >( 2 ) = 3.5;
		std::cerr << base.channelAtIndex< float >( 0 ) << std::endl;	
		std::cerr << base.channelAtIndex< float >( 1 ) << std::endl;	
		std::cerr << base.channelAtIndex< float >( 2 ) << std::endl;	
	}
	
	void testRequiredChannels()
	{
		typedef ChannelLayout< float, Chan_Red > Layout1;
		typedef BrothersLayout< float, Brothers_UV > Layout2;
		typedef DynamicLayout< float > Layout3;
		typedef PixelLayout< Layout1, Layout2, Layout3 > Layout4;
		
		BOOST_CHECK_EQUAL( Pixel< Layout1 >().requiredChannels(), ChannelSet( Layout1().requiredChannels()) );
		BOOST_CHECK_EQUAL( Pixel< Layout2 >().requiredChannels(), ChannelSet( Layout2().requiredChannels()) );
		BOOST_CHECK_EQUAL( Pixel< Layout3 >( Layout3() ).requiredChannels(), ChannelSet( Layout3().requiredChannels()) );
		BOOST_CHECK_EQUAL( Pixel< Layout4 >( Layout4() ).requiredChannels(), ChannelSet( Layout4().requiredChannels()) );
	}

	void testRuntimeChannelTraits()
	{
		typedef ChannelLayout< float, Chan_Z > Layout1;
		typedef BrothersLayout< float, Brothers_UV > Layout2;
		typedef DynamicLayout< float > Layout3;
		typedef PixelLayout< Layout1, Layout2, Layout3 > Layout4;
		
		{
			typedef Pixel< Layout1 > Base;
			Base::ChannelTraits<> traits1 = Base().channelTraits( Chan_Z );
			BOOST_CHECK_EQUAL( traits1.step(), 1 );
		}

		{	
			typedef Pixel< Layout2 > Base;
			Base::ChannelTraits<> traits2 = Base().channelTraits( Chan_U );
			BOOST_CHECK_EQUAL( traits2.step(), 2 );
		}
			
		{	
			typedef Pixel< Layout3 > Base;
			Layout3 layout;
			layout.addChannels( Chan_Blue | Chan_Green, Brothers_RGBA );

			Base::ChannelTraits<> traits3 = Base( layout ).channelTraits( Chan_Blue );
			BOOST_CHECK_EQUAL( traits3.step(), 4 );

			Base::ChannelTraits<> traits4 = Base( layout ).channelTraits( Chan_Green );
			BOOST_CHECK_EQUAL( traits4.step(), 4 );
		}
		
		{
			typedef Pixel< Layout4 > Base;
			
			Layout4 l;
			l.addChannels( Chan_Blue | Chan_Green, Brothers_RGBA );
			Base b( l );

			Base::ChannelTraits<> traits = b.channelTraits( Chan_Z );
			BOOST_CHECK_EQUAL( traits.step(), 1 );
			
			traits = b.channelTraits( Chan_U );
			BOOST_CHECK_EQUAL( traits.step(), 2 );
			
			traits = b.channelTraits( Chan_V );
			BOOST_CHECK_EQUAL( traits.step(), 2 );
			
			traits = b.channelTraits( Chan_Blue );
			BOOST_CHECK_EQUAL( traits.step(), 4 );
			
			traits = b.channelTraits( Chan_Green );
			BOOST_CHECK_EQUAL( traits.step(), 4 );
		}
	}

	void testDifferentLayoutTraits()
	{
		typedef ChannelLayout< float, Chan_Red > Layout1;
		typedef Pixel< Layout1 > Base1;
		BOOST_CHECK_EQUAL( int( Layout1::NumberOfLayouts ), 1 );
		BOOST_CHECK( int( std::is_same< Base1::ChannelTraits< Chan_Red >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Base1::LayoutTraits< 0 >::LayoutType, Layout1 >::value ) );

		typedef BrothersLayout< float, Brothers_UV > Layout2;
		typedef Pixel< Layout2 > Base2;
		BOOST_CHECK_EQUAL( int( Layout2::NumberOfLayouts ), 1 );
		BOOST_CHECK( int( std::is_same< Base2::ChannelTraits< Chan_U >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base2::LayoutTraits< 0 >::LayoutType, Layout2 >::value ) );

		typedef DynamicLayout< float > Layout3;
		typedef Pixel< Layout3 > Base3;
		BOOST_CHECK_EQUAL( int( Layout3::NumberOfLayouts ), 1 );
		BOOST_CHECK( int( std::is_same< Base3::ChannelTraits< Chan_Z >::LayoutType, Layout3 >::value ) );
		BOOST_CHECK( int( std::is_same< Base3::LayoutTraits< 0 >::LayoutType, Layout3 >::value ) );
		
		typedef PixelLayout< Layout1, Layout2, Layout3 > Layout4;
		BOOST_CHECK_EQUAL( int( Layout4::NumberOfLayouts ), 3 );
		
		typedef Pixel< Layout4 > Base4;
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_Red >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_U >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_V >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_Z >::LayoutType, Layout3 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::LayoutTraits< 0 >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::LayoutTraits< 1 >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::LayoutTraits< 2 >::LayoutType, Layout3 >::value ) );
	}
};

struct PixelTestSuite : public boost::unit_test::test_suite
{
	PixelTestSuite() : boost::unit_test::test_suite( "PixelTestSuite" )
	{
		boost::shared_ptr<PixelTest> instance( new PixelTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testDifferentLayoutTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testRuntimeChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testRequiredChannels, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testChannelAtIndex, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelTest::testChannels, instance ) );
	}
};

void addPixelTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

