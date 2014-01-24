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

#include "GanderImage/PixelBase.h"
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImageTest/PixelBaseTest.h"

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

struct PixelBaseTest
{
	void testRequiredChannels()
	{
		typedef ChannelLayout< float, Chan_Red > Layout1;
		typedef BrothersLayout< float, Brothers_UV > Layout2;
		typedef DynamicLayout< float > Layout3;
		typedef PixelLayout< Layout1, Layout2, Layout3 > Layout4;
		
		BOOST_CHECK_EQUAL( PixelBase< Layout1 >().requiredChannels(), ChannelSet( Layout1().requiredChannels()) );
		BOOST_CHECK_EQUAL( PixelBase< Layout2 >().requiredChannels(), ChannelSet( Layout2().requiredChannels()) );
		BOOST_CHECK_EQUAL( PixelBase< Layout3 >( Layout3() ).requiredChannels(), ChannelSet( Layout3().requiredChannels()) );
		BOOST_CHECK_EQUAL( PixelBase< Layout4 >( Layout4() ).requiredChannels(), ChannelSet( Layout4().requiredChannels()) );
	}

	void testDifferentLayoutTraits()
	{
		typedef ChannelLayout< float, Chan_Red > Layout1;
		typedef PixelBase< Layout1 > Base1;
		BOOST_CHECK_EQUAL( int( Layout1::NumberOfLayouts ), 1 );
		BOOST_CHECK( int( std::is_same< Base1::ChannelTraits< Chan_Red >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Base1::LayoutTraits< 0 >::LayoutType, Layout1 >::value ) );

		typedef BrothersLayout< float, Brothers_UV > Layout2;
		typedef PixelBase< Layout2 > Base2;
		BOOST_CHECK_EQUAL( int( Layout2::NumberOfLayouts ), 1 );
		BOOST_CHECK( int( std::is_same< Base2::ChannelTraits< Chan_U >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base2::LayoutTraits< 0 >::LayoutType, Layout2 >::value ) );

		typedef DynamicLayout< float > Layout3;
		typedef PixelBase< Layout3 > Base3;
		BOOST_CHECK_EQUAL( int( Layout3::NumberOfLayouts ), 1 );
		BOOST_CHECK( int( std::is_same< Base3::ChannelTraits< Chan_Z >::LayoutType, Layout3 >::value ) );
		BOOST_CHECK( int( std::is_same< Base3::LayoutTraits< 0 >::LayoutType, Layout3 >::value ) );
		
		typedef PixelLayout< Layout1, Layout2, Layout3 > Layout4;
		BOOST_CHECK_EQUAL( int( Layout4::NumberOfLayouts ), 3 );
		
		typedef PixelBase< Layout4 > Base4;
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_Red >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_U >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_V >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::ChannelTraits< Chan_Z >::LayoutType, Layout3 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::LayoutTraits< 0 >::LayoutType, Layout1 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::LayoutTraits< 1 >::LayoutType, Layout2 >::value ) );
		BOOST_CHECK( int( std::is_same< Base4::LayoutTraits< 2 >::LayoutType, Layout3 >::value ) );
	}
};

struct PixelBaseTestSuite : public boost::unit_test::test_suite
{
	PixelBaseTestSuite() : boost::unit_test::test_suite( "PixelBaseTestSuite" )
	{
		boost::shared_ptr<PixelBaseTest> instance( new PixelBaseTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelBaseTest::testDifferentLayoutTraits, instance ) );
	}
};

void addPixelBaseTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelBaseTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

