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
#ifndef __GANDERTEST_INTERFACESTEST_H__
#define __GANDERTEST_INTERFACESTEST_H__

#include <vector>

#include "Gander/Interfaces.h"

#include "boost/test/unit_test.hpp"

namespace Gander
{

namespace Test
{

void addInterfacesTest( boost::unit_test::test_suite *test );

/// A class for testing the Interface base classes.
/// This class derives from all of the available interfaces so that we can test that they work correctly.
template< class S >
struct InterfaceTest :
	private EqualComparisonOperators< InterfaceTest<S> >,
	private IncrementOperators< InterfaceTest<S> >,
	private DecrementOperators< InterfaceTest<S> >,
	private IntegerArithmeticOperators< InterfaceTest<S> >
{
	public :
		
		InterfaceTest() : data( 0 ) {}

		//! @name Equal Comparison Operators
		/// The method required to implement the equal comparison operators.
		//////////////////////////////////////////////////////////////
		//@{
		inline bool equalTo( InterfaceTest<S> const &rhs ) const
		{
			return data == rhs.data;
		}
		
		//! @name IntegerArithmetic, Increment and Decrment Operators
		/// The methods required to implement the
		/// IntegerArithmetic, Increment and Decrment Operators.
		//////////////////////////////////////////////////////////////
		//@{
		inline InterfaceTest &increment( int i )
		{
			data += i;
			return *this;
		};
		
		inline InterfaceTest &decrement( int i )
		{
			data -= i;
			return *this;
		};
		//@}
	
		/// A simple data type to test the interfaces with.	
		S data;
};

}; // namespace Test

}; // namespace Gander

#endif // __GANDERTEST_INTERFACESTEST_H__
