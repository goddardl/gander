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

#include "Gander/Common.h"
#include "Gander/Flags.h"
#include "Gander/FlagSet.h"

namespace Gander
{

namespace Image
{
	
/// Defines the available preset channel types.
enum ChannelDefaults 
{
	Chan_Unused = 0,
	Chan_Red = 1,
	Chan_Green = 2,
	Chan_Blue = 3,
	Chan_Alpha = 4,
	Chan_Z     = 5,
	Chan_Mask  = 6,
	Chan_U     = 7,
	Chan_V     = 8
};

/// Values used to mask bits within a channel set.
enum ChannelSetInit
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
	Mask_All   = 0xFFFFFFFF
};

typedef Gander::Flag<int32u, ChannelDefaults, 9, ChannelSetInit> Channel;

typedef Gander::FlagSet<int32u, ChannelSetInit, Channel> ChannelSet;

Gander::int8u channelIndex( Channel z, ChannelSet set );

}; // namespace Image

}; // namespace Gander

#endif
