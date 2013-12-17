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
#ifndef __GANDERTEST_TESTTOOLSTEST_H__
#define __GANDERTEST_TESTTOOLSTEST_H__

#include <vector>
#include <stdlib.h>

#include "boost/test/unit_test.hpp"

namespace GanderTest
{

/// Compares two Eigen matrix or vector classes and returns true if all of their
/// elements are within an absolute error of each other.
template< class S, class T >
bool areEqualWithAbsError( const S &m1, const T &m2, double err = 10e-8 )
{
	if( m1.count() != m2.count() )
	{
		return false;
	}

	for( unsigned int i = 0; i < m1.count(); ++i )
	{
		if( fabs( m1(i) - m2(i) ) > err )
		{
			return false;
		}
	}
	return true;
}

/// Returns a random angle in radians between -PI and PI.
/// @param from The minimum angle in radians.
/// @param to The maximum angle in radians.
/// @return A random angle between from and to in radians.
double randomAngle( double from = -M_PI, double to = M_PI )
{
	if( to < from )
	{
		double t = from;
		from = to;
		to = t;
	}

	if( to == from )
	{
		return to;
	}

	return ( rand() % ( int( fabs( to - from ) ) ) + from );
}

}; // namespace GanderTest

#endif // __GANDERTEST_TESTTOOLSTEST_H__