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
#include <vector>
#include <stdexcept>
#include <string.h>

#include "Gander/Common.h"
#include "GanderImage/Channel.h"

template <>
const char* Gander::Image::Channel::g_defaultFlags[9] = {
	"unused",
	"red",
	"green",
	"blue",
	"alpha",
	"z",
	"mask",
	"u",
	"v"
};

template <>
std::vector<const char*> Gander::Image::Channel::g_flagMappings = std::vector<const char*>(Gander::Image::Channel::g_defaultFlags, Gander::Image::Channel::g_defaultFlags+9);

Gander::int8u Gander::Image::channelIndex( Gander::Image::Channel z, Gander::Image::ChannelSet set )
{
	Channel i;
	Gander::int8u idx = 0;
	
	foreach( i, set )
	{
		if ( z == i )
		{
			return idx;
		}
		idx++;
	}
	throw std::runtime_error( "Channel does not exist in the specified channelSet.");
}

