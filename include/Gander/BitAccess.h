#ifndef __BITACCESS__
#define __BITACCESS__

#include <iostream>
#include <math.h>
#include "constants.h"

namespace LG
{
	
namespace Core
{
	
// Forward declarations
template < class T, unsigned WIDTH  >
class SubByte
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
		inline SubByte(T& data, const unsigned char idx):
			m_data( data ),
			m_idx( idx )
		{}
		
		inline SubByte operator = ( const T& i )
		{
			const int shift = shift_();
			const T mask = mask_();
			m_data = ((m_data & ~(mask << shift))|((i & mask) << shift));
			return *this;
		}
		
		inline friend std::ostream &operator<< ( std::ostream &out, const SubByte<T, WIDTH> &t )
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

template < class T, unsigned WIDTH >
class BitTwiddler
{
	public:
		T& m_data;
		
	public:
		inline BitTwiddler( T& data ):
			m_data( data )
		{}
		
		inline SubByte<T, WIDTH> operator [] ( const unsigned char idx )
		{
			return SubByte<T, WIDTH>(m_data, idx);
		}
		
		inline friend std::ostream &operator<< ( std::ostream &out, const BitTwiddler<T, WIDTH> &t )
		{
			for (char i = WIDTH-1; i >= 0; i--)
			{
				out << static_cast<bool>((t.m_data >> i) & 0x1);
			}
			
			return out << " (" << static_cast<unsigned int>(t.m_data) << ") ";
		}
};

template < class T, unsigned WIDTH, unsigned ARRAY_LEN >
class UnalignedArraySubByte
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
		inline UnalignedArraySubByte(T* data, const unsigned char idx):
			m_data( data ),
			m_idx( idx )
		{}
		
		inline friend std::ostream &operator<< ( std::ostream &out, const UnalignedArraySubByte<T, WIDTH, ARRAY_LEN> &t )
		{
			T data = T(t);
			for (char i = WIDTH-1; i >= 0; i--)
			{
				out << static_cast<bool>((data >> i) & 0x1);
			}
			
			return out << " (" << static_cast<unsigned int>(data)<< ")" ;
		}

		inline UnalignedArraySubByte operator = ( const T& data )
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
	
