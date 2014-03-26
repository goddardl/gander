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
#ifndef __GANDER_LINEARCURVEFN_H__
#define __GANDER_LINEARCURVEFN_H__

#include <vector>

#include "Gander/Math.h"
#include "Gander/CurveFn.h"

#include "Eigen/Dense"

namespace Gander
{

/// LinearCurve2DFn
/// Implements a simple linear curve using the function Y = A*X + B.
template< class T, class ArrayType = Eigen::Matrix< T, Eigen::Dynamic, 1 > >
class LinearCurve2DFn : public CurveFn< T, ArrayType >
{
		typedef CurveFn< T, ArrayType > BaseType;

	public :

		/// The default constructor should initialize the parameters by calling
		/// BaseType::init() and then setting their values.
		inline LinearCurve2DFn() { BaseType::init(); A() = 1.; B() = 1.; }

		/// The () operator returns the result of function using the current parameters. 
		/// This is the implementation of y = a*x + b.
		inline T operator()( T x ) const
		{
			return A() * x + B();
		}

		/// Defines the number of parameters. In this case, there are 2: A and B.
		virtual int numberOfParameters() const { return 2; };

		/// Returns the current value of parameter A.
		inline T &A() { return CurveFn< T >::parameter(0); }
		inline const T &A() const { return CurveFn< T >::parameter(0); }

		/// Returns the current value of parameter B.
		inline T &B() { return CurveFn< T >::parameter(1); }
		inline const T &B() const { return CurveFn< T >::parameter(1); }
};

}; // namespace Gander

#endif
