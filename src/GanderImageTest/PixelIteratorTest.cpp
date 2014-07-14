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

#include "GanderImageTest/PixelIteratorTest.h"
#include "GanderImage/PixelIterator.h"

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

struct PixelIteratorTest
{
	void testPixelIteratorConstructors()
	{
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGR >, ChannelLayout< float, Chan_Alpha > > Layout;
		typedef PixelIterator< Layout > PixelIterator;
		typedef Pixel< Layout > Pixel;
		typedef ConstPixelIterator< Layout > ConstPixelIterator;
		
		PixelIterator it;
		BOOST_CHECK( ( it->channels() == ChannelSet( Mask_RGBA ) ) );
		
		float bgr[6] = { 3., 2., 1., 6., 5., 4. };
		float alpha[2] = { 7., 8. };
		it->setChannelPointer( Chan_Blue, &bgr[0] );	
		it->setChannelPointer( Chan_Alpha, &alpha );	

		// Test the copy constructor from a PixelIterator.
		ConstPixelIterator it2( it );
		BOOST_CHECK( ( it == it2 ) );
		
		// Test the copy constructor from another ConstPixelIterator.
		ConstPixelIterator it3( it2 );
		BOOST_CHECK( ( it3 == it2 ) );

		// Test the copy constructor from another PixelIterator.
		PixelIterator it4( it );
		BOOST_CHECK( ( it4 == it ) );
		
		// Test assignment to and from the Pixel type.
		Pixel p;
		BOOST_CHECK( ( *it != p ) );

		// Copy the values from the iterator. 
		p = *it;
		BOOST_CHECK( ( *it == p ) );

		// Test that we can change the pointer of a ConstPixelAccessor.
		float alpha2[2] = { 30., 40. };
		it3->setChannelPointer( Chan_Alpha, &alpha2 );
		BOOST_CHECK_EQUAL( it3->channel<Chan_Alpha>(), 30. );

		// Test that we can change the pointer of a PixelAccessor.
		it->setChannelPointer( Chan_Alpha, &alpha2 );
		BOOST_CHECK_EQUAL( it->channel<Chan_Alpha>(), 30. );

		// Now change a value of the pixel and try to assign it to a pixel iterator.
		BOOST_CHECK( p.channel< Chan_Alpha >() == 7. );
		p.channel< Chan_Alpha >() = 25.;
		BOOST_CHECK( p.channel< Chan_Alpha >() == 25. );
		
		*it = p;

		BOOST_CHECK_EQUAL( it->channel<Chan_Alpha>(), 25. );
		BOOST_CHECK_EQUAL( alpha2[0], 25. );
	}
	
