//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
//  Copyright (c) 2014, Image Engine. All rights reserved.
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

#include "Gander/Math.h"
#include "Gander/CurveSolver.h"
#include "Gander/LinearCurveFn.h"
#include "Gander/ExponentialCurveFn.h"
#include "Gander/PointArray.h"

#include "GanderTest/CurveSolverTest.h"
#include "GanderTest/TestTools.h"

#include "Eigen/Geometry"

#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace Gander::Test;
using namespace boost;
using namespace boost::unit_test;

namespace Gander
{

namespace Test
{

struct CurveSolverTest
{
	
	void testExponentialCurveSolver()
	{
		try
		{
			for( int a = 1; a < 10; a += 1 )
			{
				for( int b = 1; b < 10; b += 1 )
				{
					ExponentialCurve2DFn<double> curve( a / 10., b );
					ExponentialCurve2DFn<double>::Point2DArrayType points;
					generatePointsOnCurve( points, curve, 0., b );

					double A = 0, B = 0;
					CurveSolver2D< ExponentialCurve2DFn< double > > curveSolver( points, 1000 );
					curveSolver.solve();
					A = curveSolver.fn().A();
					B = curveSolver.fn().B();

					// The error should be within 1 decimal place as the noise is +-.1	
					BOOST_CHECK( curveSolver.meanError() < 0.1 );
				}
			}
		}
		catch ( std::exception &e ) 
		{
			BOOST_WARN( !e.what() );
			BOOST_CHECK( !"Exception thrown during LevenbergMarquardtTest." );
		}
	}

	void testLinearCurveSolver()
	{
		try
		{
			for( int a = 1; a < 10; a += 1 )
			{
				for( int b = 1; b < 10; b += 1 )
				{
					LinearCurve2DFn<double> curve( a, b );
					LinearCurve2DFn<double>::Point2DArrayType points;
					generatePointsOnCurve( points, curve ); // y = a*x + b (with 10% noise).

					double A = 0, B = 0;
					Gander::fitLinearCurve2D( A, B, points );

					// The error should be within 1 decimal place as the noise is +-.1	
					BOOST_CHECK_CLOSE_FRACTION( A, a, 1e-1 );
					BOOST_CHECK_CLOSE_FRACTION( B, b, 1e-1 );
				}
			}
		}
		catch ( std::exception &e ) 
		{
			BOOST_WARN( !e.what() );
			BOOST_CHECK( !"Exception thrown during LevenbergMarquardtTest." );
		}
	}

};

struct CurveSolverTestSuite : public boost::unit_test::test_suite
{
	CurveSolverTestSuite() : boost::unit_test::test_suite( "CurveSolverTestSuite" )
	{
		boost::shared_ptr<CurveSolverTest> instance( new CurveSolverTest() );
		add( BOOST_CLASS_TEST_CASE( &CurveSolverTest::testLinearCurveSolver, instance ) );
		add( BOOST_CLASS_TEST_CASE( &CurveSolverTest::testExponentialCurveSolver, instance ) );
	}
};

void addCurveSolverTest( boost::unit_test::test_suite *test )
{
	test->add( new CurveSolverTestSuite( ) );
}

}; // namespace Test

}; // namespace Gander

