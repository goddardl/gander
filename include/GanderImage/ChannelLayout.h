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
#include "Gander/Tuple.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/LayoutBase.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

/// Forward declaration of the test class. 
namespace ImageTest { class ChannelLayoutTest; };

namespace Image
{

template< class T, ChannelDefault S >
struct ChannelLayout : public StaticLayoutBase< ChannelLayout< T, S > >
{
	private :

		typedef StaticLayoutBase< ChannelLayout< T, S > > BaseType;

	public :

		enum
		{
			NumberOfChannels = 1,
			ChannelMask = ChannelToMask<S>::Value,
			NumberOfChannelPointers = 1,
		};

		typedef ChannelLayout< T, S > Type;
		typedef T StorageType;

		template< ChannelDefault C = Chan_None >
		struct ChannelTraits : public Detail::ChannelTraitsInterface< Type >
		{
			typedef Type LayoutType;
			typedef T StorageType;
			
			enum
			{
				LayoutIndex = 0,
			};
			
			ChannelTraits( const LayoutType &l, Channel channel = Chan_None ) :
				Detail::ChannelTraitsInterface< LayoutType >( l, channel )
			{
			}
		};
		
		template< int Index, EnumType Mask = Mask_All, bool DisableStaticAsserts = false   >
		struct ChannelTraitsAtIndex : public BaseType::template LayoutTraits< 0 >
		{
			GANDER_IMAGE_STATIC_ASSERT( ( Mask & BaseType::template LayoutTraits< 0 >::LayoutType::ChannelMask ) != 0 || DisableStaticAsserts,  CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			GANDER_STATIC_ASSERT( Index == 0 || DisableStaticAsserts, VALUE_IS_OUT_OF_BOUNDS );
			
			enum
			{	
				ChannelIndexInLayout = Index,
			};
		};
		
		/// Returns the index to the base pointer for the given channel.
		template< ChannelDefault C >
		inline int _pointerIndex() const
		{
			return 0;
		}

		/// Returns the offset to be applied to the base pointer in order to access the given channel.
		template< ChannelDefault C >
		inline int _pointerOffset() const
		{
			return 0;
		}

		using BaseType::contains;

	private :

		friend class LayoutBase< ChannelLayout< T, S > >;	

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
				GANDER_ASSERT( contains( C ), "Channel is not represented by this layout." )
			}
			else
			{
				GANDER_ASSERT( contains( channel ), "Channel is not represented by this layout." )
			}
			
			return 1;
		}
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const
		{
			GANDER_IMAGE_STATIC_ASSERT( ( Mask & ChannelMask ) != 0 || DisableStaticAsserts, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return 0;
		}
		
		/// Returns the index of a channel within the layout.	
		template< EnumType Channel, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline unsigned int _indexOfChannel() const
		{
			GANDER_IMAGE_STATIC_ASSERT( ( Mask & ChannelMask ) != 0 || DisableStaticAsserts, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return 0;
		}
};

}; // namespace Image

}; // namespace Gander

#endif
