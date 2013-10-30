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

/// The flag class can be specialized to create a set of predefined flags
/// which can be stored in sets. These sets can be iterated over and queried.
/// A set of default enum values are used to specialise the class along with the number of
/// values in the enum. Finally another enum which contains the sets init values is also used
/// to specialise the class and provide useful bitwise operator overides.
/// See also the class Gander::FlagSet.

template<class T, class FlagDefault,  unsigned nDefaultFlags, class FlagMask>
struct Flags
{

class Flag
{

public :

	inline Flag() : m_value(0) {}
	inline Flag( const Flag& rhs ) : m_value( rhs.m_value ) {}
	inline Flag( const FlagDefault &rhs ) : m_value( rhs ) {}

	inline const T &value() const { return m_value; }

	inline const Flag& operator = (const FlagDefault& source)
	{
		m_value = source;
		return *this;
	}

	inline const Flag& operator = (const Flag& source)
	{
		m_value = source.m_value;
		return *this;
	}
	
	//inline operator T () { return m_value; }
/*
	inline friend std::ostream &operator << ( std::ostream &o, const Flag &z )
	{
		o << name( z );
		return o;
	}

	inline friend FlagMask operator | ( const FlagDefault &a, const FlagDefault &b )
	{
		std::cerr << "|" << std::endl;
		return FlagMask( T( a ) | T( b ) );
	}

	inline friend FlagMask operator & ( const FlagMask &a, const FlagMask &b )
	{
		return FlagMask( T( a ) & T( b ) );
	}

	inline friend FlagMask operator & ( const FlagMask &a, const Flag &b )
	{
		return b ? FlagMask( T( a ) & ( T( 1 ) << ( b.m_value - 1 ) ) ) : 0;
	}
*/
	
	inline bool operator == ( const FlagDefault& source )
	{
		return ( source == m_value );
	}
	
	inline bool operator != ( const FlagDefault& source )
	{
		return ( source != m_value );
	}
	
	/// Get the flag's name.
	/// Returns the name of the flag z by looking it up from the static g_flagMappings member.
	/// @param z The flag.
	/// @return Returns a character string with the name of the flag z.
	static const char *name( Flag z );
	/// Returns an existing or new flag by looking up a name.
	/// Returns a flag by looking up a name in the static g_flagMappings member. If the flag doesn't
	/// exist then a new mapping for the name will be created and the new Flag returned
	/// @param z The flag name.
	/// @return Returns the flag which matches the name.
	static inline Flag flag( const char *name );
	/// Returns an existing flag by looking up a name.
	/// Returns a flag by looking up the name in the static g_flagMappings member. If the flag doesn't exist
	/// then a NULL flag will be returned (with a m_value of 0).
	/// @param z The name of the flag.
	/// @return Returns the flag which matches the name.
	static inline Flag findFlag( const char *name );
	/// Returns the enum m_value of the flag.
	/// @param z The flag.
	/// @return The m_value of the flag.
	static inline T index( Flag );

private :
	
	static inline void incr( FlagDefault &c ) { c = FlagDefault( c + 1 ); }

	T m_value;
	
	static const char *g_defaultFlags[ nDefaultFlags ];
	static std::vector<const char*> g_flagMappings;
		
};


#ifdef foreach
#undef foreach
#endif
#define foreach(VAR, SET) \
	for (VAR = SET.first(); VAR; VAR = SET.next(VAR))

/// The FlagSet class is a container for the Flag class.
class FlagSet
{

public :

	FlagSet() : m_mask(0) {}
	FlagSet( const FlagSet &source ) { m_mask = source.m_mask; }
	FlagSet( FlagMask v ) : m_mask( v ) { }
	FlagSet( Flag v ) { *this = v; }

	const FlagSet &operator = ( const FlagSet& source )
	{
		m_mask = source.m_mask; return *this;
	}

	const FlagSet &operator = ( FlagMask source ) 
	{
		m_mask = source;
		return *this;
	}

	const FlagSet &operator = ( Flag z )
	{
		m_mask = T(1) << ( z.value() - 1 );
		return *this;
	}

