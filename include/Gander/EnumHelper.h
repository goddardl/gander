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
#ifndef __GANDER_ENUMHELPER_H__
#define __GANDER_ENUMHELPER_H__

#include "Gander/Common.h"

namespace Gander
{

/// Forward declaration of the test class. 
namespace Test { struct EnumHelperTest; }

/// A helpful struct that counts the number of set bits
/// in a value and sets the enum NumberOfSetBits to it.
template< EnumType M >
struct EnumHelper
{
	private :

		enum
		{
			c0 = ( M & ( 1 << 0 ) ) != 0,
			c1 = ( M & ( 1 << 1 ) ) != 0,
			c2 = ( M & ( 1 << 2 ) ) != 0,
			c3 = ( M & ( 1 << 3 ) ) != 0,
			c4 = ( M & ( 1 << 4 ) ) != 0,
			c5 = ( M & ( 1 << 5 ) ) != 0,
			c6 = ( M & ( 1 << 6 ) ) != 0,
			c7 = ( M & ( 1 << 7 ) ) != 0,
			c8 = ( M & ( 1 << 8 ) ) != 0,
			c9 = ( M & ( 1 << 9 ) ) != 0,
			c10 = ( M & ( 1 << 10 ) ) != 0,
			c11 = ( M & ( 1 << 11 ) ) != 0,
			c12 = ( M & ( 1 << 12 ) ) != 0,
			c13 = ( M & ( 1 << 13 ) ) != 0,
			c14 = ( M & ( 1 << 14 ) ) != 0,
			c15 = ( M & ( 1 << 15 ) ) != 0,
			c16 = ( M & ( 1 << 16 ) ) != 0,
			c17 = ( M & ( 1 << 17 ) ) != 0,
			c18 = ( M & ( 1 << 18 ) ) != 0,
			c19 = ( M & ( 1 << 19 ) ) != 0,
			c20 = ( M & ( 1 << 20 ) ) != 0,
			c21 = ( M & ( 1 << 21 ) ) != 0,
			c22 = ( M & ( 1 << 22 ) ) != 0,
			c23 = ( M & ( 1 << 23 ) ) != 0,
			c24 = ( M & ( 1 << 24 ) ) != 0,
			c25 = ( M & ( 1 << 25 ) ) != 0,
			c26 = ( M & ( 1 << 26 ) ) != 0,
			c27 = ( M & ( 1 << 27 ) ) != 0,
			c28 = ( M & ( 1 << 28 ) ) != 0,
			c29 = ( M & ( 1 << 29 ) ) != 0,
			c30 = ( M & ( 1 << 30 ) ) != 0,
			c31 = ( M & ( 1 << 31 ) ) != 0,
		};

	public :

		enum
		{
			MinimumRequiredBitWidth =
				M <= EnumType( 1 << 0 ) ? 1 :
				M < EnumType( 1 << 1 ) ? 1 :
				M < EnumType( 1 << 2 ) ? 2 :
				M < EnumType( 1 << 3 ) ? 3 :
				M < EnumType( 1 << 4 ) ? 4 :
				M < EnumType( 1 << 5 ) ? 5 :
				M < EnumType( 1 << 6 ) ? 6 :
				M < EnumType( 1 << 7 ) ? 7 :
				M < EnumType( 1 << 8 ) ? 8 :
				M < EnumType( 1 << 9 ) ? 9 :
				M < EnumType( 1 << 10 ) ? 10 :
				M < EnumType( 1 << 11 ) ? 11 :
				M < EnumType( 1 << 12 ) ? 12 :
				M < EnumType( 1 << 13 ) ? 13 :
				M < EnumType( 1 << 14 ) ? 14 :
				M < EnumType( 1 << 15 ) ? 15 :
				M < EnumType( 1 << 16 ) ? 16 :
				M < EnumType( 1 << 17 ) ? 17 :
				M < EnumType( 1 << 18 ) ? 18 :
				M < EnumType( 1 << 19 ) ? 19 :
				M < EnumType( 1 << 20 ) ? 20 :
				M < EnumType( 1 << 21 ) ? 21 :
				M < EnumType( 1 << 22 ) ? 22 :
				M < EnumType( 1 << 23 ) ? 23 :
				M < EnumType( 1 << 24 ) ? 24 :
				M < EnumType( 1 << 25 ) ? 25 :
				M < EnumType( 1 << 26 ) ? 26 :
				M < EnumType( 1 << 27 ) ? 27 :
				M < EnumType( 1 << 28 ) ? 28 :
				M < EnumType( 1 << 29 ) ? 39 :
				M < EnumType( 1 << 30 ) ? 30 :
				M < EnumType( 1 << 31 ) ? 31 :
				0,			

