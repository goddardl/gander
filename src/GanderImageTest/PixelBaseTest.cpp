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
	void testChannelTraits()
	{
		/// Test the we can access the layouts using the ChannelTraits struct.
		{
			typedef TestLayout<0, Chan_Red> Storage0;
			typedef TestLayout<1, Chan_Blue> Storage1;
			typedef TestLayout<2, Chan_Z> Storage2;
			typedef TestLayout<3, Chan_U> Storage3;

			typedef PixelLayout< Storage0, Storage1, Storage2, Storage3 > Layout;

			//PixelBase< Layout > pixel;
			std::cerr << PixelBase< Layout >::ChannelTraits< Chan_Red >::LayoutType::Id << std::endl;
			std::cerr << PixelBase< Layout >::ChannelTraits< Chan_Blue >::LayoutType::Id << std::endl;
			std::cerr << PixelBase< Layout >::ChannelTraits< Chan_U >::LayoutType::Id << std::endl;
			std::cerr << PixelBase< Layout >::NumberOfLayouts << std::endl;
		}
	};
};

struct PixelBaseTestSuite : public boost::unit_test::test_suite
{
	PixelBaseTestSuite() : boost::unit_test::test_suite( "PixelBaseTestSuite" )
	{
		boost::shared_ptr<PixelBaseTest> instance( new PixelBaseTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelBaseTest::testChannelTraits, instance ) );
	}
};

void addPixelBaseTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelBaseTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

