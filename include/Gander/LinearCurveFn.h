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
#include "Eigen/Geometry"

namespace Gander
{

/// LinearCurve2DFn
/// Implements a simple linear curve using the function Y = A*X + B.
template< class Real >
class LinearCurve2DFn : public CurveFn< LinearCurve2DFn< Real >, Real >
{
		typedef CurveFn< LinearCurve2DFn< Real >, Real > BaseType;

	public :
		
		typedef LinearCurve2DFn< Real > Type;
		GANDER_DECLARE_EIGEN_TYPES( Real )
		typedef Eigen::ParametrizedLine< Real, 2 > ParametrizedLineType;

		inline LinearCurve2DFn( double a, double b ) { A() = a; B() = b; }
		inline LinearCurve2DFn() { A() = 1.; B() = 1.; }

		/// The static compute method returns the result of the curve function using
		/// the given parameters.
		/// This is the implementation of y = a*x + b.
		static inline RealType compute( RealType x, const VectorXType &parameters )
		{
			return parameters(0) * x + parameters(1);
		}

		/// Defines the number of parameters. In this case, there are 2: A and B.
		inline unsigned int numberOfParameters() const { return 2; };

		/// Returns the current value of parameter A.
		inline RealType &A() { return BaseType::parameter(0); }
		inline const RealType &A() const { return BaseType::parameter(0); }

		/// Returns the current value of parameter B.
		inline RealType &B() { return BaseType::parameter(1); }
		inline const RealType &B() const { return BaseType::parameter(1); }
		
		/// Returns a parametrized line that matches this curve.
		ParametrizedLineType parametrizedLine() const
		{
			ParametrizedLineType line;
			line = ParametrizedLineType::Through( Vector2Type( 0., ( *this )( 0. ) ), Vector2Type( 10., ( *this )( 10. ) ) );
			return line;
		}
};

/*
 *
/// LinearCurve2DFn
/// Implements a simple linear curve using the function Y = A*X + B.
template< class Real >
class LinearCurve2DFn : public ParameterizedModel< Linear< Curve2DFn, Real >, Real, 1, 1 >
{
		typedef ParameterizedModel< Linear< Curve2DFn, Real >, Real, 1, 1 > BaseType;

	public :
		
		typedef LinearCurve2DFn< Real > Type;
		GANDER_DECLARE_EIGEN_TYPES( Real )
		typedef typename BaseType::ModelType ModelType;
		typedef Eigen::ParametrizedLine< Real, 2 > ParametrizedLineType;

		inline LinearCurve2DFn( double a, double b, double x )
		{
			BaseType::addParameter< double >( "A", a );
			BaseType::addParameter< double >( "B", b );
			BaseType::addParameter< double >( "X", x );
		}

		inline LinearCurve2DFn()
		{
			BaseType::addParameter< double >( "A", 1. );
			BaseType::addParameter< double >( "B", 1. );
			BaseType::addParameter< double >( "X", 0. );
		}

		/// The static compute method returns the result of the curve function using
		/// the given parameters.
		/// This is the implementation of y = a*x + b.
		static inline void compute( RealType x, const VectorXType &parameters )
		{
			this needs to be implemented
		}

		/// Returns the current value of parameter A.
		inline unsigned int A() const { return 0; }

		/// Returns the current value of parameter B.
		inline unsigned int B() const { return 1; }
		
		/// Returns the current value of parameter X.
		inline unsigned int X() const { return 2; }
		
		/// Returns a parametrized line that matches this curve.
		ParametrizedLineType parametrizedLine() const
		{
			ParametrizedLineType line;
			line = ParametrizedLineType::Through( Vector2Type( 0., ( *this )( 0. ) ), Vector2Type( 10., ( *this )( 10. ) ) );
			return line;
		}
};
*/

}; // namespace Gander

#endif