			NumberOfSetBits =
				c0 + c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + c9 +
				c10 + c11 + c12 + c13 + c14 + c15 + c16 + c17 + c18 + c19 +
				c20 + c21 + c22 + c23 + c24 + c25 + c26 + c27 + c28 + c29 +
				c30 + c31,

			LastSetBit = 
				( M & ( 1 << 31 ) ) != 0 ? 31 :
				( M & ( 1 << 30 ) ) != 0 ? 30 :
				( M & ( 1 << 29 ) ) != 0 ? 29 :
				( M & ( 1 << 28 ) ) != 0 ? 28 :
				( M & ( 1 << 27 ) ) != 0 ? 27 :
				( M & ( 1 << 26 ) ) != 0 ? 26 :
				( M & ( 1 << 25 ) ) != 0 ? 25 :
				( M & ( 1 << 24 ) ) != 0 ? 24 :
				( M & ( 1 << 23 ) ) != 0 ? 23 :
				( M & ( 1 << 22 ) ) != 0 ? 22 :
				( M & ( 1 << 21 ) ) != 0 ? 21 :
				( M & ( 1 << 20 ) ) != 0 ? 20 :
				( M & ( 1 << 19 ) ) != 0 ? 19 :
				( M & ( 1 << 18 ) ) != 0 ? 18 :
				( M & ( 1 << 17 ) ) != 0 ? 17 :
				( M & ( 1 << 16 ) ) != 0 ? 16 :
				( M & ( 1 << 15 ) ) != 0 ? 15 :
				( M & ( 1 << 14 ) ) != 0 ? 14 :
				( M & ( 1 << 13 ) ) != 0 ? 13 :
				( M & ( 1 << 12 ) ) != 0 ? 12 :
				( M & ( 1 << 11 ) ) != 0 ? 11 :
				( M & ( 1 << 10 ) ) != 0 ? 10 :
				( M & ( 1 << 9 ) ) != 0 ? 9 :
				( M & ( 1 << 8 ) ) != 0 ? 8 :
				( M & ( 1 << 7 ) ) != 0 ? 7 :
				( M & ( 1 << 6 ) ) != 0 ? 6 :
				( M & ( 1 << 5 ) ) != 0 ? 5 :
				( M & ( 1 << 4 ) ) != 0 ? 4 :
				( M & ( 1 << 3 ) ) != 0 ? 3 :
				( M & ( 1 << 2 ) ) != 0 ? 2 :
				( M & ( 1 << 1 ) ) != 0 ? 1 :
				0,

			FirstSetBit = 
				( M & ( 1 << 0 ) ) != 0 ? 0 :
				( M & ( 1 << 1 ) ) != 0 ? 1 :
				( M & ( 1 << 2 ) ) != 0 ? 2 :
				( M & ( 1 << 3 ) ) != 0 ? 3 :
				( M & ( 1 << 4 ) ) != 0 ? 4 :
				( M & ( 1 << 5 ) ) != 0 ? 5 :
				( M & ( 1 << 6 ) ) != 0 ? 6 :
				( M & ( 1 << 7 ) ) != 0 ? 7 :
				( M & ( 1 << 8 ) ) != 0 ? 8 :
				( M & ( 1 << 9 ) ) != 0 ? 9 :
				( M & ( 1 << 10 ) ) != 0 ? 10 :
				( M & ( 1 << 11 ) ) != 0 ? 11 :
				( M & ( 1 << 12 ) ) != 0 ? 12 :
				( M & ( 1 << 13 ) ) != 0 ? 13 :
				( M & ( 1 << 14 ) ) != 0 ? 14 :
				( M & ( 1 << 15 ) ) != 0 ? 15 :
				( M & ( 1 << 16 ) ) != 0 ? 16 :
				( M & ( 1 << 17 ) ) != 0 ? 17 :
				( M & ( 1 << 18 ) ) != 0 ? 18 :
				( M & ( 1 << 19 ) ) != 0 ? 19 :
				( M & ( 1 << 20 ) ) != 0 ? 20 :
				( M & ( 1 << 21 ) ) != 0 ? 21 :
				( M & ( 1 << 22 ) ) != 0 ? 22 :
				( M & ( 1 << 23 ) ) != 0 ? 23 :
				( M & ( 1 << 24 ) ) != 0 ? 24 :
				( M & ( 1 << 25 ) ) != 0 ? 25 :
				( M & ( 1 << 26 ) ) != 0 ? 26 :
				( M & ( 1 << 27 ) ) != 0 ? 27 :
				( M & ( 1 << 28 ) ) != 0 ? 28 :
				( M & ( 1 << 29 ) ) != 0 ? 29 :
				( M & ( 1 << 30 ) ) != 0 ? 30 :
				( M & ( 1 << 31 ) ) != 0 ? 31 : 0,
		};
};

/// This helpful little struct will allow you to take an enum which we can interpret as an array of elements that are 'Width' bits wide.
/// The EnumArrayHelper performs a similar function to Gander::BitArray but is evaluated at compile time. It makes the manipulation of
/// enum values very easy.
/// For example, we can interpret the value of 0x34D as an array of elements that are 2 bits long:
/// 
/// typedef EnumArrayHelper<0x34D, 2> Array;
/// 
/// This interprets 0x34D as an array of 2 bit values which would look like:
/// Index:   4   3   2   1   0
/// Value:{ 11, 01, 00, 11, 01 } = 0x34D
///
/// For more information of it's usage, see the EnumHelperTest test cases.
template< EnumType Array, EnumType Width >
struct EnumArrayHelper
{
	protected :
	
