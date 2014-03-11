//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
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

#include "GanderImageTest/RowTest.h"

#include "GanderImage/Row.h"

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

struct RowTest
{

	void testRowIterators()
	{
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGR >, ChannelLayout< float, Chan_Alpha >, DynamicLayout< float > > Layout;
		Row< Layout >::PixelIterator it;
		
		it->addChannels( Mask_U, Brothers_VU );
		it->addChannels( Mask_Z );

		float bgr[6] = { 3., 2., 1., 6., 5., 4. };
		float alpha[2] = { 7., 8. };
		float vu[4] = { 10., 9., 12., 11. };
		float z[2] = { 13., 14. };
		it->setChannelPointer( Chan_Blue, &bgr[0] );	
		it->setChannelPointer( Chan_Alpha, &alpha );	
		it->setChannelPointer( Chan_Z, &z );	
		it->setChannelPointer( Chan_U, &vu[1] );	

		Row< Layout > row( 2 );
		row.setStart( it );

		BOOST_CHECK_EQUAL( row.width(), 2 );
		BOOST_CHECK( row.getStart() == it );
		BOOST_CHECK( row.begin() == it );
		BOOST_CHECK( row.end() == it + 2 );
	}
};

struct RowTestSuite : public boost::unit_test::test_suite
{
	RowTestSuite() : boost::unit_test::test_suite( "RowTestSuite" )
	{
		boost::shared_ptr<RowTest> instance( new RowTest() );
		add( BOOST_CLASS_TEST_CASE( &RowTest::testRowIterators, instance ) );
	}
};

void addRowTest( boost::unit_test::test_suite *test )
{
	test->add( new RowTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

