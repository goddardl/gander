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
#ifndef __GANDER_ERROR_FUNCTIONS_H__
#define __GANDER_ERROR_FUNCTIONS_H__

#include <vector>

#include "Gander/Math.h"

namespace Gander
{

/// A base class for the standard Eigen::LevenbergMarquardt style funtor.
/// Derived classes should implement the following two methods:
///
/// The error function at x to be returned in fvec. The vector x should be of length
/// inputs() and the vector fvec of length values(). 
///		int operator()( const Eigen::VectorXd &x, Eigen::VectorXd &fvec ) const
///
/// To be defined if automatic differentiation is not being used. 
/// int df( const Eigen::VectorXd &x, Eigen::MatrixXd &fJac ) const
///
class ErrorFn
{
public:

	ErrorFn( size_t values, size_t inputs )
		: m_values( values ), m_inputs( inputs )
	{
	}

	inline size_t values() const { return m_values; }
	inline size_t inputs() const { return m_inputs; }

private :
	
	const size_t m_values; // The number of observable values (the residuals).
	const size_t m_inputs; // The number of unknowns to solve.
};

/// A Wrapper class for the Gander::ErrorFn which converts a Least Squares function
/// to a Least Squares Jacobian function using forward differences.
template< class LeastSquaresFn >
class ForwardDifferenceJacobian
{

public:

	ForwardDifferenceJacobian( LeastSquaresFn &fn, double step = std::numeric_limits<float>::epsilon() )
	:	m_fn( fn ),
		m_step( step )
	{}

	int operator()( const Eigen::VectorXd &x, Eigen::VectorXd &fvec ) const
	{
		return m_fn( x, fvec );
	}

	int df( const Eigen::VectorXd &x, Eigen::MatrixXd &fJac ) const
	{
		Eigen::VectorXd v( inputs() );
		v = x;

		Eigen::VectorXd fErr( values() );
		m_fn( v, fErr );

		const double reciprocal = 1. / m_step;
		for( unsigned int parameter = 0; parameter < inputs(); ++parameter )
		{
			// Increment a parameter by a step.
			v( parameter ) += m_step;

			// Compute the new error vector.
			Eigen::VectorXd stepErr( values() );
			m_fn( v, stepErr );

			for( unsigned int i = 0; i < values(); ++i )
			{
				fJac( i, parameter ) = ( stepErr(i) - fErr(i) ) * reciprocal;
			}

			// Restore the steped parameter.
			v( parameter ) = x( parameter );
		}
		return 0;
	}

	inline size_t values() const { return m_fn.values(); }
	inline size_t inputs() const { return m_fn.inputs(); }
	inline double getStep() const { return m_step; }
	inline void setStep( double step ) { m_step = step; }

private :

	LeastSquaresFn &m_fn;
	double m_step;

};

}; // namespace Gander

#endif
