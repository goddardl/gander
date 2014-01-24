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
#ifndef __GANDER_STATICASSERT_H__
#define __GANDER_STATICASSERT_H__

#include "Gander/Common.h"

namespace Gander
{

namespace Detail
{

template<bool condition>
struct static_assertion {};

template<>
struct static_assertion<true>
{
	enum
	{
		// The derived class has not implemented a function that is required by the base class.
		DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS,
		// The tuple class can be defined as either being static or dynamic. In the static case,
		// all of the dynamic methods raise this static error.
		THIS_METHOD_CANNOT_BE_CALLED_ON_A_STATIC_TUPLE,
		// An index of value is outside of the required range.
		VALUE_IS_OUT_OUT_BOUNDS,
	};
};

}; // namespace Detail

#define GANDER_STATIC_ASSERT(CONDITION,MSG) \
	enum{ CAT( CAT(ERROR, __AT__ ), __LINE__ ) = Detail::static_assertion<bool(CONDITION)>::MSG };

// Prints a compile time error when the code is compiled. This can be used to ensure that a template method is never compiled.
#define GANDER_STATIC_ASSERT_ERROR(MSG) \
	const int MSG; MSG = 1;

}; // namespace Gander

#endif