	void testConstPixelIterator()
	{
		typedef CompoundLayout< BrothersLayout< float, Brothers_BGR >, ChannelLayout< float, Chan_Alpha > > Layout;
		typedef PixelIterator< Layout > PixelIterator;
		typedef ConstPixelIterator< Layout > ConstPixelIterator;
		
		PixelIterator it;
		float bgr[6] = { 3., 2., 1., 6., 5., 4. };
		float alpha[2] = { 7., 8. };
		it->setChannelPointer( Chan_Blue, &bgr[0] );	
		it->setChannelPointer( Chan_Alpha, &alpha );	
		
		BOOST_CHECK_EQUAL( it->channels(), ChannelSet( Mask_RGBA ) );
		BOOST_CHECK_EQUAL( it->requiredChannels(), ChannelSet( Mask_Blue | Mask_Alpha ) );
		
		ConstPixelIterator cit( it );
		BOOST_CHECK_EQUAL( cit->channels(), ChannelSet( Mask_RGBA ) );
		BOOST_CHECK_EQUAL( cit->requiredChannels(), ChannelSet( Mask_Blue | Mask_Alpha ) );

		BOOST_CHECK_EQUAL( cit->channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( (*cit).channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( cit->channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( cit->channel<Chan_Alpha>(), 7. );

		BOOST_CHECK_EQUAL( (*cit)[Chan_Red], 1. );
		BOOST_CHECK_EQUAL( (*cit)[Chan_Green], 2. );
		BOOST_CHECK_EQUAL( (*cit)[Chan_Blue], 3. );
		BOOST_CHECK_EQUAL( (*cit)[Chan_Alpha], 7. );

		cit++;
		BOOST_CHECK_EQUAL( cit->channel<Chan_Red>(), 4. );
		BOOST_CHECK_EQUAL( (*cit).channel<Chan_Green>(), 5. );
		BOOST_CHECK_EQUAL( cit->channel<Chan_Blue>(), 6. );
		BOOST_CHECK_EQUAL( cit->channel<Chan_Alpha>(), 8. );

		--cit;
		BOOST_CHECK_EQUAL( cit->channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( (*cit).channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( cit->channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( cit->channel<Chan_Alpha>(), 7. );
		
		float bgr2[6] = { 3., 2., 1., 6., 5., 4. };
		PixelIterator it2( *it ); // Test the constructor from a PixelAccessor.
		BOOST_CHECK( ( cit == it2 ) );
		it2->setChannelPointer( Chan_Blue, &bgr2[0] );	
		BOOST_CHECK( ( cit != it2 ) );
	}
	
	void testPixelIterator()
	{
		typedef PixelIterator< BrothersLayout< float, Brothers_BGR > > PixelIterator;
		PixelIterator it;
		
		BOOST_CHECK_EQUAL( it->requiredChannels(), ChannelSet( Mask_Blue ) );
	
		float bgr[6] = { 3., 2., 1., 6., 5., 4. };
		it->setChannelPointer( Chan_Blue, &bgr[0] );	

		BOOST_CHECK_EQUAL( it->channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( (*it).channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Blue>(), 3. );

		it->channel<Chan_Green>() = 0.;
		BOOST_CHECK_EQUAL( it->channel<Chan_Green>(), 0. );
		(*it).channel<Chan_Green>() = 2.;
		BOOST_CHECK_EQUAL( it->channel<Chan_Green>(), 2. );

		it++;
		BOOST_CHECK_EQUAL( it->channel<Chan_Red>(), 4. );
		BOOST_CHECK_EQUAL( (*it).channel<Chan_Green>(), 5. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Blue>(), 6. );

		--it;
		BOOST_CHECK_EQUAL( it->channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( (*it).channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Blue>(), 3. );
		
		float bgr2[6] = { 3., 2., 1., 6., 5., 4. };
		PixelIterator it2( it );
		BOOST_CHECK( ( it == it2 ) );
		it2->setChannelPointer( Chan_Blue, &bgr2[0] );	
		BOOST_CHECK( ( it != it2 ) );
	}

	void testCompoundPixelIterator()
	{
		typedef PixelIterator< CompoundLayout< BrothersLayout< float, Brothers_BGR >, ChannelLayout< float, Chan_Alpha > > > PixelIterator;
		PixelIterator it;
		
		BOOST_CHECK_EQUAL( it->channels(), ChannelSet( Mask_RGBA ) );
		BOOST_CHECK_EQUAL( it->requiredChannels(), ChannelSet( Mask_Blue | Mask_Alpha ) );
	
		float bgr[6] = { 3., 2., 1., 6., 5., 4. };
		float alpha[2] = { 7., 8. };
		it->setChannelPointer( Chan_Blue, &bgr[0] );	
		it->setChannelPointer( Chan_Alpha, &alpha );	

		BOOST_CHECK_EQUAL( it->channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( (*it).channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Alpha>(), 7. );

		it->channel<Chan_Green>() = 0.;
		BOOST_CHECK_EQUAL( it->channel<Chan_Green>(), 0. );
		(*it).channel<Chan_Green>() = 2.;
		BOOST_CHECK_EQUAL( it->channel<Chan_Green>(), 2. );

		it++;
		BOOST_CHECK_EQUAL( it->channel<Chan_Red>(), 4. );
		BOOST_CHECK_EQUAL( (*it).channel<Chan_Green>(), 5. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Blue>(), 6. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Alpha>(), 8. );
		
		BOOST_CHECK_EQUAL( (*it)[Chan_Red], 4. );
		BOOST_CHECK_EQUAL( (*it)[Chan_Green], 5. );
		BOOST_CHECK_EQUAL( (*it)[Chan_Blue], 6. );
		BOOST_CHECK_EQUAL( (*it)[Chan_Alpha], 8. );

		--it;
		BOOST_CHECK_EQUAL( it->channel<Chan_Red>(), 1. );
		BOOST_CHECK_EQUAL( (*it).channel<Chan_Green>(), 2. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Blue>(), 3. );
		BOOST_CHECK_EQUAL( it->channel<Chan_Alpha>(), 7. );
		
		float bgr2[6] = { 3., 2., 1., 6., 5., 4. };
		PixelIterator it2( it );
		BOOST_CHECK( ( it == it2 ) );
		it2->setChannelPointer( Chan_Blue, &bgr2[0] );	
		BOOST_CHECK( ( it != it2 ) );
	}
};

struct PixelIteratorTestSuite : public boost::unit_test::test_suite
{
	PixelIteratorTestSuite() : boost::unit_test::test_suite( "PixelIteratorTestSuite" )
	{
		boost::shared_ptr<PixelIteratorTest> instance( new PixelIteratorTest() );
		add( BOOST_CLASS_TEST_CASE( &PixelIteratorTest::testPixelIterator, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelIteratorTest::testCompoundPixelIterator, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelIteratorTest::testConstPixelIterator, instance ) );
		add( BOOST_CLASS_TEST_CASE( &PixelIteratorTest::testPixelIteratorConstructors, instance ) );
	}
};

void addPixelIteratorTest( boost::unit_test::test_suite *test )
{
	test->add( new PixelIteratorTestSuite() );
}

} // namespace ImageTest

} // namespace Gander

