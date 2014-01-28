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
#ifndef __GANDERIMAGE_CHANNELLAYOUT__
#define __GANDERIMAGE_CHANNELLAYOUT__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Assert.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/Layout.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

/// Forward declaration of the test class. 
namespace ImageTest { class ChannelLayoutTest; };

namespace Image
{

template< class T, ChannelDefault S >
struct ChannelLayout : public Layout< ChannelLayout< T, S > >
{
	public :

		typedef ChannelLayout< T, S > Type;
		typedef T StorageType;

		enum
		{
			NumberOfChannels = 1,
			ChannelMask = ChannelToMask<S>::Value,
		};

		template< EnumType LayoutIndex >
		struct LayoutTraits
		{
			GANDER_IMAGE_STATIC_ASSERT( LayoutIndex == 0, THE_REQUESTED_LAYOUT_AT_THE_GIVEN_INDEX_DOES_NOT_EXIST );
			typedef ChannelLayout< T, S > LayoutType;
			typedef T StorageType;
		};

		template< ChannelDefault C = Chan_None >
		struct ChannelTraits : public Detail::ChannelTraitsInterface< Type >
		{
			typedef Type LayoutType;
			typedef T StorageType;
			ChannelTraits( const LayoutType &l, Channel channel = Chan_None ) :
				Detail::ChannelTraitsInterface< LayoutType >( l, channel )
			{
			}
		};

	private :

		friend class Layout< ChannelLayout< T, S > >;	

		/// Returns the channels represented by this layout.
		inline ChannelSet _channels() const
		{
			return ChannelSet( static_cast<Gander::Image::ChannelMask>( Type::ChannelMask ) );
		}
		
		/// Returns the number of channels that this layout represents.
		inline unsigned int _numberOfChannels() const
		{
			return static_cast<unsigned int>( NumberOfChannels );
		}
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const
		{
			return ChannelSet( S );
		}
		
		/// Returns the step value for a given channel.
		template< ChannelDefault C = Chan_None >
		inline int8u _step( Channel channel = Chan_None ) const
		{
			if( C != Chan_None )
			{
				GANDER_ASSERT( _contains( C ), "Channel is not represented by this layout." )
			}
			else
			{
				GANDER_ASSERT( _contains( channel ), "Channel is not represented by this layout." )
			}
			
			return 1;
		}
		
		/// Returns whether the layout represents the given channel.
		inline bool _contains( ChannelSet channels ) const
		{
			return ( ChannelSet( S ).contains( channels ) );
		}
};

}; // namespace Image

}; // namespace Gander

#endif
