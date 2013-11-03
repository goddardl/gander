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

#define GANDER_DEFINE_FLAGSET( DATA_TYPE, FLAG_ENUM, FLAGMASK_ENUM, FLAG_NAME, FLAGSET_NAME )\
	typedef Gander::FlagSet<DATA_TYPE, FLAG_ENUM, 9, FLAGMASK_ENUM>::Flag FLAG_NAME;\
	typedef Gander::FlagSet<DATA_TYPE, FLAG_ENUM, 9, FLAGMASK_ENUM> FLAGSET_NAME;\
	inline FLAGMASK_ENUM operator | ( const FLAG_ENUM a, const FLAG_ENUM b )\
	{\
		return static_cast<FLAGMASK_ENUM>( FLAGSET_NAME( a ).value() | FLAGSET_NAME( b ).value() );\
	}\
	inline FLAGMASK_ENUM operator | ( const FLAGMASK_ENUM a, const FLAGMASK_ENUM b )\
	{\
		return static_cast<FLAGMASK_ENUM>( static_cast<DATA_TYPE>( a ) | static_cast<DATA_TYPE>( b ) );\
	}\
	inline FLAGMASK_ENUM operator | ( const FLAG_ENUM a, const FLAGMASK_ENUM b )\
	{\
		return static_cast<FLAGMASK_ENUM>( static_cast<DATA_TYPE>( b ) | FLAGSET_NAME( a ).value() );\
	}\
	inline FLAGMASK_ENUM operator | ( const FLAGMASK_ENUM a, const FLAG_ENUM b )\
	{\
		return static_cast<FLAGMASK_ENUM>( static_cast<DATA_TYPE>( a ) | FLAGSET_NAME( b ).value() );\
	}\
	inline FLAGMASK_ENUM operator & ( const FLAGMASK_ENUM a, const FLAGMASK_ENUM b )\
	{\
		return static_cast<FLAGMASK_ENUM>( static_cast<DATA_TYPE>( a ) & static_cast<DATA_TYPE>( b ) );\
	}\
	inline FLAGMASK_ENUM operator & ( const FLAGMASK_ENUM a, const FLAG_ENUM b )\
	{\
		return ( a ? static_cast<FLAGMASK_ENUM>( static_cast<DATA_TYPE>( a ) & FLAGSET_NAME( b ).value() ) : static_cast<FLAGMASK_ENUM>( 0 ) );\
	}\
	inline FLAGMASK_ENUM operator & ( const FLAG_ENUM a, const FLAGMASK_ENUM b )\
	{\
		return ( b ? static_cast<FLAGMASK_ENUM>( static_cast<DATA_TYPE>( b ) & FLAGSET_NAME( a ).value() ) : static_cast<FLAGMASK_ENUM>( 0 ) );\
	}\
	std::ostream &operator << ( std::ostream &out, const FLAGMASK_ENUM &a )\
	{\
		FLAGSET_NAME::Flag v;\
		FLAGSET_NAME set( a );\
		foreach( v, set )\
		{\
			out << FLAGSET_NAME::name( v ) << " ";\
		}\
		return out;\
	}\
	std::ostream &operator << ( std::ostream &out, const FLAG_ENUM &a )\
	{\
		out << FLAGSET_NAME::name( a );\
	}
	#ifdef foreach
	#undef foreach
	#endif
	#define foreach(VAR, SET) \
		for (VAR = SET.first(); VAR; VAR = SET.next(VAR))

/// The FlagSet class is a container for a bit mask.
/// It provides operators for appending and removing flags as well as other flagSet classes.
/// Use the GANDER_DEFINE_FLAGSET macro to define one. 
/// For each definition of a FlagSet class, two static members need to be defined:
namespace Gander
{

template<class T, class FlagType,  unsigned nDefaultFlags, class FlagMaskType>
class FlagSet
{
	/// An STL style const_iterator class.
	class Iterator
	{
		public :

		Iterator( const Iterator &rhs ) : m_set( rhs.m_set ), m_flag( rhs.m_flag ) { }

		inline FlagType operator * () const { return m_flag; }
		inline Iterator &operator ++ () { m_flag = m_set.next( m_flag ); return *this; }
		inline Iterator operator ++ ( int )
		{
			Iterator tmp( *this );
			operator++();
			return tmp;
		}
		inline Iterator &operator -- () { m_flag = m_set.previous( m_flag ); return *this; }
		inline Iterator operator -- ( int )
		{
			Iterator tmp( *this );
			operator--();
			return tmp;
		}
		inline Iterator operator - ( int offset ) const
		{
			Iterator tmp( *this );
			int size = m_set.size();
			for( int i = 0; i < offset; ++i )
			{
				--tmp;
				if( *tmp == T(0) )
				{
					return m_set.begin();
				}
			}
			return tmp;
		}
		inline Iterator operator + ( int offset ) const
		{
			Iterator tmp( *this );
			int size = m_set.size();
			for( int i = 0; i < offset && i < size; ++i )
			{
				++tmp;
			}
			return tmp;
		}
		inline bool operator == ( const Iterator &rhs ) const
		{
			return m_set == rhs.m_set && m_flag == rhs.m_flag;
		}
		inline bool operator != ( const Iterator &rhs ) const
		{
			return m_set != rhs.m_set || m_flag != rhs.m_flag;
		}

