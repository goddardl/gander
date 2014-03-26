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
/// The base class for implementing a curve function.
/// Derived classes should override the numberOfParameters() method and
/// add the () operator to implement the curve's function:
/// T operator( double x ) const; // Returns the y value for a given x value.
template< class T, class ArrayType = Eigen::Matrix< T, Eigen::Dynamic, 1 > >
class CurveFn
{
	public :

		/// This method should be overridden by derived classes to return
		/// the number of parameters that are required by this curve.
		virtual int numberOfParameters() const = 0;

		inline ArrayType &parameters() { return m_parameters; }
		inline const ArrayType &parameters() const { return m_parameters; }
		
		inline T &parameter( int index ) { return m_parameters[index]; }
		inline const T &parameter( int index ) const { return m_parameters[index]; }

		/// Initializes the array to a length of numberOfParameters().
		/// The default constructor of the derived class should initialize
		/// the parameters by calling BaseType::init() in it's constructor and
		/// then setting their values.
		void init() { m_parameters.resize( numberOfParameters() ); }

	protected :

		ArrayType m_parameters;

};

}; // namespace Gander

#endif
