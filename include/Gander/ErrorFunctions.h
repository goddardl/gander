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
#include "Gander/Assert.h"

namespace Gander
{

/// A base class for the standard Eigen::LevenbergMarquardt style functor.
/// Derived classes should implement the following two methods:
///
/// The error function at x to be returned in fvec. The vector x should be of length
/// inputs() and the vector fvec of length values(). 
///		int operator()( const VectorXType &x, VectorXType &fvec ) const
///
/// To be defined if automatic differentiation is not being used. 
/// int df( const VectorXType &x, MatrixXType &fJac ) const
///
class ErrorFn
{
	public :

		ErrorFn( unsigned int inputs = 0, unsigned int values = 0 ) :
			m_values( values ),
			m_inputs( inputs )
		{
		}

		/// Provides access to the number of observable values (the residuals).
		inline void setNumberOfValues( unsigned int values ) { m_values = values; }
		inline unsigned int getNumberOfValues() const { return m_values; }
		inline unsigned int values() const { return getNumberOfValues(); }
		
		/// Provides access to the number of unknowns to solve.
		inline void setNumberOfInputs( unsigned int inputs ) { m_inputs = inputs; }
		inline unsigned int getNumberOfInputs() const { return m_inputs; }
		inline unsigned int inputs() const { return getNumberOfInputs(); }

	private :

		unsigned int m_values;
		unsigned int m_inputs;

};

/// A Wrapper class for the Gander::ErrorFn which converts a Least Squares function
/// to a Least Squares Jacobian function using forward differences.
template< class LeastSquaresFn >
class ForwardDifferenceJacobian
{

	public:

		typedef ForwardDifferenceJacobian< LeastSquaresFn > Type;
		GANDER_DECLARE_EIGEN_TYPES( typename LeastSquaresFn::RealType )

		ForwardDifferenceJacobian( const LeastSquaresFn &fn, RealType step = std::numeric_limits<float>::epsilon() )
			:	m_fn( fn ),
			m_step( step )
		{}

		int operator()( const VectorXType &x, VectorXType &fvec ) const
		{
			GANDER_ASSERT( m_fn.inputs() > 0 && m_fn.values() > 0, "The number of values and inputs must be greater than 0." );
			return m_fn( x, fvec );
		}

		int df( const VectorXType &x, MatrixXType &fJac ) const
		{
			VectorXType v( getNumberOfInputs() );
			v = x;

			VectorXType fErr( getNumberOfValues() );
			m_fn( v, fErr );

			const RealType reciprocal = 1. / m_step;
			for( unsigned int parameter = 0; parameter < getNumberOfInputs(); ++parameter )
			{
				// Increment a parameter by a step.
				v( parameter ) += m_step;

				// Compute the new error vector.
				VectorXType stepErr( getNumberOfValues() );
				m_fn( v, stepErr );

				for( unsigned int i = 0; i < getNumberOfValues(); ++i )
				{
					fJac( i, parameter ) = ( stepErr(i) - fErr(i) ) * reciprocal;
				}

				// Restore the stepped parameter.
				v( parameter ) = x( parameter );
			}
			return 0;
		}

		inline unsigned int getNumberOfValues() const { return m_fn.getNumberOfValues(); }
		inline unsigned int values() const { return m_fn.values(); }

		inline unsigned int getNumberOfInputs() const { return m_fn.getNumberOfInputs(); }
		inline unsigned int inputs() const { return m_fn.inputs(); }

		inline RealType getStep() const { return m_step; }
		inline void setStep( RealType step ) { m_step = step; }

	private :

		const LeastSquaresFn &m_fn;
		RealType m_step;

};

}; // namespace Gander

#endif