		private :

		Iterator( const FlagSet &set, FlagType flag ) :
			m_set( set ),
			m_flag( flag )
		{
		}

		friend class Gander::FlagSet< T, FlagType, nDefaultFlags, FlagMaskType>;
		FlagSet m_set;
		FlagType m_flag;
	};
	
	public :

	typedef FlagType Flag;
	typedef FlagMaskType FlagMask;
	typedef FlagSet<T, FlagType, nDefaultFlags, FlagMaskType>::Iterator const_iterator;

	//! @name Constructors
	/// Various constructors for creating FlagSets from FlagType and
	/// FlagMask enums and other FlagSets.
	//////////////////////////////////////////////////////////////
	//@{
	inline FlagSet() : m_mask(0) {}
	inline FlagSet( const FlagSet &source ) { m_mask = source.m_mask; }
	inline FlagSet( FlagMask v ) : m_mask( v ) { }
	inline FlagSet( Flag v ) { *this = v; }
	inline void erase( Flag v ) { *this -= v; }
	inline void erase( FlagSet v ) { *this -= v; }
	inline void erase( FlagMask v ) { *this -= v; }
	//@}

	//! @name Assignment Operators
	//////////////////////////////////////////////////////////////
	//@{
	inline const FlagSet &operator = ( const FlagSet& source )
	{
		m_mask = source.m_mask; return *this;
	}
	inline const FlagSet &operator = ( FlagMask source ) 
	{
		m_mask = source;
		return *this;
	}
	inline const FlagSet &operator = ( Flag z )
	{
		m_mask = T(1) << ( z - 1 );
		return *this;
	}
	//@}

	//! @name Accessors
	/// Various accessors for setting and querying the contents of
	/// the FlagSet.
	//////////////////////////////////////////////////////////////
	//@{
	inline void clear() { m_mask = 0; }
	inline bool empty() const { return !m_mask; }
	inline T value() const { return m_mask; }
	inline bool contains( const FlagSet &v ) const { return ( ( v.m_mask & m_mask ) == v.m_mask ); }
	inline bool contains( const FlagMask &v ) const { return !( ~m_mask & v ); }
	inline bool contains( const Flag &v ) const {
		return ( ( T(1) << ( static_cast<T>(v) - T(1) ) ) & m_mask ) == (  T(1) << ( static_cast<T>(v) - T(1) ) );
	}
	unsigned short size() const
	{
		T total = 0;
		for( unsigned short i = 0; i < ( sizeof(T) << 3 ); ++i )
		{
			total += ( ( ( m_mask >> i ) & T(1)) != T(0) );
		}
		return total;
	}
	//@}

	//! @name Equality Operators
	//////////////////////////////////////////////////////////////
	//@{
	inline bool operator == ( const FlagSet &v ) const { return m_mask == v.m_mask; }
	inline bool operator == ( const FlagMask &v ) const { return m_mask == static_cast<T>(v); }
	inline bool operator == ( const Flag &v ) const { return m_mask == T(1) << ( static_cast<T>(v) - 1 ); }
	inline bool operator != ( const FlagSet &v ) const { return !(*this == v); }
	inline bool operator != ( const FlagMask &v ) const { return !(*this == v); }
	inline bool operator != ( const Flag &v ) const { return !(*this == v); }
	inline bool operator < ( const FlagSet &v ) const { return (*this).size() < v.size(); }
	inline bool operator > ( const FlagSet &v ) const { return (*this).size() > v.size(); }
	inline bool operator <= ( const FlagSet &v ) const { return (*this).size() <= v.size(); }
	inline bool operator >= ( const FlagSet &v ) const { return (*this).size() >= v.size(); }
	//@}

