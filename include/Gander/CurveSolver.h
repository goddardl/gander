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
#ifndef __GANDER_CURVESOLVER_H__
#define __GANDER_CURVESOLVER_H__

#include <vector>
#include <set>

#include "Gander/Math.h"
#include "Gander/PointArray.h"
#include "Gander/ErrorFunctions.h"
#include "Gander/CurveFn.h"
#include "Gander/LinearCurveFn.h"

#include "unsupported/Eigen/NonLinearOptimization"

namespace Gander
{

/// CurveSolver2D
/// Provides a mechanism for fitting a curve with a known function to a set of 2D points.
/// The CurveSolver2D class accepts a FnType as a template argument
/// along with the type of the 2D points to be solved.
template< class ModelFn > 
class CurveSolver2D : public Gander::ErrorFn
{

	public:

		typedef ModelFn FnType;
		typedef CurveSolver2D< FnType > Type;
		GANDER_DECLARE_EIGEN_TYPES( typename FnType::RealType )
		
		CurveSolver2D( const FnType &fn, const Point2DArrayType *points, int maxIterations = 1000, double errorTolerance = 10e-6, double parameterTolerance = 10e-6 ) :
			ErrorFn( m_fn.numberOfParameters() ),
			m_fn( fn, points ),
			m_maxIterations( maxIterations ),
			m_errorTolerance( errorTolerance ),
			m_parameterTolerance( parameterTolerance ),
			m_step( std::numeric_limits<float>::epsilon() ),
			m_pointsPtr( NULL )
		{
			setPoints( points );
		}
		
		CurveSolver2D( const Point2DArrayType *points, int maxIterations = 1000, double errorTolerance = 10e-6, double parameterTolerance = 10e-6 ) :
			ErrorFn( m_fn.numberOfParameters() ),
			m_maxIterations( maxIterations ),
			m_errorTolerance( errorTolerance ),
			m_parameterTolerance( parameterTolerance ),
			m_step( std::numeric_limits<float>::epsilon() ),
			m_pointsPtr( NULL )
		{
			setPoints( points );
		}

		//! @name Parameter Accessors
		/// The setter and getter methods for the parameters of the
		/// Levenberg Marquardt algorithm and the curve function.
		//////////////////////////////////////////////////////////////
		//@{
		/// The maximum number of iterations to perform whilst minimizing.
		inline const int &maxIterations() const { return m_maxIterations; };
		inline int &maxIterations() { return m_maxIterations; };
		
		/// The maximum desired error tolerance to use when minimizing the function. 
		inline const double &errorTolerance() const { return m_errorTolerance; };
		inline double &errorTolerance() { return m_errorTolerance; };

		/// The maximum desired parameter tolerance to use when minimizing the function. 
		inline const double &parameterTolerance() const { return m_parameterTolerance; };
		inline double &parameterTolerance() { return m_parameterTolerance; };

		/// Returns the FnType class that is fitted to the set of data points.
		inline const FnType &fn() const { return m_fn; }
		inline FnType &fn() { return m_fn; }
		
		/// The step (epsilon) value to use when calculating the Jacobian of forward differences.
		inline const double &step() const { return m_step; };
		inline double &step() { return m_step; };

		/// Returns a constant pointer to the points that the curve is being fitted to.
		inline const Point2DArrayType *getPoints() const { return m_pointsPtr; }
		
		/// Sets the pointer to the list of observable points that the curve is being fitted to.
		inline void setPoints( const Point2DArrayType *points )
		{
			m_pointsPtr = points;
			if( points != NULL )
			{
				ErrorFn::setNumberOfValues( points->size() );
			}
		}
		//@}

		/// Executes the solver, solving the parameters in place.
		/// The solved parameters can be accessed from the FnType.
		void solve();

		/// Returns the mean error of the fitted model.
		double squaredMeanError() const;
		
		/// Returns an VectorXType of the errors of each point from the fitted model.
		/// This method assumes that the Vector is of the same length as the number of points.
		void errorVector( VectorXType &fvec ) const;
		
		/// Returns an VectorXType of the errors of each point from the curve function
		/// when using a set of parameters that are passed into the method.
		/// This method assumes that the Vector is of the same length as the number of points.
		void errorVector( const VectorXType &parameters, VectorXType &fvec ) const;
		
		/// This operator is called by the Eigen::LevenbergMarquardt
		/// algorithm to compute an array of errors.
		int operator()( const VectorXType &x, VectorXType &fvec ) const;
		
	private :

		FnType m_fn;
		int m_maxIterations;
		double m_errorTolerance;
		double m_parameterTolerance;
		double m_step;
		const Point2DArrayType *m_pointsPtr;

};

/// Fits the function y = a*x + b to a set of points.
/// This method is just a wrapper for the Curve2DSolver using a LinearCurve2DFn.
/// @param a Returns the solved value for A.
/// @param b Returns the solved value for B.
/// @param points FloatPoint2dArray or DoublePoint2dArray that contains a set of points that the curve should be fitted to.
/// @param maxIterations The maximum number of iterations to perform whilst minimizing.
/// @param errorTolerance The maximum desired error tolerance to use when minimizing the function. 
/// @param parameterTolerance The maximum desired parameter tolerance to use when minimizing the function. 
template< class T > 
void fitLinearCurve2D(
	T &a,
	T &b,
	const std::vector< Eigen::Matrix< T, 2, 1 >, Eigen::aligned_allocator< Eigen::Matrix< T, 2, 1 > > > &points,
	int maxIterations = 1000,
	double errorTolerance = 1e-6,
	double parameterTolerance = 1e-6
	);

/// Fits the function y = 1. - pow( 1. - a, x / b ) to a set of points.
/// This method is just a wrapper for the Curve2DSolver using an ExponentialCurve2DFn.
/// @param a Returns the solved value for A.
/// @param b Returns the solved value for B.
/// @param points FloatPoint2dArray or DoublePoint2dArray that contains a set of points that the curve should be fitted to.
/// @param maxIterations The maximum number of iterations to perform whilst minimizing.
/// @param errorTolerance The maximum desired error tolerance to use when minimizing the function. 
/// @param parameterTolerance The maximum desired parameter tolerance to use when minimizing the function. 
template< class T > 
void fitExponentialCurve2D(
	T &a,
	T &b,
	const std::vector< Eigen::Matrix< T, 2, 1 >, Eigen::aligned_allocator< Eigen::Matrix< T, 2, 1 > > > &points,
	int maxIterations = 1000,
	double errorTolerance = 1e-6,
	double parameterTolerance = 1e-6
	);

}; // namespace Gander

#include "Gander/CurveSolver.inl"

#endif
