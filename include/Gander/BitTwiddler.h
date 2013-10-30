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

template < class T, unsigned WIDTH  >
class Bits
{
	T& m_data;
	const unsigned char m_idx;
	
	private:
		inline T mask_() const
		{
			return static_cast<T>(~((~static_cast<T>(0x0)) << WIDTH));
		}
		
		inline const int shift_() const
		{
			return WIDTH*(int)m_idx;
		}
	
	public:
		inline Bits(T& data, const unsigned char idx):
			m_data( data ),
			m_idx( idx )
		{}
		
		inline Bits operator = ( const T& i )
		{
			const int shift = shift_();
			const T mask = mask_();
			m_data = ((m_data & ~(mask << shift))|((i & mask) << shift));
			return *this;
		}
		
		inline friend std::ostream &operator<< ( std::ostream &out, const Bits<T, WIDTH> &t )
		{
			int shift = t.shift_();
			for (char i = WIDTH-1; i >= 0; i--)
			{
				out << static_cast<bool>((t.m_data >> (shift + i)) & 0x1);
			}
			
			return out << " (" << (static_cast<T>((t.m_data >> shift) & t.mask_()))<< ")" ;
		}
		
		inline operator T() const { return (static_cast<T>((m_data >> shift_()) & mask_())); }
};

/// A utility class for manipulating individual bits of data.	
template < class T, unsigned WIDTH >
class BitTwiddler
{
	public:
		
		/// Constructs a BitTwiddler with the data that it will be manipulating.
		inline BitTwiddler( T& data ):
			m_data( data )
		{}
		
		/// Returns a class that can manipulate a set of consecutive bits WIDTH wide.
		inline Bits<T, WIDTH> operator [] ( const int8u idx )
		{
			return Bits<T, WIDTH>( m_data, idx );
		}
	
		inline friend std::ostream &operator<< ( std::ostream &out, const BitTwiddler<T, WIDTH> &t )
		{
			for( int8u i = WIDTH-1; i >= 0; --i )
			{
				out << static_cast<bool>( ( t.m_data >> i ) & 0x1 );
			}
			
			return out << " (" << static_cast<int32u>(t.m_data) << ") ";
		}
	
	private:
		
		T& m_data;
};

};

#endif
