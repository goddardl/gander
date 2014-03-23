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

#include "boost/filesystem.hpp"

#include "GanderImageTest/PPMTest.h"
#include "GanderImageTest/PPM.h"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::ImageTest;
using namespace boost;
using namespace boost::unit_test;

namespace Gander
{

namespace ImageTest
{

	struct PPMTest
	{
		filesystem::path outputPath()
		{
			return filesystem::absolute( filesystem::path( outputDir ) );
		}

		PPMTest() : outputDir( "test/GanderImageTest/tmp/" )
		{
			if( !filesystem::exists( outputPath() ) )
			{
				filesystem::create_directories( outputPath() );				
			}
		}

		~PPMTest()
		{
			if( filesystem::exists( outputPath() ) && filesystem::is_directory( outputPath() ) )
			{
				filesystem::remove_all( outputPath() );
			}
		}

		void testPPM()
		{
			std::string tmpFile( "GanderImageTest_PPMTest_testPPM.ppm" );

			PPM ppm( 32, 32 ), ppmIn;

			srand( 0 );
			for( unsigned int y = 0; y < ppm.height; ++y )
			{
				for( unsigned int x = 0; x < ppm.width; ++x )
				{
					ppm.buf[ ( x + y * ppm.width ) * 3 ] = rand() % 255;						
					ppm.buf[ ( x + y * ppm.width ) * 3 + 1 ] = rand() % 255;						
					ppm.buf[ ( x + y * ppm.width ) * 3 + 2 ] = rand() % 255;						
				}
			}

			filesystem::path p = outputPath();
			p /= tmpFile;

			BOOST_CHECK( filesystem::exists( outputPath() ) );
			BOOST_CHECK( !filesystem::exists( p ) );
			ppm.write( p.string() );
			ppmIn.read( p.string() );

			BOOST_CHECK( ppm == ppmIn );
		}
		
		const std::string outputDir;
	};

	struct PPMTestSuite : public ::test_suite
	{
		PPMTestSuite() : ::test_suite( "PPMTestSuite" )
		{
			shared_ptr<PPMTest> instance( new PPMTest() );
			add( BOOST_CLASS_TEST_CASE( &PPMTest::testPPM, instance ) );
		}
	};

	void addPPMTest( ::test_suite *test )
	{
		test->add( new PPMTestSuite( ) );
	}

} // namespace ImageTest

} // namespace Gander