		inline UnalignedArraySubByte<T, WIDTH, ARRAY_LEN> operator [] ( const unsigned char idx )
		{
			return UnalignedArraySubByte< T, WIDTH, ARRAY_LEN >( m_data, idx );
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
	
		inline SubByte<T, WIDTH> operator [] ( const unsigned char idx )
		{
			const unsigned int offsetIdx = idx/((sizeof(T)*8)/WIDTH);
			std::cerr << "idx " << (int)idx << " " << offsetIdx << " " << idx/((sizeof(T)*8)/WIDTH) << std::endl;
			return SubByte< T, WIDTH >( m_data[offsetIdx], idx-offsetIdx*((sizeof(T)*8)/WIDTH) );
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
	typedef AlignedBitArray< U8, 1, DEPTH > BitArray1;
	typedef AlignedBitArray< U8, 2, DEPTH > BitArray2;
	typedef UnalignedBitArray< U8, 3, DEPTH > BitArray3;
	typedef AlignedBitArray< U8, 4, DEPTH > BitArray4;
	typedef UnalignedBitArray< U8, 5, DEPTH > BitArray5;
	typedef UnalignedBitArray< U8, 6, DEPTH > BitArray6;
	typedef UnalignedBitArray< U8, 7, DEPTH > BitArray7;
	typedef UnalignedBitArray< U16, 9, DEPTH > BitArray9;
	typedef UnalignedBitArray< U16, 10, DEPTH > BitArray10;
	typedef UnalignedBitArray< U16, 11, DEPTH > BitArray11;
	typedef UnalignedBitArray< U16, 12, DEPTH > BitArray12;
	typedef UnalignedBitArray< U16, 13, DEPTH > BitArray13;
	typedef UnalignedBitArray< U16, 14, DEPTH > BitArray14;
	typedef UnalignedBitArray< U16, 15, DEPTH > BitArray15;
	typedef UnalignedBitArray< U32, 17, DEPTH > BitArray17;
	typedef UnalignedBitArray< U32, 18, DEPTH > BitArray18;
	typedef UnalignedBitArray< U32, 19, DEPTH > BitArray19;
	typedef UnalignedBitArray< U32, 20, DEPTH > BitArray20;
	typedef UnalignedBitArray< U32, 21, DEPTH > BitArray21;
	typedef UnalignedBitArray< U32, 22, DEPTH > BitArray22;
	typedef UnalignedBitArray< U32, 23, DEPTH > BitArray23;
	typedef UnalignedBitArray< U32, 24, DEPTH > BitArray24;
	typedef UnalignedBitArray< U32, 25, DEPTH > BitArray25;
	typedef UnalignedBitArray< U32, 26, DEPTH > BitArray26;
	typedef UnalignedBitArray< U32, 27, DEPTH > BitArray27;
	typedef UnalignedBitArray< U32, 28, DEPTH > BitArray28;
	typedef UnalignedBitArray< U32, 29, DEPTH > BitArray29;
	typedef UnalignedBitArray< U32, 30, DEPTH > BitArray30;
	typedef UnalignedBitArray< U32, 31, DEPTH > BitArray31;
	typedef UnalignedBitArray< U64, 33, DEPTH > BitArray33;
	typedef UnalignedBitArray< U64, 34, DEPTH > BitArray34;
	typedef UnalignedBitArray< U64, 35, DEPTH > BitArray35;
	typedef UnalignedBitArray< U64, 36, DEPTH > BitArray36;
	typedef UnalignedBitArray< U64, 37, DEPTH > BitArray37;
	typedef UnalignedBitArray< U64, 38, DEPTH > BitArray38;
	typedef UnalignedBitArray< U64, 39, DEPTH > BitArray39;
	typedef UnalignedBitArray< U64, 40, DEPTH > BitArray40;
	typedef UnalignedBitArray< U64, 41, DEPTH > BitArray41;
	typedef UnalignedBitArray< U64, 42, DEPTH > BitArray42;
	typedef UnalignedBitArray< U64, 43, DEPTH > BitArray43;
	typedef UnalignedBitArray< U64, 44, DEPTH > BitArray44;
	typedef UnalignedBitArray< U64, 45, DEPTH > BitArray45;
	typedef UnalignedBitArray< U64, 46, DEPTH > BitArray46;
	typedef UnalignedBitArray< U64, 47, DEPTH > BitArray47;
	typedef UnalignedBitArray< U64, 48, DEPTH > BitArray48;
	typedef UnalignedBitArray< U64, 49, DEPTH > BitArray49;
	typedef UnalignedBitArray< U64, 50, DEPTH > BitArray50;
	typedef UnalignedBitArray< U64, 51, DEPTH > BitArray51;
	typedef UnalignedBitArray< U64, 52, DEPTH > BitArray52;
	typedef UnalignedBitArray< U64, 53, DEPTH > BitArray53;
	typedef UnalignedBitArray< U64, 54, DEPTH > BitArray54;
	typedef UnalignedBitArray< U64, 55, DEPTH > BitArray55;
	typedef UnalignedBitArray< U64, 56, DEPTH > BitArray56;
	typedef UnalignedBitArray< U64, 57, DEPTH > BitArray57;
	typedef UnalignedBitArray< U64, 58, DEPTH > BitArray58;
	typedef UnalignedBitArray< U64, 59, DEPTH > BitArray59;
	typedef UnalignedBitArray< U64, 60, DEPTH > BitArray60;
	typedef UnalignedBitArray< U64, 61, DEPTH > BitArray61;
	typedef UnalignedBitArray< U64, 62, DEPTH > BitArray62;
	typedef UnalignedBitArray< U64, 63, DEPTH > BitArray63;
};

};

};

#endif
