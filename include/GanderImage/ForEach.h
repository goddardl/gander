//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014, Luke Goddard. All rights reserved.
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
#ifndef __GANDERIMAGE_FOREACH__
#define __GANDERIMAGE_FOREACH__

#include <iostream>
#include <stdexcept>

#include "Gander/Common.h"
#include "Gander/EnumHelper.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

#include "GanderImage/Detail/ForEach.inl"

namespace Gander
{

namespace Image
{

/// This function can be used to apply a functor to each channel of two pixels. Channels that are not shared between the two pixels are ignored.
template< class Pixel1, class Pixel2, class Op >
void forEachChannel( const Pixel1 &p1, const Pixel2 &p2, Op &op )
{
	enum
	{
		FullMask = Gander::Image::Mask_All,
		StaticMask = ( Pixel1::LayoutType::ChannelMask & Pixel2::LayoutType::ChannelMask & FullMask ),
	};
	
	op.init();	
	Detail::ForEachRecurse< Pixel1, Pixel2, Op, FullMask, StaticMask, StaticMask >()( p1, p2, op );
}

template< class Pixel1, class Pixel2, class Op >
void forEachChannel( Pixel1 &p1, Pixel2 &p2, Op op )
{
	enum
	{
		FullMask = Gander::Image::Mask_All,
		StaticMask = ( Pixel1::LayoutType::ChannelMask & Pixel2::LayoutType::ChannelMask & FullMask ),
	};
	
	op.init();	
	Detail::ForEachRecurse< Pixel1, Pixel2, Op, FullMask, StaticMask, StaticMask >()( p1, p2, op );
}

}; // namespace Image

}; // namespace Gander

#endif
