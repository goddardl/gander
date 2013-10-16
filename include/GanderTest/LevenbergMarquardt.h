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
#ifndef __GANDER_LEVENBERGMARQUARDT_H__
#define __GANDER_LEVENBERGMARQUARDT_H__

#include <vector>

#include "unsupported/Eigen/NonLinearOptimization"
#include "Gander/ErrorFunctions.h"

namespace GanderTest
{

typedef std::vector< Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > Point2DVector;

/// A simple error function that fits a line of form y = a*x + b to a set of points.
class CurveLeastSquaresFn : public Gander::ErrorFn
{

public:

	CurveLeastSquaresFn( const GanderTest::Point2DVector &points ) :
		Gander::ErrorFn( points.size(), 2 ),
		m_points( points )
	{}

	int operator()( const Eigen::VectorXd &x, Eigen::VectorXd &fvec ) const
	{
		for(unsigned int i = 0; i < m_points.size(); ++i)
		{
			double y = x(0) * m_points[i](0) + x(1);
			fvec(i) = ( y - m_points[i](1) ) * ( y - m_points[i](1) );
		}
		return 0;
	}
	
private :

	const GanderTest::Point2DVector &m_points;

};

}; // namespace Gander

#endif