	enum
	{
		MaskAll = ~EnumType( 0 ),
	};

	public :

	static inline int8u element( int8u index )
	{
		return ( ( Array >> ( index * Width ) ) & ( ~( MaskAll << Width ) ) );
	}

	template< EnumType Index >
	struct Element
	{
		public :

			enum
			{
				Value = ( ( (~( MaskAll << Width )) << ( Index * Width ) ) & Array ) >> ( Index * Width ),
			};
	};

	template< EnumType Index, EnumType Element >
	struct InsertElement
	{
		private :
			
			enum
			{
				Mask = ( MaskAll << ( Width * Index ) ),
			};
		
		public :

			enum
			{
				Value = ( ( Array & Mask ) << Width ) | ( ( ~Mask ) & Array ) | ( ( Element & ( ~( MaskAll << Width ) ) ) << ( Index * Width ) ),
			};
	};

	template< EnumType Index >
	struct RemoveElement
	{
		private :

			enum
			{
				MaxIndex = ( EnumTypeSize * 8 ) / Width - 1,
				LowerClampedIndex = Index > MaxIndex ? MaxIndex : Index,
				UpperClampedIndex = Index + 1 > MaxIndex ? MaxIndex : Index + 1,
				MaskTop = Index + 1 > MaxIndex ? 0 : EnumType( MaskAll << ( Width * UpperClampedIndex ) ),
				MaskBottom = Index > MaxIndex ? MaskAll : ~EnumType( MaskAll << ( Width * LowerClampedIndex ) )
			};
		
		public :

			enum
			{
				Value = ( ( Array & MaskTop ) >> Width ) | ( MaskBottom & Array ),
			};
	
			friend class Gander::Test::EnumHelperTest;
	};
	
	template< EnumType Mask >
	struct PadElements
	{
		private :

