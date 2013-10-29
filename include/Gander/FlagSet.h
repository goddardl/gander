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
#ifndef __GANDER_FLAGSET__
#define __GANDER_FLAGSET__

#include "Gander/Common.h"
#include "Gander/Flags.h"

namespace Gander
{

#ifdef foreach
#undef foreach
#endif
#define foreach(VAR, SET) \
	for (VAR = SET.first(); VAR; VAR = SET.next(VAR))

/// The FlagSet class is a container for the Flag class.
template< class T, class FlagSetInitEnum, typename Flag >
class FlagSet
{

public :

	FlagSet() : m_mask(0) {}
	FlagSet( const FlagSet &source ) { m_mask = source.m_mask; }
	FlagSet( FlagSetInitEnum v ) : m_mask( v ) { }
	FlagSet( Flag v ) { *this = v; }

	// Why doesn't this friend operator work? 	
	inline friend FlagSet operator | ( const FlagSetInitEnum &a, const FlagSetInitEnum &b )
	{
		std::cerr << "|" << std::endl;
		return FlagSet(a  );
	}

	const FlagSet &operator = ( const FlagSet& source )
	{
		m_mask = source.m_mask; return *this;
	}

	const FlagSet &operator = ( FlagSetInitEnum source ) 
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
	bool operator == ( FlagSetInitEnum v ) const { return m_mask == T(v); }
	bool operator != ( FlagSetInitEnum v ) const { return !(*this == v); }
	bool operator == ( Flag z ) const { return m_mask == T(1) << ( T(z) - 1 ); }
	bool operator != ( Flag z) const { return !(*this == z); }
	void operator += ( const FlagSet &source ) { m_mask |= source.m_mask; }
	void operator += ( FlagSetInitEnum source ) { m_mask |= source; }
	void operator += ( Flag z ) { m_mask |= T(1) << ( z.value() - 1 ); }
	void insert( Flag z ) { *this += z; }
	void insert( const Flag* array, int n ) { for( int i = 0; i < n; ++i ) *this += array[i]; }
	void operator -= ( const FlagSet &source ) { m_mask &= ~source.m_mask; }
	void operator -= ( FlagSetInitEnum source ) { m_mask &= ~source; }
	void operator -= ( Flag z ) { m_mask &= ~z.value(); };
	void erase( Flag z ) { *this -= z; }
	void erase( const Flag* array, int n ) { for( int i = 0; i < n; ++i ) *this -= array[i]; }
	void operator &= ( const FlagSet &source ) { m_mask &= source.m_mask; }
	void operator &= ( FlagSetInitEnum source ) { m_mask &= source; }
	void operator &= ( Flag z ) { m_mask &= T(1) << ( T(z) - 1 ); }
	bool operator & ( const FlagSet &k ) const { return ( m_mask & ( T(1) << ( T( k ) - 1 ) ) ) != 0; }
	bool operator & ( FlagSetInitEnum k ) const { return ( m_mask & k ) != 0; }
	bool operator & ( Flag k ) const { return ( m_mask & ( T(1) << ( T( k ) - 1 ) ) ) != 0; }
	unsigned size( Flag k ) const { return *this & k ? 1 : 0; }
	bool contains( const FlagSet &source ) const { return ( ( source.m_mask & m_mask ) == source.m_mask ); }
	bool contains( FlagSetInitEnum source ) const { return !( ~m_mask & source ); }
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

}; // Gander

#endif // __GANDER_FLAGSET__

