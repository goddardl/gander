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

#include "boost/test/test_tools.hpp"
#include "boost/test/results_reporter.hpp"
#include "boost/test/unit_test_suite.hpp"
#include "boost/test/output_test_stream.hpp"
#include "boost/test/unit_test_log.hpp"
#include "boost/test/framework.hpp"
#include "boost/test/detail/unit_test_parameters.hpp"

#include "GanderImageTest/ChannelTest.h"
#include "GanderImageTest/ChannelBrothersTest.h"
#include "GanderImageTest/PPMTest.h"
#include "GanderImageTest/OpTest.h"
#include "GanderImageTest/CompoundLayoutTest.h"
#include "GanderImageTest/CompoundLayoutContainerTest.h"
#include "GanderImageTest/ChannelLayoutTest.h"
#include "GanderImageTest/BrothersLayoutTest.h"
#include "GanderImageTest/DynamicLayoutTest.h"
#include "GanderImageTest/PixelTest.h"
#include "GanderImageTest/PixelIteratorTest.h"
#include "GanderImageTest/RowTest.h"
#include "GanderImageTest/ImageTest.h"

using namespace boost::unit_test;
using boost::test_tools::output_test_stream;

using namespace Gander;
using namespace Gander::ImageTest;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	test_suite* test = BOOST_TEST_SUITE( "Gander Image unit test" );

	try
	{
		addPPMTest(test);
		addChannelTest(test);
		addChannelBrothersTest(test);
		addChannelLayoutTest(test);
		addBrothersLayoutTest(test);
		addDynamicLayoutTest(test);
		addCompoundLayoutTest(test);
		addCompoundLayoutContainerTest(test);
		addOpTest(test);
		addPixelTest(test);
		addPixelIteratorTest(test);
		addRowTest(test);
		addImageTest(test);
	}
	catch (std::exception &ex)
	{
		std::cerr << "Failed to create test suite: " << ex.what() << std::endl;
		throw;
	}

	return test;
}