			enum
			{
				Insert0 = ( ( Mask & ( EnumType( 1 ) << 0 ) ) == 0 ),
				Array0 = Insert0 == 1 ? EnumArrayHelper< Array, Width >:: template InsertElement< 0, 0 >::Value : Array,
				Insert1 = ( ( Mask & ( EnumType( 1 ) << 1 ) ) == 0 ),
				Array1 = Insert1 == 1 ? EnumArrayHelper< Array0, Width >:: template InsertElement< 1, 0 >::Value : Array0,
				Insert2 = ( ( Mask & ( EnumType( 1 ) << 2 ) ) == 0 ),
				Array2 = Insert2 == 1 ? EnumArrayHelper< Array1, Width >:: template InsertElement< 2, 0 >::Value : Array1,
				Insert3 = ( ( Mask & ( EnumType( 1 ) << 3 ) ) == 0 ),
				Array3 = Insert3 == 1 ? EnumArrayHelper< Array2, Width >:: template InsertElement< 3, 0 >::Value : Array2,
				Insert4 = ( ( Mask & ( EnumType( 1 ) << 4 ) ) == 0 ),
				Array4 = Insert4 == 1 ? EnumArrayHelper< Array3, Width >:: template InsertElement< 4, 0 >::Value : Array3,
				Insert5 = ( ( Mask & ( EnumType( 1 ) << 5 ) ) == 0 ),
				Array5 = Insert5 == 1 ? EnumArrayHelper< Array4, Width >:: template InsertElement< 5, 0 >::Value : Array4,
				Insert6 = ( ( Mask & ( EnumType( 1 ) << 6 ) ) == 0 ),
				Array6 = Insert6 == 1 ? EnumArrayHelper< Array5, Width >:: template InsertElement< 6, 0 >::Value : Array5,
				Insert7 = ( ( Mask & ( EnumType( 1 ) << 7 ) ) == 0 ),
				Array7 = Insert7 == 1 ? EnumArrayHelper< Array6, Width >:: template InsertElement< 7, 0 >::Value : Array6,
				Insert8 = ( ( Mask & ( EnumType( 1 ) << 8 ) ) == 0 ),
				Array8 = Insert8 == 1 ? EnumArrayHelper< Array7, Width >:: template InsertElement< 8, 0 >::Value : Array7,
				Insert9 = ( ( Mask & ( EnumType( 1 ) << 9 ) ) == 0 ),
				Array9 = Insert9 == 1 ? EnumArrayHelper< Array8, Width >:: template InsertElement< 9, 0 >::Value : Array8,
				Insert10 = ( ( Mask & ( EnumType( 1 ) << 10 ) ) == 0 ),
				Array10 = Insert10 == 1 ? EnumArrayHelper< Array9, Width >:: template InsertElement< 10, 0 >::Value : Array9,
				Insert11 = ( ( Mask & ( EnumType( 1 ) << 11 ) ) == 0 ),
				Array11 = Insert11 == 1 ? EnumArrayHelper< Array10, Width >:: template InsertElement< 11, 0 >::Value : Array10,
				Insert12 = ( ( Mask & ( EnumType( 1 ) << 12 ) ) == 0 ),
				Array12 = Insert12 == 1 ? EnumArrayHelper< Array11, Width >:: template InsertElement< 12, 0 >::Value : Array11,
				Insert13 = ( ( Mask & ( EnumType( 1 ) << 13 ) ) == 0 ),
				Array13 = Insert13 == 1 ? EnumArrayHelper< Array12, Width >:: template InsertElement< 13, 0 >::Value : Array12,
				Insert14 = ( ( Mask & ( EnumType( 1 ) << 14 ) ) == 0 ),
				Array14 = Insert14 == 1 ? EnumArrayHelper< Array13, Width >:: template InsertElement< 14, 0 >::Value : Array13,
				Insert15 = ( ( Mask & ( EnumType( 1 ) << 15 ) ) == 0 ),
				Array15 = Insert15 == 1 ? EnumArrayHelper< Array14, Width >:: template InsertElement< 15, 0 >::Value : Array14,
			};
		
		public :

			enum
			{
				Value = Array15
			};
	
			friend class Gander::Test::EnumHelperTest;
	};

