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
#ifndef __GANDER_ARECLOSE_H__
#define __GANDER_ARECLOSE_H__

#include <vector>
#include <stdlib.h>

namespace Gander
{

/// Compares two Eigen classes and returns true if they equal within a tolerance.
/// The tolerance values are positive, typically very small numbers.
/// The relative difference (rtol * abs(b)) and the absolute difference atol are
/// added together to compare against the absolute difference between a and b.
template< typename DerivedA, typename DerivedB >
bool areClose(
	const Eigen::DenseBase<DerivedA>& a,
	const Eigen::DenseBase<DerivedB>& b,
	const typename DerivedA::RealScalar& rtol = Eigen::NumTraits<typename DerivedA::RealScalar>::dummy_precision(),
	const typename DerivedA::RealScalar& atol = Eigen::NumTraits<typename DerivedA::RealScalar>::epsilon()
	)
{
	return ( ( a.derived() - b.derived() ).array().abs() <= ( atol + rtol * b.derived().array().abs()) ).all();
}

}; // namespace Gander

#endif // __GANDER_ARECLOSE_H__
