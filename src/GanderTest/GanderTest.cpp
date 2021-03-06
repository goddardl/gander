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

#include "GanderTest/LevenbergMarquardtTest.h"
#include "GanderTest/HomographyTest.h"
#include "GanderTest/AngleConversionTest.h"
#include "GanderTest/DecomposeRQ3x3Test.h"
#include "GanderTest/CommonTest.h"
#include "GanderTest/EnumHelperTest.h"
#include "GanderTest/BitTwiddlerTest.h"
#include "GanderTest/TupleTest.h"
#include "GanderTest/InterfacesTest.h"
#include "GanderTest/CurveSolverTest.h"
#include "GanderTest/ParameterizedModelTest.h"

using namespace boost::unit_test;
using boost::test_tools::output_test_stream;

using namespace Gander;
using namespace Gander::Test;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	test_suite* test = BOOST_TEST_SUITE( "Gander unit test" );

	try
	{
		addLevenbergMarquardtTest(test);
		addHomographyTest(test);
//		addDecomposeRQ3x3Test(test);
		addAngleConversionTest(test);
		addCommonTest(test);
		addEnumHelperTest(test);
		addBitTwiddlerTest(test);
		addTupleTest(test);
		addInterfacesTest(test);
		addCurveSolverTest(test);
		addParameterizedModelTest(test);
	}
	catch (std::exception &ex)
	{
		std::cerr << "Failed to create test suite: " << ex.what() << std::endl;
		throw;
	}

	return test;
}
