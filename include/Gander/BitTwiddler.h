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
#ifndef __GANDER_BITTWIDDLER__
#define __GANDER_BITTWIDDLER__

#include <iostream>
#include <math.h>

#include "Gander/Common.h"

namespace Gander 
{

template < class T, unsigned WIDTH, class ELEMENT_TYPE = int8u >
class Bits
{
	private :

		enum
		{
			Mask = ~( ( ~ELEMENT_TYPE( 0x0 ) ) << WIDTH ),
		};
	
	public :

		inline Bits( T &data, int8u idx ) :
			m_data( data ),
			m_idx( idx )
		{}
		
		inline Bits operator = ( const ELEMENT_TYPE &i )
		{
			const int s = shift();
			m_data = ( ( m_data & ( ~( T( Mask ) << s ) ) ) | ( T( i & Mask ) << s ) );
			return *this;
		}
		
		inline friend std::ostream &operator << ( std::ostream &out, const Bits<T, WIDTH, ELEMENT_TYPE> &t )
		{
			const int s = t.shift();
			for( int8 i = WIDTH-1; i >= 0; --i )
			{
				out << static_cast<bool>( ( t.m_data >> ( s + i ) ) & 0x1 );
			}
			
			return out << " (" << ( static_cast<int>( ( t.m_data >> s ) & t.Mask ) ) << ")";
		}
		
		inline operator ELEMENT_TYPE() const
		{
			return static_cast<ELEMENT_TYPE>( ( m_data >> shift() ) ) & Mask;
		}

	private :

		inline const int shift() const
		{
			return WIDTH * int( m_idx );
		}
	
		T& m_data;
		const int8u m_idx;
};

/// A utility class for manipulating individual bits of data.	
template < class T, unsigned WIDTH, class ELEMENT_TYPE = int8u >
class BitTwiddler
{

	public :
		
		/// Constructs a BitTwiddler with the data that it will be manipulating.
		inline BitTwiddler( T& data ) :
			m_data( data )
		{
		}
		
		/// Returns a class that can manipulate a set of WIDTH consecutive bits.
		inline Bits<T, WIDTH, ELEMENT_TYPE> operator [] ( int8u idx )
		{
			return Bits<T, WIDTH, ELEMENT_TYPE>( m_data, idx );
		}
	
		/// Inserts a data element at the specified index.	
		inline void insert( int8u index, ELEMENT_TYPE element )
		{		
			const int16u i = WIDTH * index;
			const T mask = ( ~T(0) ) << i;
			m_data = ( ( m_data & mask ) << WIDTH ) | ( ( ~mask ) & m_data ) | ( T( element & ( ~( ( ~ELEMENT_TYPE(0) ) << WIDTH ) ) ) << i );
		}

		/// Erases a data element at the specified index.	
		inline void erase( int8u index )
		{
			const int16u i = WIDTH * index;
			const T maskTop = ( ( ~T(0) ) << ( i + WIDTH ) );
			const T maskBottom = ~( ( ~T(0) ) << i );
			m_data = ( ( m_data & maskTop ) >> WIDTH ) | ( maskBottom & m_data );
		}
		
		/// Returns the maximum number of elements that this data type can be split up into.
		unsigned int size() const
		{
			return ( sizeof(T) * 8 ) / WIDTH;
		}

		/// Print operator that outputs the data as a binary string followed by the decimal value.
		inline friend std::ostream &operator << ( std::ostream &out, const BitTwiddler<T, WIDTH, ELEMENT_TYPE> &t )
		{
			for( int8 i = sizeof( T ) * 8 - 1; i >= 0; --i )
			{
				out << static_cast<bool>( ( t.m_data >> i ) & 0x1 );
			}
			
			return out << " (" << static_cast<int64u>( t.m_data ) << ") ";
		}
	
	private :
		
		T& m_data;
};

};

#endif
