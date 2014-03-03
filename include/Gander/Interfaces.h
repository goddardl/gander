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
#ifndef __GANDER_INTERFACES__
#define __GANDER_INTERFACES__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"

namespace Gander
{

/// A class template to express an equality comparison interface.
/// This class should be used as a base for any class that wishes to implement the comparison operators.
/// All the derived class has to do is implement the method equalTo :
/// bool equalTo( Derived const &rhs ) const;
template< typename Derived >
class EqualComparisonOperators
{
	template< class B >
	friend inline bool operator != ( Derived const &a, B const &b ) { return !a.equalTo(b); }

	template< class B >
	friend inline bool operator == ( Derived const &a, B const &b ) { return a.equalTo(b); }
};

/// A class template to define the pre-decrement and post-decrement operators.
/// This class should be used as a base for any class that wishes to implement the pre-decrement and post-decrement operators.
/// All the derived class has to do is implement the method equalTo :
/// Derived &decrement( int );
template< typename Derived >
class DecrementOperators
{
	friend inline Derived &operator -- ( Derived &a ) { return a.decrement( 1 ); }
	friend inline Derived operator -- ( Derived &a, int )
	{
		Derived d( a );
		a.decrement( 1 );
		return d;
	}
};

/// A class template to define the pre-increment and post-increment operators.
/// This class should be used as a base for any class that wishes to implement the pre-increment and post-increment operators.
/// All the derived class has to do is implement the method equalTo :
/// Derived &increment( int );
template< typename Derived >
class IncrementOperators
{
	friend inline Derived &operator ++ ( Derived &a ) { return a.increment( 1 ); }
	friend inline Derived operator ++ ( Derived &a, int )
	{
		Derived d( a );
		a.increment( 1 );
		return d;
	}
};

/// A class template to define the integer addition, subtraction, subtract-accumulate and add-accumulate operators.
/// In order for the derived class to use these operators, it must implement the folloing methods :
/// Derived &increment( int );
/// Derived &decrement( int );
template< typename Derived >
class IntegerArithmeticOperators
{
	/// The add-accumulate operator.
	friend inline Derived &operator += ( Derived &a, int i )
	{
		a.increment( i );
		return a;
	}
	/// The subtract-accumulate operator.
	friend inline Derived &operator -= ( Derived &a, int i )
	{
		a.decrement( i );
		return a;
	}
	/// Addition operator.
	friend inline Derived operator + ( Derived &a, int i )
	{
		Derived b( a );
		b += i;
		return b;
	}
	/// Subtraction operator.
	friend inline Derived operator - ( Derived &a, int i )
	{
		Derived b( a );
		b -= i;
		return b;
	}
};

}; // namespace Gander

#endif
