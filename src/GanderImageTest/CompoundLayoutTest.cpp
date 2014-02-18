//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014, Luke Goddard. All rights reserved.
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
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImageTest/CompoundLayoutTest.h"

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
		NumberOfChannelPointers = 1,
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

struct CompoundLayoutTest
{
	void testLayoutTraits()
	{
		/// Create 4 test storage types that we can use to assert that access to the StorageTypes works correctly.
		typedef TestLayout<0, Chan_Red> Storage0;
		typedef TestLayout<1, Chan_Blue> Storage1;
		typedef TestLayout<2, Chan_Z> Storage2;
		typedef TestLayout<3, Chan_U> Storage3;

		typedef CompoundLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
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
		typedef CompoundLayout< Layout1, Layout2, Layout3, Layout4 > Layout;
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, Mask_All>::LayoutIndex ), 0 ); // Red 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, Mask_All>::LayoutIndex ), 0 ); // Green 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, Mask_All>::LayoutIndex ), 0 ); // Blue
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, Mask_All>::LayoutIndex ), 1 ); // Alpha
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<4>::LayoutIndex ), 2 ); // Z
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<5>::LayoutIndex ), 3 ); // U
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<6>::LayoutIndex ), 3 ); // V
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<4, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 3 );
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::LayoutIndex ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::LayoutIndex ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::LayoutIndex ), 3 );
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::LayoutIndex ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::LayoutIndex ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::LayoutIndex ), 3 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::LayoutIndex ), 3 );

		/// Check that we can calculate the index of the channel within the layout.
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<4, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<5, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, Mask_All>::ChannelIndexInLayout ), 0 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, Mask_All>::ChannelIndexInLayout ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, Mask_All>::ChannelIndexInLayout ), 2 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, Mask_All>::ChannelIndexInLayout ), 0 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<4, Mask_All>::ChannelIndexInLayout ), 0 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<5>::ChannelIndexInLayout ), 0 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<6>::ChannelIndexInLayout ), 1 ); 
	};
	
	void testChannelTraits()
	{
		/// Test the we can access the layouts using the ChannelTraits struct.
		{
			typedef TestLayout<0, Chan_Red> Storage0;
			typedef TestLayout<1, Chan_Blue> Storage1;
			typedef TestLayout<2, Chan_Z> Storage2;
			typedef TestLayout<3, Chan_U> Storage3;

			typedef CompoundLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
			BOOST_CHECK_EQUAL( int(Layout().numberOfChannels() ), 4 );
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
		
		typedef CompoundLayout< Storage0, Storage1, Storage2 > Layout;
		Layout l;
		
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_UV | Mask_RGB | Mask_Alpha ) );
		BOOST_CHECK_EQUAL( int( l.numberOfChannels() ), 6 );
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
		
		typedef CompoundLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
		Layout l;
		l.addChannels( Mask_UV, Brothers_VU );
		BOOST_CHECK_EQUAL( l.channels(), ChannelSet( Mask_UV | Mask_RGB | Mask_Z | Mask_Alpha ) );
		
		BOOST_CHECK_EQUAL( int( l.numberOfChannels() ), 7 );
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
		typedef DynamicLayout<float> Storage4;
			
		typedef CompoundLayout< Storage0, Storage1, Storage2, Storage3, Storage4 > Layout;
		BOOST_CHECK_EQUAL( int(Layout::NumberOfLayouts), 5 );
		BOOST_CHECK_EQUAL( int(Layout::NumberOfChannels), 7 );
		BOOST_CHECK_EQUAL( int(Layout::ChannelMask), int( Mask_RGB | Mask_Alpha | Mask_Z | Mask_UV ) );

		BOOST_CHECK_EQUAL( Layout().requiredChannels(), ChannelSet( Mask_Red | Mask_Alpha | Mask_Z | Mask_V ) );
				
		BOOST_CHECK( ( Layout().child<0>() == Storage0() ) );
		BOOST_CHECK( ( Layout().child<1>() == Storage1() ) );
		BOOST_CHECK( ( Layout().child<2>() == Storage2() ) );
		BOOST_CHECK( ( Layout().child<3>() == Storage3() ) );
		BOOST_CHECK( ( Layout().child<4>() == Storage4() ) );

		Storage4 dl;
		dl.addChannels( Mask_Mask );
		BOOST_CHECK_EQUAL( dl.requiredChannels(), ChannelSet( Mask_Mask) );
		BOOST_CHECK_EQUAL( int( dl.numberOfChannels() ), 1 );

		Layout l;
		l.addChannels( Mask_Mask );
		BOOST_CHECK_EQUAL( int( l.numberOfChannels() ), 8 );
		BOOST_CHECK_EQUAL( l.requiredChannels(), ChannelSet( Mask_Red | Mask_Alpha | Mask_Z | Mask_V | Mask_Mask) );
		
		BOOST_CHECK_EQUAL( ( l.child<4>() == Storage4() ), 0 );
		BOOST_CHECK_EQUAL( ( l.child<4>() == dl ), 1 );

		BOOST_CHECK_EQUAL( Layout() == Layout(), true );
		BOOST_CHECK_EQUAL( Layout() == Storage0(), false );
		BOOST_CHECK_EQUAL( Layout() == Storage2(), false );
		BOOST_CHECK_EQUAL( Layout() != Storage2(), true );
		BOOST_CHECK_EQUAL( Layout() != Layout(), false );
		BOOST_CHECK_EQUAL( Layout() != l, true );

		Layout l2;
		l2.addChannels( Mask_Mask );
		BOOST_CHECK_EQUAL( l2 == l, true );
		BOOST_CHECK_EQUAL( l2 != l, false );
		
		l2.addChannels( Mask_Backward );
		BOOST_CHECK_EQUAL( l2.channels(), ChannelSet( Mask_RGB | Mask_Alpha | Mask_Z | Mask_V | Mask_U | Mask_Mask | Mask_Backward ) );
		BOOST_CHECK_EQUAL( l2 != l, true );
		BOOST_CHECK_EQUAL( l2 == l, false );
	}
};

struct CompoundLayoutTestSuite : public boost::unit_test::test_suite
{
	CompoundLayoutTestSuite() : boost::unit_test::test_suite( "CompoundLayoutTestSuite" )
	{
		boost::shared_ptr<CompoundLayoutTest> instance( new CompoundLayoutTest() );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testLayoutTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testMaskedLayoutTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testChannelTraits, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testStep, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testContains, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testCommonLayoutAttributes, instance ) );
	}
};

void addCompoundLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new CompoundLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