	//! @name Arithmetic Operators
	//////////////////////////////////////////////////////////////
	//@{
	inline void operator += ( const FlagSet &v ) { m_mask |= v.m_mask; }
	inline void operator += ( const FlagMask &v ) { m_mask |= static_cast<T>(v); }
	inline void operator += ( const Flag &v ) { m_mask |= T(1) << ( static_cast<T>(v) - T(1) ); }
	inline void operator -= ( const FlagSet &v ) { m_mask &= (~v.m_mask); }
	inline void operator -= ( const FlagMask &v ) { m_mask &= (~static_cast<T>(v)); }
	inline void operator -= ( const Flag &v ) { m_mask &= ~(T(1) << ( static_cast<T>(v) - T(1) )); };
	inline void operator &= ( const FlagSet &v ) { m_mask &= v.m_mask; }
	inline void operator &= ( const FlagMask &v ) { m_mask &= static_cast<T>(v); }
	inline void operator &= ( const Flag &v ) { m_mask &= T(1) << ( static_cast<T>(v) - T(1) ); }
	inline void operator |= ( const FlagSet &v ) { m_mask |= v.m_mask; }
	inline void operator |= ( const FlagMask &v ) { m_mask |= static_cast<T>(v); }
	inline void operator |= ( const Flag &v ) { m_mask |= T(1) << ( static_cast<T>(v) - T(1) ); }
	template< class Type >
	inline FlagSet operator + ( Type z ) const
	{
		FlagSet tmp = *this;
		tmp += z;
		return tmp;
	}
	template< class Type >
	inline FlagSet operator - ( Type z ) const
	{
		FlagSet tmp = *this;
		tmp -= z;
		return tmp;
	}
	//@}

	//! @name STL-style Iterators and methods.
	//////////////////////////////////////////////////////////////
	//@{
	operator bool() const { return m_mask != 0; }
	Iterator begin() const { return Iterator( *this, this->first() ); }
	Iterator end() const { return Iterator( *this, static_cast<FlagType>(0) ); }
	void erase( Iterator &it ) { Flag v = it.m_set.previous( it.m_flag ); *this -= *it; it = Iterator( *this, v ); }
	//@}

	/// Return the intersection of this FlagSet with another as a FlagSet.
	template< class Type >
	FlagSet intersection( Type v ) const
	{
		FlagSet tmp = *this;
		tmp &= v;
		return tmp;
	}

	Flag first() const
	{
		if( m_mask == 0 )
		{
			return static_cast<Flag>(0);
		}

		T i = 0;
		for( ; i < ( static_cast<T>( sizeof(T) ) << T(3) ) && ( ( m_mask >> i ) & T(1) ) == T(0); ++i );
		return Flag( i+T(1) );
	}

	Flag next( const Flag &v ) const
	{
		if ( m_mask >> static_cast<T>( v ) == T(0) )
		{
			return Flag( T(0) );
		}

		T i = T(v);
		for( ; i < ( sizeof(T) << T(3) ) && ( ( m_mask >> i ) & T(1) ) == T(0); ++i );
		return Flag( i+T(1) );
	}

	Flag last() const
	{
		if( empty() )
		{
			return static_cast<Flag>(0);
		}

		T i = ( sizeof( T ) << T(3) ) - T(1);
		for( ; i >= T(0) && ( ( m_mask >> i ) & T(1) ) == T(0); --i );
		return Flag( i + T(1) );
	}

	Flag previous( Flag v ) const
	{
		T i = static_cast<T>(v);
		if( i == T(0) )
		{
			return last();
		}

		for( ; ( ( m_mask >> (i-1) ) & T(1) ) == T(0); i-- );

		if( ( m_mask & ( T(1) << ( i - 2 ) ) ) == T(0) )
		{
			return static_cast<Flag>( 0 );
		}
	
		return static_cast<Flag>( i-1 );
	}

	friend std::ostream & operator << ( std::ostream &out, FlagSet &set )
	{
		Flag v;
		foreach( v, set )
		{
			out << name( v ) << " ";    
		}
		return out;
	}

	/// Get the flag's name.
	/// Returns the name of the flag by looking it up from the static g_flagMappings member.
	/// @param name The flag.
	/// @return Returns a character string with the name of the flag v.
	static const char *name( Flag v );
	/// Returns an existing or new flag by looking up a name.
	/// Returns a flag by looking up a name in the static g_flagMappings member. If the flag doesn't
	/// exist then a new mapping for the name will be created and the new Flag returned.
	/// @param name The flag name.
	/// @return Returns the flag which matches the name.
	static inline Flag flag( const char *name );
	/// Returns an existing flag by looking up a name.
	/// Returns a flag by looking up the name in the static g_flagMappings member. If the flag doesn't exist
	/// then a NULL flag will be returned (with a m_value of 0).
	/// @param name The name of the flag.
	/// @return Returns the flag which matches the name.
	static inline Flag findFlag( const char *name );

	private :
	
	static const char *g_defaultFlags[ nDefaultFlags ];
	static std::vector<const char*> g_flagMappings;

	T m_mask;
};

}; // namespace Gander

#include "Gander/Flags.inl"

#endif

