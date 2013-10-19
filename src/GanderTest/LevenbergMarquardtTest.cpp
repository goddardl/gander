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

#include "GanderTest/LevenbergMarquardtTest.h"
#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

using namespace Gander;
using namespace GanderTest;
using namespace boost;
using namespace boost::unit_test;

namespace GanderTest
{

namespace Detail
{

typedef std::vector< Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > Point2DVector;

/// Returns a set of points which are modeled on: y = a*x + b but include some noise.
void generatePoints( Point2DVector &points, double a, double b )
{
	points.clear();
	for( unsigned int i = 0; i < 50; ++i )
	{
		float x = static_cast<double>(i);
		Eigen::Vector2d point;
		point(0) = x;
		point(1) = a * x + b + drand48() / 10.0f;
		points.push_back( point );
	}
}

/// A simple error function that fits a line of form y = a*x + b to a set of points.
class CurveLeastSquaresFn : public Gander::ErrorFn
{

public:

	CurveLeastSquaresFn( const Point2DVector &points ) :
		Gander::ErrorFn( points.size(), 2 ),
		m_points( points )
	{}

	int operator()( const Eigen::VectorXd &x, Eigen::VectorXd &fvec ) const
	{
		for(unsigned int i = 0; i < m_points.size(); ++i)
		{
			double y = x(0) * m_points[i](0) + x(1);
			fvec(i) = ( y - m_points[i](1) ) * ( y - m_points[i](1) );
		}
		return 0;
	}
	
private :

	const Point2DVector &m_points;

};


}; // namespace Detail

struct LevenbergMarquardtTest
{
	/// A simple test case that fits a line with a function y = a*x + b
	/// to a set of points scattered along y = 2*x + 5 with some noise.
	/// It demonstrates the solving of parameters a and b. 
	/// This test code is based on an example by David Doria:
	/// https://github.com/daviddoria/Examples/blob/master/c%2B%2B/Eigen/LevenbergMarquardt/CurveFitting.cpp
	void testCurveFitting()
	{
		try
		{
			for( int a = 1; a < 10; a += 2 )
			{
				for( int b = 1; b < 10; b += 2 )
				{
					Detail::Point2DVector points;
					Detail::generatePoints( points, a, b ); // y = a*x + b (with noise).

					Detail::CurveLeastSquaresFn functor( points );
					ForwardDifferenceJacobian<Detail::CurveLeastSquaresFn> fn( functor );
					Eigen::LevenbergMarquardt<ForwardDifferenceJacobian<Detail::CurveLeastSquaresFn>, double> lm( fn );

					Eigen::VectorXd x(2);
					x.fill(1.0f);

					lm.parameters.ftol = 1e-6; // Error tolerance
					lm.parameters.xtol = 1e-6; // Parameter tolerance
					lm.parameters.maxfev = 1000; // Max iterations
					lm.minimize(x);

					// The error should be within 1 decimal place as the noise is +-.1	
					BOOST_CHECK_CLOSE_FRACTION( double(a), x(0), 1e-1 );
					BOOST_CHECK_CLOSE_FRACTION( double(b), x(1), 1e-1 );
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

struct LevenbergMarquardtTestSuite : public boost::unit_test::test_suite
{

	LevenbergMarquardtTestSuite() : boost::unit_test::test_suite( "LevenbergMarquardtTestSuite" )
	{
		boost::shared_ptr<LevenbergMarquardtTest> instance( new LevenbergMarquardtTest() );
		add( BOOST_CLASS_TEST_CASE( &LevenbergMarquardtTest::testCurveFitting, instance ) );
	}
};

void addLevenbergMarquardtTest( boost::unit_test::test_suite *test )
{
	test->add( new LevenbergMarquardtTestSuite( ) );
}

} // namespace GanderTest

