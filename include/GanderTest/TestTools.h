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
#ifndef __GANDERTEST_TESTTOOLSTEST_H__
#define __GANDERTEST_TESTTOOLSTEST_H__

#include <vector>
#include <stdlib.h>

#include "Gander/Common.h"
#include "Gander/Math.h"
#include "Eigen/Dense"

#include "boost/test/unit_test.hpp"

namespace Gander
{

namespace Test
{

/// Returns a random number between the given range.
/// @param from The minimum value.
/// @param to The maximum value.
/// @return A random number between from and to.
double randomNumber( double from = 0., double to = 1. );

/// Returns a set of randomly placed points along the given curve with an optional amount of noise.
/// @param points The points on the curve to return.
/// @param curve The curve function to generate the points along.
/// @param numberOfPoints The number of points to generate along the curve.
/// @param noisePercent The percentage of random noise to offset the resulting points by.
template< class CurveFn >
void generatePointsOnCurve( typename CurveFn::Point2DArrayType &points, const CurveFn &curve, double xMin = 0, double xMax = 50, unsigned int numberOfPoints = 50, double noisePercent = 10. );

}; // namespace Test

}; // namespace Gander

#include "GanderTest/TestTools.inl"

#endif // __GANDERTEST_TESTTOOLSTEST_H__
