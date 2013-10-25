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
#ifndef __GANDER_BITARRAY__
#define __GANDER_BITARRAY__

#include <iostream>
#include <math.h>

#include "Gander/Common.h"
#include "Gander/BitTwiddler.h"

namespace Gander 
{

template < class T, unsigned WIDTH, unsigned ARRAY_LEN >
class UnalignedArrayBits
{
	T *m_data;
	const unsigned char m_idx;
	
	private:
		inline T mask_() const
		{
			return static_cast<T>(~((~T(0)) << WIDTH));
		}
		
		inline const int shift_() const
		{
			return WIDTH*(int)m_idx;
		}
	
	public:
		inline UnalignedArrayBits(T* data, const unsigned char idx):
			m_data( data ),
			m_idx( idx )
		{}
		
		inline friend std::ostream &operator<< ( std::ostream &out, const UnalignedArrayBits<T, WIDTH, ARRAY_LEN> &t )
		{
			T data = T(t);
			for (char i = WIDTH-1; i >= 0; i--)
			{
				out << static_cast<bool>((data >> i) & 0x1);
			}
			
			return out << " (" << static_cast<unsigned int>(data)<< ")" ;
		}

		inline UnalignedArrayBits operator = ( const T& data )
		{
			int shift = shift_();
			T mask = mask_();
			
			unsigned int startIdx = shift/(sizeof(T)*8);
			shift -= startIdx * (sizeof(T)*8);
			T partMask = mask << shift;
			
			m_data[startIdx] = (m_data[startIdx] & (~partMask)) | ((data << shift) & partMask);
			
			if (shift+WIDTH > (sizeof(T)*8) )
			{
				unsigned int endIdx = startIdx+1;
				shift = (sizeof(T)*8)-shift;
				partMask = mask >> shift;
				m_data[endIdx] = (m_data[endIdx] & (~partMask)) | ((data >> shift) & partMask);
			}
				
			return *this;
		};
		
		inline operator T() const 
		{ 
			int shift = shift_();
			T mask = mask_();
			T data = 0;
			
			unsigned int startIdx = shift/(sizeof(T)*8);
			shift -= startIdx * (sizeof(T)*8);
			T partMask = mask << shift;
			
			data |= (m_data[startIdx] & partMask) >> shift;
				
			if (shift+WIDTH > (sizeof(T)*8) )
			{
				unsigned int endIdx = startIdx+1;
				shift = (sizeof(T)*8)-shift;
				partMask = mask >> shift;
				data |= (m_data[endIdx] & partMask) << shift;
			}
			
			return static_cast<T>(data);
		};
};

template < class T, unsigned WIDTH, unsigned DEPTH >
class UnalignedBitArray
{
	#define ARRAY_LEN ((WIDTH*DEPTH)/(sizeof(T)*8)+1)
	private:
		T m_data[ARRAY_LEN];
		
	public:
		UnalignedBitArray()
		{
			memset(m_data, 0, ARRAY_LEN*sizeof(T));
		}
	
		inline UnalignedArrayBits<T, WIDTH, ARRAY_LEN> operator [] ( const unsigned char idx )
		{
			return UnalignedArrayBits< T, WIDTH, ARRAY_LEN >( m_data, idx );
		}
};

template < class T, unsigned WIDTH, unsigned DEPTH >
class AlignedBitArray
{
	#define ARRAY_LEN ((WIDTH*DEPTH)/(sizeof(T)*8)+1)
	private:
		T m_data[ARRAY_LEN];
		
	public:
		AlignedBitArray()
		{
			memset(m_data, 0, ARRAY_LEN*sizeof(T));
		}
	
