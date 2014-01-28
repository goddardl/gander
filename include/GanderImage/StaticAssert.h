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
#ifndef __GANDER_IMAGE_STATICASSERT_H__
#define __GANDER_IMAGE_STATICASSERT_H__

#include "Gander/Common.h"

namespace Gander
{

namespace Image
{

namespace Detail
{

template<bool condition>
struct static_assertion {};

template<>
struct static_assertion<true>
{
	enum
	{
		// The template arguments to the ImageLayout class must be defined in order of lowest channel value
		// on the left, ascending to the channel of highest value on the right. See Gander::Image::Channel
		// for the values associated with each channel.
		TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_BE_SUPPLIED_IN_ORDER_OF_CHANNEL_VALUE,
		// The template arguments to the ImageLayout class must define the layout of a Channel twice. 
		// For example, by using defining ChannelLayout< X, Chan_Red > and BrotherLayout< X, Brothers_RGB >
		// as they both represent the red channel.
		TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_ALL_REPRESENT_UNIQUE_CHANNELS,
		// The layout class does not contain the requested channel.
		CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT,
		// The Pixel class uses a dynamic layout and therefore the default constructor cannot be used.
		CLASS_CONTAINS_A_DYNAMIC_LAYOUT_PLEASE_USE_THE_CONSTRUCTOR_THAT_INITIALIZES_IT,
		// The PixelLayout class requires that any dynamic layouts used as template arguments
		// be the last argument on the right.
		ONLY_ONE_DYNAMIC_LAYOUT_MUST_BE_SPECIFED_AS_THE_LAST_TEMPLATE_ARGUMENT,
		// An index was given which is out of bounds when looking up a Layout. This is most likely the
		// result of calling PixelBase::LayoutTraits<X> where X is an index that is out of bounds.
		THE_REQUESTED_LAYOUT_AT_THE_GIVEN_INDEX_DOES_NOT_EXIST,
		// You can only add channels to a DynamicLayout.
		THE_LAYOUT_MUST_BE_DYNAMIC_IN_ORDER_TO_ADD_CHANNELS_TO_IT,
	};
};

}; // namespace Detail

#define GANDER_IMAGE_STATIC_ASSERT(CONDITION,MSG) \
	enum{ CAT( CAT(ERROR, __AT__ ), __LINE__ ) = Detail::static_assertion<bool(CONDITION)>::MSG };

}; // namespace Image

}; // namespace Gander

#endif
