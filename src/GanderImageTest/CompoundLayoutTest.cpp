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
	enum
	{
		NumberOfChannels = 1,
		NumberOfChannelPointers = 1,
		ChannelMask = ChannelToMask<S>::Value,
		ChannelPointerMask = ChannelMask,
		Id = ID,
	};
	
	typedef typename Gander::template TypeTraits< float >::Type ChannelType;
	typedef typename Gander::template TypeTraits< float >::StorageType StorageType;
	typedef typename Gander::template TypeTraits< float >::PointerType PointerType;
	typedef typename Gander::template TypeTraits< float >::ReferenceType ReferenceType;
	typedef typename Gander::template TypeTraits< float >::ConstReferenceType ConstReferenceType;
	typedef Gander::template Tuple< StorageType, NumberOfChannels, false > ChannelContainerType;
	typedef Gander::template Tuple< PointerType, NumberOfChannelPointers, false > ChannelPointerContainerType;
		
	template< ChannelDefault >
	struct ChannelTraits
	{
		typedef TestLayout< ID, S > LayoutType;
		typedef typename LayoutType::StorageType StorageType;
		typedef typename LayoutType::ChannelType ChannelType;
		typedef typename LayoutType::PointerType PointerType;
		typedef typename LayoutType::ReferenceType ReferenceType;
		typedef typename LayoutType::ConstReferenceType ConstReferenceType;
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

		typedef CompoundLayout< Layout1, Layout2, Layout3 > Layout;
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, Mask_All>::LayoutIndex ), 0 ); // Red 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, Mask_All>::LayoutIndex ), 0 ); // Green 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, Mask_All>::LayoutIndex ), 0 ); // Blue
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, Mask_All>::LayoutIndex ), 1 ); // Alpha
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<4>::LayoutIndex ), 2 ); // Z
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, CombineMasks< Mask_RGB, Mask_Z, Mask_U >::Value >::LayoutIndex ), 2 );
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::LayoutIndex ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::LayoutIndex ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, CombineMasks< Mask_Green, Mask_Alpha, Mask_Z, Mask_V >::Value >::LayoutIndex ), 2 );
		
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::LayoutIndex ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_Alpha, Mask_Z, Mask_UV >::Value >::LayoutIndex ), 2 );

		/// Check that we can calculate the index of the channel within the layout.
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 2 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, CombineMasks< Mask_RGB, Mask_Z, Mask_UV >::Value >::ChannelIndexInLayout ), 0 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<0, Mask_All>::ChannelIndexInLayout ), 0 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<1, Mask_All>::ChannelIndexInLayout ), 1 );
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<2, Mask_All>::ChannelIndexInLayout ), 2 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<3, Mask_All>::ChannelIndexInLayout ), 0 ); 
		BOOST_CHECK_EQUAL( int( Layout::ChannelTraitsAtIndex<4>::ChannelIndexInLayout ), 0 ); 
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

	void testContainerAccess() 
	{
		typedef BrothersLayout<float, Brothers_BGR> Storage0;
		typedef ChannelLayout<float, Chan_Alpha> Storage1;
		typedef ChannelLayout<float, Chan_Z> Storage2;
		typedef BrothersLayout<float, Brothers_VU> Storage3;
			
		typedef CompoundLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
		
		Layout layout;
		BOOST_CHECK( ( layout.child<0>() == Storage0() ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Red >::LayoutType, Storage0 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Green >::LayoutType, Storage0 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Blue >::LayoutType, Storage0 >::value ) );
		BOOST_CHECK( ( layout.child<1>() == Storage1() ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Alpha >::LayoutType, Storage1 >::value ) );
		BOOST_CHECK( ( layout.child<2>() == Storage2() ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Z >::LayoutType, Storage2 >::value ) );
		BOOST_CHECK( ( layout.child<3>() == Storage3() ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_U >::LayoutType, Storage3 >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_V >::LayoutType, Storage3 >::value ) );

		Layout::ChannelContainerType c( layout );
		Layout::ChannelPointerContainerType cp( layout );

		BOOST_CHECK_EQUAL( layout.channels(), ChannelSet( Mask_RGBA | Mask_Z | Mask_UV ) );
		BOOST_CHECK_EQUAL( layout.requiredChannels(), ChannelSet( Mask_Blue | Mask_Alpha | Mask_Z | Mask_V ) );
		BOOST_CHECK_EQUAL( cp.size(), layout.numberOfChannelPointers() );
		BOOST_CHECK_EQUAL( c.size(), layout.numberOfChannels() );
		
		layout.channel< Chan_Red >( c ) = 1.;
		layout.channel< Chan_Green >( c ) = 2.;
		layout.channel< Chan_Blue >( c ) = 3.;
		layout.channel< Chan_Alpha >( c ) = 4.;
		layout.channel< Chan_Z >( c ) = 5.;
		layout.channel< Chan_U >( c ) = 6.;
		layout.channel< Chan_V >( c ) = 7.;
		
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Red >( c ) ), 1. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Green >( c ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Blue >( c ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Alpha >( c ) ), 4. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Z >( c ) ), 5. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_U >( c ) ), 6. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_V >( c ) ), 7. );
		
		layout.channelAtIndex< 0 >( c ) = 8.;
		layout.channelAtIndex< 1 >( c ) = 7.;
		layout.channelAtIndex< 2 >( c ) = 6.;
		layout.channelAtIndex< 3 >( c ) = 5.;
		layout.channelAtIndex< 4 >( c ) = 4.;
		layout.channelAtIndex< 5 >( c ) = 3.;
		layout.channelAtIndex< 6 >( c ) = 2.;
		
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 0 >( c ) ), 8. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 1 >( c ) ), 7. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 2 >( c ) ), 6. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 3 >( c ) ), 5. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 4 >( c ) ), 4. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 5 >( c ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 6 >( c ) ), 2. );

		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Red >( c ) ), 8. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Green >( c ) ), 7. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Blue >( c ) ), 6. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Alpha >( c ) ), 5. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Z >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_U >( c ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_V >( c ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, ChannelMask( CombineMasks< Mask_Z, Mask_V >::Value ) >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Z, Mask_V >::Value ) >( c ) ), 2. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Alpha, Mask_Z, Mask_Forward >::Value ) >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 2, ChannelMask( CombineMasks< Mask_Red, Mask_Z, Mask_Green >::Value ) >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V, Mask_Backward >::Value ) >( c ) ), 8. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V, Mask_Backward >::Value ) >( c ) ), 6. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 2, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V, Mask_Backward >::Value ) >( c ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 3, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V, Mask_Backward >::Value ) >( c ) ), 2. );

		float bgra[4] = { 3., 2., 1., 4. };	
		float z = 5.;	
		float vu[2] = { 7., 6. };	

		layout.setChannelPointer( cp, Chan_Blue, &bgra[0] );
		layout.setChannelPointer( cp, Chan_Alpha, &bgra[3] );
		layout.setChannelPointer( cp, Chan_Z, &z );
		layout.setChannelPointer( cp, Chan_V, &vu[0] );

		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Red >( cp ) ), 1. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Green >( cp ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Blue >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Alpha >( cp ) ), 4. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Z >( cp ) ), 5. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_U >( cp ) ), 6. );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_V >( cp ) ), 7. );
		
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 0 >( cp ) ), 1. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 1 >( cp ) ), 2. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 2 >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 3 >( cp ) ), 4. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 4 >( cp ) ), 5. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 5 >( cp ) ), 6. );
		BOOST_CHECK_EQUAL( ( layout.channelAtIndex< 6 >( cp ) ), 7. );
		
		layout.channel< Chan_Red >( cp ) = 9.;
		layout.channel< Chan_Green >( cp ) = 8.;
		layout.channel< Chan_Blue >( cp ) = 7.;
		layout.channel< Chan_Alpha >( cp ) = 6.;
		layout.channel< Chan_Z >( cp ) = 5.;
		layout.channel< Chan_U >( cp ) = 4.;
		layout.channel< Chan_V >( cp ) = 3.;
		
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Red >( cp ) ), bgra[2] );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Green >( cp ) ), bgra[1] );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Blue >( cp ) ), bgra[0] );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Alpha >( cp ) ), bgra[3] );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_Z >( cp ) ), z );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_U >( cp ) ), vu[1] );
		BOOST_CHECK_EQUAL( ( layout.channel< Chan_V >( cp ) ), vu[0] );

		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Red >( cp ) ), 9. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Green >( cp ) ), 8. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Blue >( cp ) ), 7. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Alpha >( cp ) ), 6. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_Z >( cp ) ), 5. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_U >( cp ) ), 4. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, Mask_V >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, ChannelMask( CombineMasks< Mask_V, Mask_Backward >::Value ) >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Z, Mask_Alpha >::Value ) >( cp ) ), 5. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_V, Mask_U, Mask_Forward >::Value ) >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 2, ChannelMask( CombineMasks< Mask_Green, Mask_Alpha, Mask_V >::Value ) >( cp ) ), 3. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 0, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V >::Value ) >( cp ) ), 9. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 1, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V >::Value ) >( cp ) ), 7. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 2, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V >::Value ) >( cp ) ), 5. );
		BOOST_CHECK_EQUAL( float( layout.channelAtIndex< 3, ChannelMask( CombineMasks< Mask_Red, Mask_Blue, Mask_Z, Mask_V >::Value ) >( cp ) ), 3. );
	}

	void testCommonLayoutAttributes()
	{
		typedef BrothersLayout<float, Brothers_RGB> Storage0;
		typedef ChannelLayout<float, Chan_Alpha> Storage1;
		typedef ChannelLayout<float, Chan_Z> Storage2;
		typedef BrothersLayout<float, Brothers_VU> Storage3;
			
		typedef CompoundLayout< Storage0, Storage1, Storage2, Storage3 > Layout;
		BOOST_CHECK_EQUAL( int(Layout::NumberOfLayouts), 4 );
		BOOST_CHECK_EQUAL( int(Layout::NumberOfChannels), 7 );
		BOOST_CHECK_EQUAL( int(Layout::ChannelMask), int( Mask_RGB | Mask_Alpha | Mask_Z | Mask_UV ) );

		BOOST_CHECK_EQUAL( Layout().requiredChannels(), ChannelSet( Mask_Red | Mask_Alpha | Mask_Z | Mask_V ) );
				
		BOOST_CHECK( ( Layout().child<0>() == Storage0() ) );
		BOOST_CHECK( ( Layout().child<1>() == Storage1() ) );
		BOOST_CHECK( ( Layout().child<2>() == Storage2() ) );
		BOOST_CHECK( ( Layout().child<3>() == Storage3() ) );

		Layout l;
		BOOST_CHECK_EQUAL( int( l.numberOfChannels() ), 7 );
		BOOST_CHECK_EQUAL( l.requiredChannels(), ChannelSet( Mask_Red | Mask_Alpha | Mask_Z | Mask_V ) );
		
		BOOST_CHECK_EQUAL( Layout() == Layout(), true );
		BOOST_CHECK_EQUAL( Layout() == Storage0(), false );
		BOOST_CHECK_EQUAL( Layout() == Storage2(), false );
		BOOST_CHECK_EQUAL( Layout() != Storage2(), true );
		BOOST_CHECK_EQUAL( Layout() != Layout(), false );
		BOOST_CHECK_EQUAL( Layout() != l, false );

		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Red >::LayoutType, BrothersLayout<float, Brothers_RGB> >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Green >::LayoutType, BrothersLayout<float, Brothers_RGB> >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Blue >::LayoutType, BrothersLayout<float, Brothers_RGB> >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Alpha >::LayoutType, ChannelLayout<float, Chan_Alpha> >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_Z >::LayoutType, ChannelLayout<float, Chan_Z> >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_V >::LayoutType, BrothersLayout<float, Brothers_VU> >::value ) );
		BOOST_CHECK( ( std::is_same< Layout::ChannelTraits< Chan_U >::LayoutType, BrothersLayout<float, Brothers_VU> >::value ) );
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
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testContains, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testCommonLayoutAttributes, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CompoundLayoutTest::testContainerAccess, instance ) );
	}
};

void addCompoundLayoutTest( boost::unit_test::test_suite *test )
{
	test->add( new CompoundLayoutTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

