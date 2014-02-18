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

/// Defines some enum values required by classes in Gander::Image.
/// This macro needs to be added as the end of a BrotherTraits specialization.
#define DECLARE_DEFAULT_BROTHER_ENUMS( BROTHERS_ENUM ) \
public:\
enum\
{\
	BrotherOfLowestValueMask = ( 1 << ( BrotherOfLowestValue - 1 ) ),\
	FirstBrotherInBrothers = ( BrotherOfLowestValue ) + ( BrotherOrder1 == 0 ? 0 : BrotherOrder2 == 0 ? 1 : BrotherOrder3 == 0 ? 2 : BrotherOrder4 == 0 ? 3 : 0 ),\
	SecondBrotherInBrothers = ( BrotherOfLowestValue ) + ( BrotherOrder1 == 1 ? 0 : BrotherOrder2 == 1 ? 1 : BrotherOrder3 == 1 ? 2 : BrotherOrder4 == 1 ? 3 : 0 ),\
	ThirdBrotherInBrothers = ( BrotherOfLowestValue ) + ( BrotherOrder1 == 2 ? 0 : BrotherOrder2 == 2 ? 1 : BrotherOrder3 == 2 ? 2 : BrotherOrder4 == 2 ? 3 : 0 ),\
	FourthBrotherInBrothers = ( BrotherOfLowestValue ) + ( BrotherOrder1 == 3 ? 0 : BrotherOrder2 == 3 ? 1 : BrotherOrder3 == 3 ? 2 : BrotherOrder4 == 3 ? 3 : 0 ),\
	FirstBrotherInBrothersMask = ( 1 << ( FirstBrotherInBrothers - 1 ) ),\
	BrothersMask =\
		( ( 1 << BrotherOrder1 ) |\
		( NumberOfBrothers >= 2 ? ( 1 << BrotherOrder2 ) : 0 ) |\
		( NumberOfBrothers >= 3 ? ( 1 << BrotherOrder3 ) : 0 ) |\
		( NumberOfBrothers >= 4 ? ( 1 << BrotherOrder4 ) : 0 ) ) << ( BrotherOfLowestValue - 1 ),\
	OffsetsShiftedByChannelValue = ( ( BrotherOrder1 | ( BrotherOrder2 << 2 ) | ( BrotherOrder3 << 4 ) | ( BrotherOrder4 << 6 ) ) << ( ( BrotherOfLowestValue - 1 ) << 1 ) ),\
	ChannelBrothers = BROTHERS_ENUM,\
};\
template< EnumType C >\
struct IndexOfChannelInBrothers \
{\
	enum\
	{\
		Value = ( C == FirstBrotherInBrothers ) ? 0 : ( C == SecondBrotherInBrothers ) ? 1 : ( C == ThirdBrotherInBrothers ) ? 2 : ( C == FourthBrotherInBrothers ) ? 3 : 0,\
	};\
};\
static BrotherTraitsRegistry::BrotherTraitsRegistration< BROTHERS_ENUM > g_brothersRegistration;\

#define DECLARE_BROTHER_TRAITS( BROTHERS_ENUM )\
Gander::Image::BrotherTraitsRegistry::BrotherTraitsRegistration< BROTHERS_ENUM > Gander::Image::BrotherTraits< BROTHERS_ENUM >::g_brothersRegistration;

/// RGB BrotherTraits specialization.
template<>
struct BrotherTraits< Brothers_RGB > : public BrotherTraitsRegistry
{
	enum
	{
		BrotherOfLowestValue = Chan_Red,
		NumberOfBrothers = 3,
		BrotherOrder1 = 0,
		BrotherOrder2 = 1,
		BrotherOrder3 = 2,
		BrotherOrder4 = 0,
	};
	
	DECLARE_DEFAULT_BROTHER_ENUMS( Brothers_RGB );
};

/// RGBA BrotherTraits specialization.
template<>
struct BrotherTraits< Brothers_RGBA > : public BrotherTraitsRegistry
{
	enum
	{
		BrotherOfLowestValue = Chan_Red,
		NumberOfBrothers = 4,
		BrotherOrder1 = 0,
		BrotherOrder2 = 1,
		BrotherOrder3 = 2,
		BrotherOrder4 = 3,
	};
	
	DECLARE_DEFAULT_BROTHER_ENUMS( Brothers_RGBA );
};

/// BGR BrotherTraits specialization.
template<>
struct BrotherTraits< Brothers_BGR > : public BrotherTraitsRegistry
{
	enum
	{
		BrotherOfLowestValue = Chan_Red,
		NumberOfBrothers = 3,
		BrotherOrder1 = 2,
		BrotherOrder2 = 1,
		BrotherOrder3 = 0,
		BrotherOrder4 = 0,
	};
	
	DECLARE_DEFAULT_BROTHER_ENUMS( Brothers_BGR );
};

/// BGRA BrotherTraits specialization.
template<>
struct BrotherTraits< Brothers_BGRA > : public BrotherTraitsRegistry
{
	enum
	{
		BrotherOfLowestValue = Chan_Red,
		NumberOfBrothers = 4,
		BrotherOrder1 = 2,
		BrotherOrder2 = 1,
		BrotherOrder3 = 0,
		BrotherOrder4 = 3,
	};
	
	DECLARE_DEFAULT_BROTHER_ENUMS( Brothers_BGRA );
};

/// UV BrotherTraits specialization.
template<>
struct BrotherTraits< Brothers_UV > : public BrotherTraitsRegistry
{
	enum
	{
		BrotherOfLowestValue = Chan_U,
		NumberOfBrothers = 2,
		BrotherOrder1 = 0,
		BrotherOrder2 = 1,
		BrotherOrder3 = 0,
		BrotherOrder4 = 0,
	};
	
	DECLARE_DEFAULT_BROTHER_ENUMS( Brothers_UV );
};

/// VU BrotherTraits specialization.
template<>
struct BrotherTraits< Brothers_VU > : public BrotherTraitsRegistry
{
	enum
	{
		BrotherOfLowestValue = Chan_U,
		NumberOfBrothers = 2,
		BrotherOrder1 = 1,
		BrotherOrder2 = 0,
		BrotherOrder3 = 0,
		BrotherOrder4 = 0,
	};
	
	DECLARE_DEFAULT_BROTHER_ENUMS( Brothers_VU );
};

