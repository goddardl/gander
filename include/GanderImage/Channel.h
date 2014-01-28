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
#ifndef __GANDERIMAGE_CHANNEL__
#define __GANDERIMAGE_CHANNEL__

#include <iostream>

#include "Gander/EnumHelper.h"
#include "Gander/Flags.h"

namespace Gander
{

namespace Image
{

enum
{
	/// An enum that is used to indicate that the number of channels that the format requires is decided at runtime.
	DYNAMIC_NUMBER_OF_CHANNELS = 99999,
	DYNAMIC_CHANNEL_MASK = 99998
};

/// A class that defines some useful characteristics of the Channel enums.
/// It is required that this struct is updated when new hard-coded defaults are added to the list of channel enums.
/// The reason for this is that we create several look-up tables of channel numbers to indices for use in the
/// templated image classes. These lookup tables are squashed into a single enum value by bit-shifting each element
/// into the enum by such that: element << ( index * ( width of the largest element in bits ) ). As a result, the max
/// number of ChannelDefault values that we can have is limited by the largest data type that is supported.
/// \todo: Is there a way that the lookup table mechanism can be changed so that we don't need a hard limit on the
/// number of entries that it can support?
struct ChannelTraits
{
	typedef intLongestu ChannelLookupEnumType;

	enum
	{
		NumberOfDefaultChannels = 9,     /// Should be set to the number of enum values in ChannelDefault. This includes Chan_None.
		SizeOfChannelLookupEnumType = 8, /// Should be set to sizeof( ChannelLookupEnumType )
		RepresentableBitWidth = Gander::EnumHelper< NumberOfDefaultChannels >::MinimumRequiredBitWidth, /// Defines the number of bits required to represent all ChannelDefault values.
		MaxNumberOfDefaultChannels = ( SizeOfChannelLookupEnumType * 8 ) / RepresentableBitWidth, /// Defines the maximum number of entries that can be expressed in ChannelDefault.
	};
};

/// Defines the available preset channel types.
/// If you wish to add more channels to this list of defaults,
/// make sure that ChannelTraits::NumberOfDefaultChannels is updated to
/// represent the new total, a mask is also added to ChannelMasks and
/// finally, a string representation of the channel name added to
/// the instantiation of ChannelSet::g_defaultFlags which is located in
/// Gander::Image::Channel.cpp.
enum ChannelDefault
{
	Chan_None = 0,
	Chan_Red = 1,
	Chan_Green = 2,
	Chan_Blue = 3,
	Chan_Alpha = 4,
	Chan_Z     = 5,
	Chan_U     = 6,
	Chan_V     = 7,
	Chan_Mask  = 8,
};

/// Values used to mask bits within a channel set.
/// Several of the different combinations of mask have been
/// explicitly created for use within template arguments.
enum ChannelMask
{
	Mask_None  = 0,
	Mask_Red   = 1 << ( Chan_Red - 1 ),
	Mask_Green   = 1 << ( Chan_Green - 1 ),
	Mask_Blue   = 1 << ( Chan_Blue - 1 ),
	Mask_Alpha = 1 << ( Chan_Alpha - 1 ),
	Mask_Z     = 1 << ( Chan_Z - 1 ),
	Mask_Mask  = 1 << ( Chan_Mask - 1 ),
	Mask_U     = 1 << ( Chan_U - 1 ),
	Mask_V     = 1 << ( Chan_V - 1 ),
	Mask_UV  = Mask_U | Mask_V,
	Mask_RGB  = Mask_Red | Mask_Green | Mask_Blue,
	Mask_RGBA  = Mask_RGB | Mask_Alpha,
	Mask_RG  = Mask_Red | Mask_Green,
	Mask_RB  = Mask_Red | Mask_Blue,
	Mask_GB  = Mask_Green | Mask_Blue,
	Mask_RGA  = Mask_Red | Mask_Green | Mask_Alpha,
	Mask_RBA  = Mask_Red | Mask_Blue | Mask_Alpha,
	Mask_GBA  = Mask_Green | Mask_Blue | Mask_Alpha,
	Mask_RA  = Mask_Red | Mask_Alpha,
	Mask_BA  = Mask_Blue | Mask_Alpha,
	Mask_GA  = Mask_Green | Mask_Alpha,
	Mask_All   = 0xFFFFFFFF
};

template< EnumType C > struct ChannelToMask { enum{ Value = EnumType( 1 << ( C - 1 ) ) }; };
template<> struct ChannelToMask< Chan_None > { enum{ Value = EnumType( Mask_None ) }; };

template< EnumType M, EnumType C > struct MaskContainsChannel { enum{ Value = ( EnumType( M ) & EnumType( ChannelToMask< C >::Value ) ) != 0 }; };

/// Declares the "ChannelSet" FlagSet.
GANDER_DEFINE_FLAGSET( int32u, ChannelTraits::NumberOfDefaultChannels, ChannelDefault, ChannelMask, Channel, ChannelSet );

}; // namespace Image

}; // namespace Gander

#endif
