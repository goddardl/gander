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

#include "Gander/LinearCurveFn.h"
#include "Gander/ExponentialCurveFn.h"

namespace Gander
{

template< class CurveFN > 
void CurveSolver2D< CurveFN >::solve()
{
	typedef ForwardDifferenceJacobian< Type > FDJacobian;

	// Create a copy of the curve function's initial parameters.	
	typename FnType::VectorXType resolvedParameters( m_fn.parameters() );

	// Wrap this class with a an error function for computing the jacobian
	// needed by the Levenberg Marquardt algorithm.
	FDJacobian fn( *this, m_step );

	// Create the LM class, set it's values and run it.
	Eigen::LevenbergMarquardt< FDJacobian > lm( fn );

	lm.parameters.ftol = m_errorTolerance;
	lm.parameters.xtol = m_parameterTolerance;
	lm.parameters.maxfev = m_maxIterations;
	lm.minimize( resolvedParameters );

	// Write the parameters back to the curve function.
	m_fn.parameters() = resolvedParameters;
}
		
template< class CurveFN > 
int CurveSolver2D< CurveFN >::operator()( const VectorXType &x, VectorXType &fvec ) const
{
	errorVector( x, fvec );
	return 0;
}

template< class CurveFN > 
double CurveSolver2D< CurveFN >::meanError() const
{
	double sum( 0. );
	VectorXType errors;
	errors.resize( m_points.size() );
	errorVector( errors );

	for( unsigned int i = 0; i < m_points.size(); ++i )
	{
		sum += errors(i);
	}

	return sum / double( m_points.size() );
}

template< class CurveFN > 
void CurveSolver2D< CurveFN >::errorVector( VectorXType &fvec ) const
{
	errorVector( m_fn.parameters(), fvec );
}

template< class CurveFN > 
void CurveSolver2D< CurveFN >::errorVector( const VectorXType &parameters, VectorXType &fvec ) const
{
	for( unsigned int i = 0; i < m_points.size(); ++i )
	{
		RealType y = FnType::compute( m_points[i](0), parameters );
		fvec(i) = ( y - m_points[i](1) ) * ( y - m_points[i](1) );
	}
}

template< class T > 
void fitLinearCurve2D(
		T &a,
		T &b,
		const std::vector< Eigen::Matrix< T, 2, 1 >, Eigen::aligned_allocator< Eigen::Matrix< T, 2, 1 > > > &points,
		int maxIterations,
		double errorTolerance,
		double parameterTolerance
		)
{
	CurveSolver2D< LinearCurve2DFn< T > > curveSolver( points, maxIterations, errorTolerance, parameterTolerance );
	curveSolver.solve();
	a = curveSolver.fn().A();
	b = curveSolver.fn().B();
};

template< class T > 
void fitExponentialCurve2D(
		T &a,
		T &b,
		const std::vector< Eigen::Matrix< T, 2, 1 >, Eigen::aligned_allocator< Eigen::Matrix< T, 2, 1 > > > &points,
		int maxIterations,
		double errorTolerance,
		double parameterTolerance
		)
{
	CurveSolver2D< ExponentialCurve2DFn< T > > curveSolver( points, maxIterations, errorTolerance, parameterTolerance );
	curveSolver.solve();
	a = curveSolver.fn().A();
	b = curveSolver.fn().B();
};

}; // namespace Gander

