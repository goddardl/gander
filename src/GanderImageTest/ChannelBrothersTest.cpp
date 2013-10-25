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

#include "GanderImage/ChannelBrothers.h"
#include "GanderImageTest/ChannelBrothersTest.h"

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

struct ChannelBrothersTest
{
	void testBrothersTraitsRegistry()
	{
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::numberOfBrothers( Brothers_RGB ) ), 3 );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::numberOfBrothers( Brothers_RGBA ) ), 4 );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::numberOfBrothers( Brothers_BGR ) ), 3 );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::numberOfBrothers( Brothers_BGRA ) ), 4 );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::numberOfBrothers( Brothers_UV ) ), 2 );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::numberOfBrothers( Brothers_VU ) ), 2 );
		
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::brotherMask( Brothers_RGB ) ), int( BrotherTraits< Brothers_RGB >::BrothersMask ) );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::brotherMask( Brothers_RGBA ) ), int( BrotherTraits< Brothers_RGBA >::BrothersMask ) );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::brotherMask( Brothers_BGR ) ), int( BrotherTraits< Brothers_BGR >::BrothersMask ) );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::brotherMask( Brothers_BGRA ) ), int( BrotherTraits< Brothers_BGRA >::BrothersMask ) );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::brotherMask( Brothers_UV ) ), int( BrotherTraits< Brothers_UV >::BrothersMask ) );
		BOOST_CHECK_EQUAL( int( BrotherTraits<>::brotherMask( Brothers_VU ) ), int( BrotherTraits< Brothers_VU >::BrothersMask ) );
	}

	void testBrotherTraitsSpecialization()
	{
		/// Check that we can decalare a specialization of the ChannelBrothers class.
		/// If there was no specialization, a compilation error would occur.
	
		{
		typedef BrotherTraits< Brothers_RGB > Traits;
		BOOST_CHECK_EQUAL( int( Traits::NumberOfBrothers ), 3 );
		BOOST_CHECK_EQUAL( int( Traits::BrothersMask ), 7 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder1 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder2 ), 1 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder3 ), 2 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder4 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::OffsetsShiftedByChannelValue ), 36 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOfLowestValue ), int( Chan_Red ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothers ), int( Chan_Red ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothersMask ), int( Mask_Red ) );
		}
		
		{
		typedef BrotherTraits< Brothers_RGBA > Traits;
		BOOST_CHECK_EQUAL( int( Traits::NumberOfBrothers ), 4 );
		BOOST_CHECK_EQUAL( int( Traits::BrothersMask ), 15 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder1 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder2 ), 1 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder3 ), 2 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder4 ), 3 );
		BOOST_CHECK_EQUAL( int( Traits::OffsetsShiftedByChannelValue ), 228 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOfLowestValue ), int( Chan_Red ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothers ), int( Chan_Red ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothersMask ), int( Mask_Red ) );
		}
		
		{
		typedef BrotherTraits< Brothers_BGR > Traits;
		BOOST_CHECK_EQUAL( int( Traits::NumberOfBrothers ), 3 );
		BOOST_CHECK_EQUAL( int( Traits::BrothersMask ), 7 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder1 ), 2 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder2 ), 1 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder3 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder4 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::OffsetsShiftedByChannelValue ), 6 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOfLowestValue ), int( Chan_Red ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothers ), int( Chan_Blue ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothersMask ), int( Mask_Blue ) );
		}
		
		{
		typedef BrotherTraits< Brothers_BGRA > Traits;
		BOOST_CHECK_EQUAL( int( Traits::NumberOfBrothers ), 4 );
		BOOST_CHECK_EQUAL( int( Traits::BrothersMask ), 15 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder1 ), 2 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder2 ), 1 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder3 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder4 ), 3 );
		BOOST_CHECK_EQUAL( int( Traits::OffsetsShiftedByChannelValue ), 198 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOfLowestValue ), int( Chan_Red ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothers ), int( Chan_Blue ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothersMask ), int( Mask_Blue ) );
		}
		
		{
		typedef BrotherTraits< Brothers_UV > Traits;
		BOOST_CHECK_EQUAL( int( Traits::NumberOfBrothers ), 2 );
		BOOST_CHECK_EQUAL( int( Traits::BrothersMask ), 96 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder1 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder2 ), 1 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder3 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder4 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::OffsetsShiftedByChannelValue ), 4096 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOfLowestValue ), int( Chan_U ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothers ), int( Chan_U ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothersMask ), int( Mask_U ) );
		}
		
		{
		typedef BrotherTraits< Brothers_VU > Traits;
		BOOST_CHECK_EQUAL( int( Traits::NumberOfBrothers ), 2 );
		BOOST_CHECK_EQUAL( int( Traits::BrothersMask ), 96 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder1 ), 1 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder2 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder3 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOrder4 ), 0 );
		BOOST_CHECK_EQUAL( int( Traits::OffsetsShiftedByChannelValue ), 1024 );
		BOOST_CHECK_EQUAL( int( Traits::BrotherOfLowestValue ), int( Chan_U ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothers ), int( Chan_V ) );
		BOOST_CHECK_EQUAL( int( Traits::FirstBrotherInBrothersMask ), int( Mask_V ) );
		}
	}
};

struct ChannelBrothersTestSuite : public boost::unit_test::test_suite
{
	ChannelBrothersTestSuite() : boost::unit_test::test_suite( "ChannelBrothersTestSuite" )
	{
		boost::shared_ptr<ChannelBrothersTest> instance( new ChannelBrothersTest() );
		add( BOOST_CLASS_TEST_CASE( &ChannelBrothersTest::testBrotherTraitsSpecialization, instance ) );
		add( BOOST_CLASS_TEST_CASE( &ChannelBrothersTest::testBrothersTraitsRegistry, instance ) );
	}
};

void addChannelBrothersTest( boost::unit_test::test_suite *test )
{
	test->add( new ChannelBrothersTestSuite( ) );
}

} // namespace ImageTest

} // namespace Gander

