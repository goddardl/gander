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
//  IS" AND ANY EBPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EBEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////
#ifndef __GANDER_EXPONENTIALCURVEFN_H__
#define __GANDER_EXPONENTIALCURVEFN_H__

#include <vector>

#include "Gander/Math.h"
#include "Gander/CurveFn.h"

#include "Eigen/Dense"
#include "Eigen/Geometry"

namespace Gander
{

/// ExponentialCurve2DFn
/// Implements an exponential curve using the function Y = 1. - pow( 1 - A, x / B )
/// Where A is in the range of 0-1.
template< class Real >
class ExponentialCurve2DFn : public CurveFn< ExponentialCurve2DFn< Real >, Real >
{
		typedef CurveFn< ExponentialCurve2DFn< Real >, Real > BaseType;

	public :
		
		typedef ExponentialCurve2DFn< Real > Type;
		GANDER_DECLARE_EIGEN_TYPES( Real )

		inline ExponentialCurve2DFn( double a, double b ) { A() = a; B() = b; }
		inline ExponentialCurve2DFn() { A() = .5; B() = 1.; }

		/// The static compute method returns the result of the curve function using
		/// the given parameters.
		/// This is the implementation of the curve Y = 1. - pow( 1. - A, x / B )
		static inline RealType compute( RealType x, const VectorXType &parameters )
		{
			RealType A = parameters(0);
			RealType B = parameters(1);
			if( A >= 1. )
			{
				return 1.;
			}
			else if( x >= B )
			{
				return A;
			}
			else if( x < 0 )
			{
				return 0.;
			}
			else
			{
				x /= B;

				/// The code below is a more numerically stable version of:
				/// return 1. - pow( 1. - A, B );
				if( A > RealType( std::numeric_limits<float>::min() ) )
				{
					return -expm1( x * log1p( -A ) );
				}
				else
				{
					return A * x;
				}
			}
		}

		/// Defines the number of parameters. In this case, there are 2: A and B.
		inline unsigned int numberOfParameters() const { return 2; };

		/// Returns the current value of parameter A.
		inline RealType &A() { return BaseType::parameter(0); }
		inline const RealType &A() const { return BaseType::parameter(0); }

		/// Returns the current value of parameter B.
		inline RealType &B() { return BaseType::parameter(1); }
		inline const RealType &B() const { return BaseType::parameter(1); }
};

}; // namespace Gander

#endif
