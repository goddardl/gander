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

#include "GanderTest/ParameterizedModelTest.h"

#include "Gander/ParameterizedModel.h"

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

struct ParameterizedModelTest
{
	void testAccessors()
	{
		ParameterizedModel< double > p;

		Eigen::Vector2d v( 1, 2 ), v2;
		p.addParameter( "A", v );

		p.getParameter( "A", v2 );
		BOOST_CHECK_EQUAL( v, v2 );
		
		p.setParameter( "A", ( Eigen::VectorXd( 2 ) << 3, 4 ).finished() );
		p.getParameter( "A", v );
		BOOST_CHECK_EQUAL( v, Eigen::Vector2d( 3, 4 ) );
		
		p.getParameter( 0, v2 );
		BOOST_CHECK_EQUAL( v2, Eigen::Vector2d( 3, 4 ) );
		
		p.setParameter( 0, Eigen::Vector2d( 5, 6 ) );
		p.getParameter( 0, v );
		BOOST_CHECK_EQUAL( v, Eigen::Vector2d( 5, 6 ) );
	
		Eigen::Matrix2d m = ( Eigen::Matrix2d() << 1.1, 1.2, 1.3, 1.4 ).finished(), m2 = Eigen::Matrix2d::Zero();	
		p.addParameter( "B", m );
		p.getParameter( "B", m2 );
		BOOST_CHECK_EQUAL( m2, m );
		BOOST_CHECK_EQUAL( m2, ( Eigen::Matrix2d() << 1.1, 1.2, 1.3, 1.4 ).finished() );

		p.setParameter( "B", ( Eigen::MatrixXd( 2, 2 ) << 1, 2, 3, 4 ).finished() );
		p.getParameter( "B", m );
		BOOST_CHECK_EQUAL( m, ( Eigen::MatrixXd( 2, 2 ) << 1, 2, 3, 4 ).finished() );
	
		BOOST_CHECK_EQUAL( p.parameterName( 0 ), "A" );
		BOOST_CHECK_EQUAL( p.parameterName( 1 ), "B" );
			
		BOOST_CHECK_EQUAL( p.parameters(), ( ParameterizedModel< double >::VectorXType( p.parameters().size() ) << 5, 6, 1, 3, 2, 4 ).finished() );

		BOOST_CHECK_THROW( p.addParameter( "A", v ), std::runtime_error ); // Try to add an existing parameter.
		BOOST_CHECK_THROW( p.getParameter( "C", v ), std::runtime_error ); // Try to get a non-existent parameter.
		BOOST_CHECK_THROW( p.getParameter( "A", m ), std::runtime_error ); // Get a parameter of the wrong size.
	}
};

struct ParameterizedModelTestSuite : public boost::unit_test::test_suite
{
	ParameterizedModelTestSuite() : boost::unit_test::test_suite( "ParameterizedModelTestSuite" )
	{
		boost::shared_ptr<ParameterizedModelTest> instance( new ParameterizedModelTest() );
		add( BOOST_CLASS_TEST_CASE( &ParameterizedModelTest::testAccessors, instance ) );
	}
};

void addParameterizedModelTest( boost::unit_test::test_suite *test )
{
	test->add( new ParameterizedModelTestSuite( ) );
}

} // namespace Gander

} // namespace Test

