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

#include "GanderImage/PixelLayout.h"
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImageTest/PixelLayoutTest.h"

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

struct PixelLayoutTest
{
	void testLayoutTraits()
	{
		/// Create 4 test storage types that we can use to assert that access to the StorageTypes works correctly.
		typedef TestLayout<0, Chan_Red> Storage0;
		typedef TestLayout<1, Chan_Blue> Storage1;
		typedef TestLayout<2, Chan_Z> Storage2;
		typedef TestLayout<3, Chan_U> Storage3;

		typedef PixelLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
		BOOST_CHECK_EQUAL( int(Layout::NumberOfLayouts), 4 );
		BOOST_CHECK_EQUAL( int(Layout::LayoutTraits<0>::LayoutType::Id), 0 );
		BOOST_CHECK_EQUAL( int(Layout::LayoutTraits<1>::LayoutType::Id), 1 );
		BOOST_CHECK_EQUAL( int(Layout::LayoutTraits<2>::LayoutType::Id), 2 );
		BOOST_CHECK_EQUAL( int(Layout::LayoutTraits<3>::LayoutType::Id), 3 );
	};
	
	void testMaskedLayoutTraits()
	{
		/// Here we test the use of the helper class that can convert an index into the number of
		/// channels - possibly masked - to the index of the layout that represents the channel.
		typedef BrothersLayout<float, Brothers_RGB> Layout1;
		typedef ChannelLayout<float, Chan_Alpha> Layout2;
		typedef ChannelLayout<float, Chan_Z> Layout3;
		typedef BrothersLayout<float, Brothers_UV> Layout4;
		typedef PixelLayout< Layout1, Layout2, Layout3, Layout4 > Layout;
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<0, Mask_All>::Value ), 0 ); // Red 
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<1, Mask_All>::Value ), 0 ); // Green 
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<2, Mask_All>::Value ), 0 ); // Blue
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<3, Mask_All>::Value ), 1 ); // Alpha
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<4>::Value ), 2 ); // Z
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<5>::Value ), 3 ); // U
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<6>::Value ), 3 ); // V
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<0, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::Value ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<1, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::Value ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<2, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::Value ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<3, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::Value ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<4, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::Value ), 3 );
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<0, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::Value ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<1, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::Value ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<2, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::Value ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<3, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::Value ), 3 );
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<0, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::Value ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<1, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::Value ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<2, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::Value ), 3 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelIndexToLayoutIndex<3, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::Value ), 3 );
	};
	
	void testChannelTraits()
	{
		/// Test the we can access the layouts using the ChannelTraits struct.
		{
			typedef TestLayout<0, Chan_Red> Storage0;
			typedef TestLayout<1, Chan_Blue> Storage1;
			typedef TestLayout<2, Chan_Z> Storage2;
			typedef TestLayout<3, Chan_U> Storage3;

			typedef PixelLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
			BOOST_CHECK_EQUAL( int(Layout::NumberOfLayouts), 4 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelToLayoutIndex<Chan_Red>::Value), 0 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelToLayoutIndex<Chan_Blue>::Value), 1 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelToLayoutIndex<Chan_Z>::Value), 2 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelToLayoutIndex<Chan_U>::Value), 3 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelTraits<Chan_Red>::LayoutType::Id), 0 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelTraits<Chan_Blue>::LayoutType::Id), 1 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelTraits<Chan_Z>::LayoutType::Id), 2 );
			BOOST_CHECK_EQUAL( int(Layout::ChannelTraits<Chan_U>::LayoutType::Id), 3 );
		}
	};
	
	void testContains()
	{
		typedef BrothersLayout<float, Brothers_RGB> Storage0;
		typedef ChannelLayout<float, Chan_Alpha> Storage1;
		typedef BrothersLayout<float, Brothers_VU> Storage2;
		
		typedef PixelLayout< Storage0, Storage1, Storage2 > Layout;
		Layout l;
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Alpha ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Blue ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Red ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Green ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_U ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_V ) ), true );
		BOOST_CHECK_EQUAL( int( l.contains( Chan_Z ) ), false );
	}

	void testStep()
	{
		typedef BrothersLayout<float, Brothers_RGB> Storage0;
		typedef ChannelLayout<float, Chan_Alpha> Storage1;
		typedef ChannelLayout<float, Chan_Z> Storage2;
		typedef DynamicLayout<float> Storage3;
		
		typedef PixelLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
		Layout l;
		l.addChannels( Mask_UV, Brothers_VU );
		
		BOOST_CHECK_EQUAL( int( l.step<Chan_Alpha>() ), 1 );
		BOOST_CHECK_EQUAL( int( l.step<Chan_Red>() ), 3 );
		BOOST_CHECK_EQUAL( int( l.step<Chan_Green>() ), 3 );
		BOOST_CHECK_EQUAL( int( l.step<Chan_Blue>() ), 3 );
		BOOST_CHECK_EQUAL( int( l.step<Chan_U>() ), 2 );
		BOOST_CHECK_EQUAL( int( l.step<Chan_V>() ), 2 );
		BOOST_CHECK_EQUAL( int( l.step( Chan_U ) ), 2 );
		BOOST_CHECK_EQUAL( int( l.step( Chan_V ) ), 2 );

		BOOST_CHECK_EQUAL( int( l.step( Chan_Alpha ) ), 1 );
		BOOST_CHECK_THROW( l.step( Chan_Mask ), std::runtime_error );
		BOOST_CHECK_THROW( l.step<Chan_Mask>(), std::runtime_error );
		
		l.addChannels( Mask_Mask );
		BOOST_CHECK_EQUAL( int( l.step( Chan_Mask ) ), 1 );
		BOOST_CHECK_EQUAL( int( l.step<Chan_Mask>() ), 1 );
	}

	void testCommonLayoutAttributes()
	{
		typedef BrothersLayout<float, Brothers_RGB> Storage0;
		typedef ChannelLayout<float, Chan_Alpha> Storage1;
		typedef ChannelLayout<float, Chan_Z> Storage2;
		typedef BrothersLayout<float, Brothers_VU> Storage3;
			
		typedef PixelLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
		BOOST_CHECK_EQUAL( int(Layout::NumberOfLayouts), 4 );
		BOOST_CHECK_EQUAL( int(Layout::NumberOfChannels), 7 );
		BOOST_CHECK_EQUAL( int(Layout::ChannelMask), int( Mask_RGB | Mask_Alpha | Mask_Z | Mask_UV ) );

		BOOST_CHECK_EQUAL( Layout().requiredChannels(), ChannelSet( Mask_Red | Mask_Alpha | Mask_Z | Mask_V ) );
	}
	
	void testDynamicLayout()
	{
		typedef PixelLayout< ChannelLayout< float, Chan_Red >, DynamicLayout< float > > Layout;
		Layout l;
	}
};

struct PixelLayoutTestSuite : public boost::unit_test::test_suite
{
	PixelLayoutTestSuite() : boost::unit_test::test_suite( "PixelLayoutTestSuite" )
	{
		boost::shared_ptr<PixelLayoutTest> instance( new PixelLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelLayoutTest::testLayoutTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelLayoutTest::testMaskedLayoutTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelLayoutTest::testChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelLayoutTest::testDynamicLayout, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelLayoutTest::testStep, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelLayoutTest::testContains, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelLayoutTest::testCommonLayoutAttributes, instance ) );
	}
};

void addPixelLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