		inline Bits<T, WIDTH> operator [] ( const unsigned char idx )
		{
			const unsigned int offsetIdx = idx/((sizeof(T)*8)/WIDTH);
			std::cerr << "idx " << (int)idx << " " << offsetIdx << " " << idx/((sizeof(T)*8)/WIDTH) << std::endl;
			return Bits< T, WIDTH >( m_data[offsetIdx], idx-offsetIdx*((sizeof(T)*8)/WIDTH) );
		}
};

/// A class to proide arrays of odd numbers of bits.
///
/// The BitArrayX class facilitates the construction arrays with words of any arbitrary bit width.
/// The BitArrayX is a set of convenience typedefs which use the AlignedBitArray (a class for creating arrays of words which are of a power of 2) and
/// the UnalignedBitArray class (for creating all other arrays of arbitrary bit widths). They behave in a similar way to traditional arrays.
/// For example:
///\code
/// LG::Core::BitArray<16>::BitArray4 array;                 // Creates an array of 16 words of 4 bits wide.
/// array[3] = 3;                                            // Sets the fourth 4-bit word to 3.
///\endcode
template < unsigned DEPTH >
struct BitArray
{
	typedef AlignedBitArray< int8u, 1, DEPTH > BitArray1;
	typedef AlignedBitArray< int8u, 2, DEPTH > BitArray2;
	typedef UnalignedBitArray< int8u, 3, DEPTH > BitArray3;
	typedef AlignedBitArray< int8u, 4, DEPTH > BitArray4;
	typedef UnalignedBitArray< int8u, 5, DEPTH > BitArray5;
	typedef UnalignedBitArray< int8u, 6, DEPTH > BitArray6;
	typedef UnalignedBitArray< int8u, 7, DEPTH > BitArray7;
	typedef UnalignedBitArray< int16u, 9, DEPTH > BitArray9;
	typedef UnalignedBitArray< int16u, 10, DEPTH > BitArray10;
	typedef UnalignedBitArray< int16u, 11, DEPTH > BitArray11;
	typedef UnalignedBitArray< int16u, 12, DEPTH > BitArray12;
	typedef UnalignedBitArray< int16u, 13, DEPTH > BitArray13;
	typedef UnalignedBitArray< int16u, 14, DEPTH > BitArray14;
	typedef UnalignedBitArray< int16u, 15, DEPTH > BitArray15;
	typedef UnalignedBitArray< int32u, 17, DEPTH > BitArray17;
	typedef UnalignedBitArray< int32u, 18, DEPTH > BitArray18;
	typedef UnalignedBitArray< int32u, 19, DEPTH > BitArray19;
	typedef UnalignedBitArray< int32u, 20, DEPTH > BitArray20;
	typedef UnalignedBitArray< int32u, 21, DEPTH > BitArray21;
	typedef UnalignedBitArray< int32u, 22, DEPTH > BitArray22;
	typedef UnalignedBitArray< int32u, 23, DEPTH > BitArray23;
	typedef UnalignedBitArray< int32u, 24, DEPTH > BitArray24;
	typedef UnalignedBitArray< int32u, 25, DEPTH > BitArray25;
	typedef UnalignedBitArray< int32u, 26, DEPTH > BitArray26;
	typedef UnalignedBitArray< int32u, 27, DEPTH > BitArray27;
	typedef UnalignedBitArray< int32u, 28, DEPTH > BitArray28;
	typedef UnalignedBitArray< int32u, 29, DEPTH > BitArray29;
	typedef UnalignedBitArray< int32u, 30, DEPTH > BitArray30;
	typedef UnalignedBitArray< int32u, 31, DEPTH > BitArray31;
	typedef UnalignedBitArray< int64u, 33, DEPTH > BitArray33;
	typedef UnalignedBitArray< int64u, 34, DEPTH > BitArray34;
	typedef UnalignedBitArray< int64u, 35, DEPTH > BitArray35;
	typedef UnalignedBitArray< int64u, 36, DEPTH > BitArray36;
	typedef UnalignedBitArray< int64u, 37, DEPTH > BitArray37;
	typedef UnalignedBitArray< int64u, 38, DEPTH > BitArray38;
	typedef UnalignedBitArray< int64u, 39, DEPTH > BitArray39;
	typedef UnalignedBitArray< int64u, 40, DEPTH > BitArray40;
	typedef UnalignedBitArray< int64u, 41, DEPTH > BitArray41;
	typedef UnalignedBitArray< int64u, 42, DEPTH > BitArray42;
	typedef UnalignedBitArray< int64u, 43, DEPTH > BitArray43;
	typedef UnalignedBitArray< int64u, 44, DEPTH > BitArray44;
	typedef UnalignedBitArray< int64u, 45, DEPTH > BitArray45;
	typedef UnalignedBitArray< int64u, 46, DEPTH > BitArray46;
	typedef UnalignedBitArray< int64u, 47, DEPTH > BitArray47;
	typedef UnalignedBitArray< int64u, 48, DEPTH > BitArray48;
	typedef UnalignedBitArray< int64u, 49, DEPTH > BitArray49;
	typedef UnalignedBitArray< int64u, 50, DEPTH > BitArray50;
	typedef UnalignedBitArray< int64u, 51, DEPTH > BitArray51;
	typedef UnalignedBitArray< int64u, 52, DEPTH > BitArray52;
	typedef UnalignedBitArray< int64u, 53, DEPTH > BitArray53;
	typedef UnalignedBitArray< int64u, 54, DEPTH > BitArray54;
	typedef UnalignedBitArray< int64u, 55, DEPTH > BitArray55;
	typedef UnalignedBitArray< int64u, 56, DEPTH > BitArray56;
	typedef UnalignedBitArray< int64u, 57, DEPTH > BitArray57;
	typedef UnalignedBitArray< int64u, 58, DEPTH > BitArray58;
	typedef UnalignedBitArray< int64u, 59, DEPTH > BitArray59;
	typedef UnalignedBitArray< int64u, 60, DEPTH > BitArray60;
	typedef UnalignedBitArray< int64u, 61, DEPTH > BitArray61;
	typedef UnalignedBitArray< int64u, 62, DEPTH > BitArray62;
	typedef UnalignedBitArray< int64u, 63, DEPTH > BitArray63;
};

}; // namespace Gander

#endif