	void clear() { m_mask = 0; }
	operator bool() const { return m_mask; }
	T value() const { return m_mask; }
	bool empty() const { return !m_mask; }
	bool all() const { return ( m_mask & 0x80000000u ) != 0; }
	bool operator == ( const FlagSet &source ) const { return m_mask == source.m_mask; }
	bool operator != ( const FlagSet &source ) const { return !(*this == source); }
	bool operator < ( const FlagSet &source ) const { return (*this).size() < source.size(); }
	bool operator == ( FlagMask v ) const { return m_mask == T(v); }
	bool operator != ( FlagMask v ) const { return !(*this == v); }
	bool operator == ( Flag z ) const { return m_mask == T(1) << ( T(z) - 1 ); }
	bool operator != ( Flag z) const { return !(*this == z); }
	void operator += ( const FlagSet &source ) { m_mask |= source.m_mask; }
	void operator += ( FlagMask source ) { m_mask |= source; }
	void operator += ( Flag z ) { m_mask |= T(1) << ( z.value() - 1 ); }
	void insert( Flag z ) { *this += z; }
	void insert( const Flag* array, int n ) { for( int i = 0; i < n; ++i ) *this += array[i]; }
	void operator -= ( const FlagSet &source ) { m_mask &= ~source.m_mask; }
	void operator -= ( FlagMask source ) { m_mask &= ~source; }
	void operator -= ( Flag z ) { m_mask &= ~z.value(); };
	void erase( Flag z ) { *this -= z; }
	void erase( const Flag* array, int n ) { for( int i = 0; i < n; ++i ) *this -= array[i]; }
	void operator &= ( const FlagSet &source ) { m_mask &= source.m_mask; }
	void operator &= ( FlagMask source ) { m_mask &= source; }
	void operator &= ( Flag z ) { m_mask &= T(1) << ( T(z) - 1 ); }
	bool operator & ( const FlagSet &k ) const { return ( m_mask & ( T(1) << ( T( k ) - 1 ) ) ) != 0; }
	bool operator & ( FlagMask k ) const { return ( m_mask & k ) != 0; }
	bool operator & ( Flag k ) const { return ( m_mask & ( T(1) << ( T( k ) - 1 ) ) ) != 0; }
	unsigned size( Flag k ) const { return *this & k ? 1 : 0; }
	bool contains( const FlagSet &source ) const { return ( ( source.m_mask & m_mask ) == source.m_mask ); }
	bool contains( FlagMask source ) const { return !( ~m_mask & source ); }
	bool contains( Flag k ) const { return ( ( T(1) << ( k.value() - 1 ) ) & m_mask ) == ( T(1) << ( k.value() - 1 ) ); }

	template< class Type >
	FlagSet operator + ( Type z ) const
	{
		FlagSet tmp = *this;
		tmp += z;
		return tmp;
	}

	template< class Type >
	FlagSet operator - ( Type z ) const
	{
		FlagSet tmp = *this;
		tmp -= z;
		return tmp;
	}

	/// return the intersection of this FlagSet with another as a FlagSet
	template< class Type >
	FlagSet intersection( Type z ) const
	{
		FlagSet tmp = *this;
		tmp &= z;
		return tmp;
	}

	unsigned short size() const
	{
		unsigned short total = 0;
		for( unsigned short i = 0; i < ( sizeof(T) << 3 ); ++i )
		{
			total += ( ( ( m_mask >> i ) & T(1)) != 0 );
		}
		return total;
	}

	Flag first() const
	{
		int8u i = 0;
		for( ; i < ( T( sizeof(T) ) << 3 ) && ( ( m_mask >> i ) & T(1) ) == 0; ++i );
		return Flag( i+1 );
	}

	Flag next( Flag k ) const
	{
		if ( m_mask >> T( k ) == 0 )
		{
			return Flag( T( 0 ) );
		}
		int8u i = k;
		for( ; i < ( sizeof(T) << 3 ) && ( ( m_mask >> i ) & T(1) ) == 0; ++i );
		return Flag( i+1 );
	}

	Flag last() const
	{
		char i = ( sizeof( T ) << 3 ) - 1;
		for( ; i >= 0 && ( ( m_mask >> i ) & T(1) ) == 0; --i );
		return Flag( i + 1 );
	}

	Flag previous( Flag k ) const
	{
		int8u i = k;
		if ((m_mask & ~((~T(0)) << T(k-1))) == 0)
			return Flag(T(0));

		for (; i >= 0 && ((m_mask >> i) & T(1)) == 0; i--);
		return Flag(i-1) ;
	}

	friend std::ostream & operator << ( std::ostream &out, FlagSet &set )
	{
		Flag z;
		foreach( z, set )
		{
			out << z << " ";    
		}
		return out;
	}

private :

	T m_mask;

};

}; // struct Flags

}; // namespace Gander

#include "Gander/Flags.inl"

#endif

