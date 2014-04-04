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
#ifndef __GANDER_CURVEFN_H__
#define __GANDER_CURVEFN_H__

#include <vector>

#include "Gander/Math.h"
#include "Gander/PointArray.h"

namespace Gander
{

/// CurveFn
/// The base class for implementing a curve function of the form y = x.
/// Derived classes should implement two methods:
/// unsigned int numberOfParameters() const; // Return the number of parameters that the model requires.
/// static T compute( double x, const VectorXType &parameters ); // Returns the y value for a given x value using the parameters supplied.
template< class Derived, class Real >
class CurveFn
{
	public :
		
		typedef CurveFn< Derived, Real > Type;
		GANDER_DECLARE_EIGEN_TYPES( Real )

		CurveFn() : m_parameters( static_cast< const Derived * >( this )->numberOfParameters() )
		{
		}
		
		/// The () operator returns the result of the curve function using
		/// the current member parameters. It does this by calling the static
		/// implementation of compute() on the Derived class.
		inline RealType operator()( RealType x ) const
		{
			return static_cast< const Derived * >( this )->compute( x, m_parameters );
		}

		inline VectorXType &parameters() { return m_parameters; }
		inline const VectorXType &parameters() const { return m_parameters; }
		
		inline RealType &parameter( int index ) { return m_parameters[index]; }
		inline const RealType &parameter( int index ) const { return m_parameters(index); }

		/// Defines the number of parameters. This should be overridden by the derived class.
		inline unsigned int numberOfParameters() const { return 0; };
	
	protected :

		VectorXType m_parameters;

};

}; // namespace Gander

#endif
