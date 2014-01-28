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
#ifndef __GANDERIMAGE_PIXELBASE__
#define __GANDERIMAGE_PIXELBASE__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Tuple.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"
#include "GanderImage/PixelLayout.h"

namespace Gander
{

namespace Image
{

namespace Detail
{

template< class L, unsigned N >
struct PixelBaseRecurse;

template< class L >
struct PixelBaseRecurse< L, 0 >
{
};

template< class L, unsigned N >
struct PixelBaseRecurse : public PixelBaseRecurse< L, N - 1 >
{
	typedef PixelBaseRecurse< L, N - 1  > BaseType;
	typedef typename L::template LayoutTraits< N - 1 >::LayoutType LayoutType;
	typedef typename LayoutType::StorageType StorageType;
	
	Tuple< StorageType, LayoutType::NumberOfChannels, LayoutType::IsDynamic > m_data;
};

};

template< class L >
class PixelBase : public Detail::PixelBaseRecurse< L, L::NumberOfLayouts >
{
	typedef Detail::PixelBaseRecurse< L, L::NumberOfLayouts + 1  > BaseType;
	
	public :
		
		enum
		{
			NumberOfLayouts = L::NumberOfLayouts,
		};

		typedef L LayoutType;

		template< ChannelDefault C = Chan_None >
		struct ChannelTraits : public LayoutType::template ChannelTraits< C >
		{
			ChannelTraits( L layout, Channel channel ) :
				LayoutType::template ChannelTraits< C >( layout, channel )
			{
			}
		};
		
		template< EnumType LayoutIndex >
		struct LayoutTraits : public LayoutType::template LayoutTraits< LayoutIndex >
		{};
		
		/// The default constructor.
		/// The default constructor can be used for all layouts which aren't dynamic.
		/// For dynamic layouts, please use the other constructor.
		PixelBase()
		{
			GANDER_IMAGE_STATIC_ASSERT( !L::IsDynamic, CLASS_CONTAINS_A_DYNAMIC_LAYOUT_PLEASE_USE_THE_CONSTRUCTOR_THAT_INITIALIZES_IT );
		}
		/// The dynamic constructor.
		/// A basic constructor that just initializes the internal instance of the layout to the parameter that is passed to it.
		/// This constructor must be used when using a dynamic layout to ensure that the layout is initialized correctly.
		PixelBase( const L &layout ) :
			m_layout( layout )
		{
		}
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet requiredChannels() const
		{
			return m_layout.requiredChannels();
		}

		/// The runtime creator of the channel traits struct.
		template< ChannelDefault C = Chan_None >
		ChannelTraits<C> channelTraits( Channel channel )
		{
			return ChannelTraits<C>( m_layout, channel );
		}

	private :

		L m_layout;

};

}; // namespace Image

}; // namespace Gander

#endif
