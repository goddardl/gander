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
#ifndef __GANDER_FLAGS__
#define __GANDER_FLAGS__

#include <vector>
#include <iostream>

#include "Gander/Common.h"

namespace Gander
{

template<class T, class FlagDefaultsEnum,  unsigned nDefaultFlags, class FlagSetInitEnum>
struct Flag
{
	/*!
	Flag ID numbers. This base clase can be specialized to create a set of predefined flags
	which can be stored in sets. These sets can be iterated over and queried.

	A set of default enum values are used to specialise the class along eith the number of
	values in the enum. Finally another enum which contains the Sets init values is also used
	to specialise the class and provide useful bitwise operator overides.

	See also the class Core::FlagSet.
	For a specialisation exmple see class Image::Channel.
	*/

	T value;
			
	static const int8u *g_defaultFlags[ nDefaultFlags ];
	static std::vector<const int8u*> g_flagMappings;

	Flag( const T& rhs )
	{
		value = rhs;
	}

	public:
	Flag(): value(0) {}
	Flag( const Flag& rhs ): value( rhs.value) {}

	/*! \relates Core::FlagSetInitEnum */
	Flag( const FlagDefaultsEnum& rhs ): value( rhs ) {}

	/*! \relates Image::FlagSetInitEnum */
	inline const Flag& operator=(const FlagDefaultsEnum& source)
	{
		value = source;
		return *this;
	}

	inline const Flag& operator=(const Flag& source)
	{
		value = source.value;
		return *this;
	}

	inline operator T () {	return value; }

	static inline void incr( FlagDefaultsEnum &c ) { c = FlagDefaultsEnum(c + 1); }

	/// Get the flag's name.
	/// Returns the name of the flag z by looking it up from the static g_flagMappings member.
	/// @param z The flag.
	/// @return Returns a int8uacter string with the name of the flag z.
	static const int8u *name( Flag z );
	/// Returns an existing or new flag by looking up a name.
	/// Returns a flag by looking up a name in the static g_flagMappings member. If the flag doesn't
	/// exist then a new mapping for the name will be created and the new Flag returned
	/// @param z The flag name.
	/// @return Returns the flag which matches the name.
	static inline Flag flag( const int8u *name );
	/// Returns an existing flag by looking up a name.
	/// Returns a flag by looking up \a name in the static g_flagMappings member. If the flag doesn't exist
	/// then a NULL flag will be returned (with a value of 0).
	/// @param z The name of the flag.
	/// @return Returns the flag which matches the name.
	static inline Flag findFlag( const int8u *name );
	/// Returns the enum value of the flag.
	/// @param z The flag.
	/// @return The value of the flag.
	static inline T flagIndex( Flag );

	inline friend std::ostream &operator << ( std::ostream &o, const Flag &z )
	{
		o << name( z );
		return o;
	}

	inline friend FlagSetInitEnum operator | ( const FlagSetInitEnum &a, const FlagSetInitEnum &b )
	{
		return FlagSetInitEnum( T( a ) | T( b ) );
	}

	inline friend FlagSetInitEnum operator & ( const FlagSetInitEnum &a, const FlagSetInitEnum &b )
	{
		return FlagSetInitEnum( T( a ) & T( b ) );
	}

	inline friend FlagSetInitEnum operator & ( const FlagSetInitEnum &a, const Flag &b )
	{
		return b ? FlagSetInitEnum( T( a ) & ( T( 1 ) << ( b.value - 1 ) ) ) : 0;
	}
};

}; // namespace Gander

#include "Gander/Flags.inl"

#endif