	template< EnumType Mask >
	struct RemoveElements
	{
		private :
			enum
			{
				Array0 = RemoveElement< EnumHelper<~Mask>::FirstSetBit >::Value,
				Mask0 = EnumArrayHelper< Mask, 1 >:: template RemoveElement< EnumHelper<~Mask>::FirstSetBit >::Value,
				Array1 = EnumArrayHelper< Array0, Width >:: template RemoveElement< EnumHelper<~Mask0>::FirstSetBit >::Value,
				Mask1 = EnumArrayHelper< Mask0, 1 >:: template RemoveElement< EnumHelper<~Mask0>::FirstSetBit >::Value,
				Array2 = EnumArrayHelper< Array1, Width >:: template RemoveElement< EnumHelper<~Mask1>::FirstSetBit >::Value,
				Mask2 = EnumArrayHelper< Mask1, 1 >:: template RemoveElement< EnumHelper<~Mask1>::FirstSetBit >::Value,
				Array3 = EnumArrayHelper< Array2, Width >:: template RemoveElement< EnumHelper<~Mask2>::FirstSetBit >::Value,
				Mask3 = EnumArrayHelper< Mask2, 1 >:: template RemoveElement< EnumHelper<~Mask2>::FirstSetBit >::Value,
				Array4 = EnumArrayHelper< Array3, Width >:: template RemoveElement< EnumHelper<~Mask3>::FirstSetBit >::Value,
				Mask4 = EnumArrayHelper< Mask3, 1 >:: template RemoveElement< EnumHelper<~Mask3>::FirstSetBit >::Value,
				Array5 = EnumArrayHelper< Array4, Width >:: template RemoveElement< EnumHelper<~Mask4>::FirstSetBit >::Value,
				Mask5 = EnumArrayHelper< Mask4, 1 >:: template RemoveElement< EnumHelper<~Mask4>::FirstSetBit >::Value,
				Array6 = EnumArrayHelper< Array5, Width >:: template RemoveElement< EnumHelper<~Mask5>::FirstSetBit >::Value,
				Mask6= EnumArrayHelper< Mask5, 1 >:: template RemoveElement< EnumHelper<~Mask5>::FirstSetBit >::Value,
				Array7 = EnumArrayHelper< Array6, Width >:: template RemoveElement< EnumHelper<~Mask6>::FirstSetBit >::Value,
				Mask7= EnumArrayHelper< Mask6, 1 >:: template RemoveElement< EnumHelper<~Mask6>::FirstSetBit >::Value,
				Array8 = EnumArrayHelper< Array7, Width >:: template RemoveElement< EnumHelper<~Mask7>::FirstSetBit >::Value,
				Mask8= EnumArrayHelper< Mask7, 1 >:: template RemoveElement< EnumHelper<~Mask7>::FirstSetBit >::Value,
				Array9 = EnumArrayHelper< Array8, Width >:: template RemoveElement< EnumHelper<~Mask8>::FirstSetBit >::Value,
				Mask9 = EnumArrayHelper< Mask8, 1 >:: template RemoveElement< EnumHelper<~Mask8>::FirstSetBit >::Value,
				Array10 = EnumArrayHelper< Array9, Width >:: template RemoveElement< EnumHelper<~Mask9>::FirstSetBit >::Value,
				Mask10 = EnumArrayHelper< Mask9, 1 >:: template RemoveElement< EnumHelper<~Mask9>::FirstSetBit >::Value,
				Array11 = EnumArrayHelper< Array10, Width >:: template RemoveElement< EnumHelper<~Mask10>::FirstSetBit >::Value,
				Mask11 = EnumArrayHelper< Mask10, 1 >:: template RemoveElement< EnumHelper<~Mask10>::FirstSetBit >::Value,
				Array12 = EnumArrayHelper< Array11, Width >:: template RemoveElement< EnumHelper<~Mask11>::FirstSetBit >::Value,
				Mask12 = EnumArrayHelper< Mask11, 1 >:: template RemoveElement< EnumHelper<~Mask11>::FirstSetBit >::Value,
				Array13 = EnumArrayHelper< Array12, Width >:: template RemoveElement< EnumHelper<~Mask12>::FirstSetBit >::Value,
				Mask13 = EnumArrayHelper< Mask12, 1 >:: template RemoveElement< EnumHelper<~Mask12>::FirstSetBit >::Value,
				Array14 = EnumArrayHelper< Array13, Width >:: template RemoveElement< EnumHelper<~Mask13>::FirstSetBit >::Value,
				Mask14 = EnumArrayHelper< Mask13, 1 >:: template RemoveElement< EnumHelper<~Mask13>::FirstSetBit >::Value,
				Array15 = EnumArrayHelper< Array14, Width >:: template RemoveElement< EnumHelper<~Mask14>::FirstSetBit >::Value,
				Mask15 = EnumArrayHelper< Mask14, 1 >:: template RemoveElement< EnumHelper<~Mask14>::FirstSetBit >::Value,
				Array16 = EnumArrayHelper< Array15, Width >:: template RemoveElement< EnumHelper<~Mask15>::FirstSetBit >::Value,
				Mask16= EnumArrayHelper< Mask15, 1 >:: template RemoveElement< EnumHelper<~Mask15>::FirstSetBit >::Value,
				Array17 = EnumArrayHelper< Array16, Width >:: template RemoveElement< EnumHelper<~Mask16>::FirstSetBit >::Value,
				Mask17= EnumArrayHelper< Mask16, 1 >:: template RemoveElement< EnumHelper<~Mask16>::FirstSetBit >::Value,
				Array18 = EnumArrayHelper< Array17, Width >:: template RemoveElement< EnumHelper<~Mask17>::FirstSetBit >::Value,
				Mask18= EnumArrayHelper< Mask17, 1 >:: template RemoveElement< EnumHelper<~Mask17>::FirstSetBit >::Value,
				Array19 = EnumArrayHelper< Array18, Width >:: template RemoveElement< EnumHelper<~Mask18>::FirstSetBit >::Value,
				Mask19 = EnumArrayHelper< Mask18, 1 >:: template RemoveElement< EnumHelper<~Mask18>::FirstSetBit >::Value,
				Array20 = EnumArrayHelper< Array19, Width >:: template RemoveElement< EnumHelper<~Mask19>::FirstSetBit >::Value,
				Mask20 = EnumArrayHelper< Mask19, 1 >:: template RemoveElement< EnumHelper<~Mask19>::FirstSetBit >::Value,
				Array21 = EnumArrayHelper< Array20, Width >:: template RemoveElement< EnumHelper<~Mask20>::FirstSetBit >::Value,
				Mask21 = EnumArrayHelper< Mask20, 1 >:: template RemoveElement< EnumHelper<~Mask20>::FirstSetBit >::Value,
				Array22 = EnumArrayHelper< Array21, Width >:: template RemoveElement< EnumHelper<~Mask21>::FirstSetBit >::Value,
				Mask22 = EnumArrayHelper< Mask21, 1 >:: template RemoveElement< EnumHelper<~Mask21>::FirstSetBit >::Value,
				Array23 = EnumArrayHelper< Array22, Width >:: template RemoveElement< EnumHelper<~Mask22>::FirstSetBit >::Value,
				Mask23 = EnumArrayHelper< Mask22, 1 >:: template RemoveElement< EnumHelper<~Mask22>::FirstSetBit >::Value,
				Array24 = EnumArrayHelper< Array23, Width >:: template RemoveElement< EnumHelper<~Mask23>::FirstSetBit >::Value,
				Mask24 = EnumArrayHelper< Mask23, 1 >:: template RemoveElement< EnumHelper<~Mask23>::FirstSetBit >::Value,
				Array25 = EnumArrayHelper< Array24, Width >:: template RemoveElement< EnumHelper<~Mask24>::FirstSetBit >::Value,
				Mask25 = EnumArrayHelper< Mask24, 1 >:: template RemoveElement< EnumHelper<~Mask24>::FirstSetBit >::Value,
				Array26 = EnumArrayHelper< Array25, Width >:: template RemoveElement< EnumHelper<~Mask25>::FirstSetBit >::Value,
				Mask26= EnumArrayHelper< Mask25, 1 >:: template RemoveElement< EnumHelper<~Mask25>::FirstSetBit >::Value,
				Array27 = EnumArrayHelper< Array26, Width >:: template RemoveElement< EnumHelper<~Mask26>::FirstSetBit >::Value,
				Mask27= EnumArrayHelper< Mask26, 1 >:: template RemoveElement< EnumHelper<~Mask26>::FirstSetBit >::Value,
				Array28 = EnumArrayHelper< Array27, Width >:: template RemoveElement< EnumHelper<~Mask27>::FirstSetBit >::Value,
				Mask28= EnumArrayHelper< Mask27, 1 >:: template RemoveElement< EnumHelper<~Mask27>::FirstSetBit >::Value,
				Array29 = EnumArrayHelper< Array28, Width >:: template RemoveElement< EnumHelper<~Mask28>::FirstSetBit >::Value,
				Mask29 = EnumArrayHelper< Mask28, 1 >:: template RemoveElement< EnumHelper<~Mask28>::FirstSetBit >::Value,
				Array30 = EnumArrayHelper< Array29, Width >:: template RemoveElement< EnumHelper<~Mask29>::FirstSetBit >::Value,
				Mask30 = EnumArrayHelper< Mask29, 1 >:: template RemoveElement< EnumHelper<~Mask29>::FirstSetBit >::Value,
				Array31 = EnumArrayHelper< Array30, Width >:: template RemoveElement< EnumHelper<~Mask30>::FirstSetBit >::Value,
				Mask31 = EnumArrayHelper< Mask30, 1 >:: template RemoveElement< EnumHelper<~Mask30>::FirstSetBit >::Value,
			};

		public :

			enum
			{
				Value = Array31
			};
	
			friend class Gander::Test::EnumHelperTest;
	};
	
};

}; // namespace Gander

#endif
