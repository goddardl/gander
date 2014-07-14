//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
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
struct ChannelLayout : public LayoutBase< ChannelLayout< T, S >, T >
{

	public :

		enum
		{
			ChannelMask = ChannelToMask<S>::Value,
			NumberOfChannels = 1,
			ChannelPointerMask = ChannelMask,
			NumberOfChannelPointers = 1,
		};
		
		typedef ChannelLayout< T, S > Type;
		typedef Type LayoutType;
		typedef LayoutBase< ChannelLayout< T, S >, T > BaseType;
		typedef typename BaseType::StorageType ChannelType;
		typedef typename BaseType::StorageType StorageType;
		typedef typename BaseType::PointerType PointerType;
		typedef typename BaseType::ReferenceType ReferenceType;
		typedef typename BaseType::ConstReferenceType ConstReferenceType;
		typedef Detail::ChannelContainerWrapper< Type, Gander::template Tuple< StorageType, NumberOfChannels, false > > ChannelContainerType;
		typedef Detail::ChannelPointerContainerWrapper< Type, Gander::template Tuple< PointerType, NumberOfChannelPointers, false > > ChannelPointerContainerType;

		template< ChannelDefault C = Chan_None, bool DisableStaticAsserts = false >
		struct ChannelTraits
		{
			typedef Type LayoutType;
			typedef T ChannelType;
			typedef typename LayoutType::StorageType StorageType;
			typedef typename LayoutType::PointerType PointerType;
			typedef typename LayoutType::ReferenceType ReferenceType;
			typedef typename LayoutType::ConstReferenceType ConstReferenceType;
			typedef typename LayoutType::ChannelContainerType ChannelContainerType;
			typedef typename LayoutType::ChannelPointerContainerType ChannelPointerContainerType;
			
			enum
			{
				LayoutIndex = 0,
			};
		};
		
		template< int Index, EnumType Mask = Mask_All, bool DisableStaticAsserts = false   >
		struct ChannelTraitsAtIndex : public ChannelTraits<Chan_None, DisableStaticAsserts>
		{
			GANDER_IMAGE_STATIC_ASSERT( ( Mask & BaseType::template LayoutTraits< 0 >::LayoutType::ChannelMask ) != 0 || DisableStaticAsserts,  CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			GANDER_STATIC_ASSERT( Index == 0 || DisableStaticAsserts, VALUE_IS_OUT_OF_BOUNDS );
			
			enum
			{	
				ChannelIndexInLayout = Index,
			};
		};
		
		/// Increments the specified channel pointer in the container by v.
		inline void increment( ChannelPointerContainerType &container, Channel c, int v );

		/// Increments all channel pointers in the container by v.
		inline void increment( ChannelPointerContainerType &container, int v );
		
	private :

		friend class LayoutBase< ChannelLayout< T, S >, T >;	

		/// Returns a reference to the given channel from the container which is specified by the "C" template argument.
		template< ChannelDefault C >
		inline ReferenceType _channel( ChannelContainerType &container );
		
		template< ChannelDefault C >
		inline ConstReferenceType _channel( const ChannelContainerType &container ) const;
		
		/// Returns a reference to the given channel from the container which is specified by the "C" template argument.
		template< ChannelDefault C >
		inline ReferenceType _channel( ChannelPointerContainerType &container );
		
		template< ChannelDefault C >
		inline ConstReferenceType _channel( const ChannelPointerContainerType &container ) const;

		/// Returns a reference to the channel found at the given index into the number of channels in the layout from the container.
		template< EnumType Index >
		inline ReferenceType _channelAtIndex( ChannelContainerType &container );
		
		template< EnumType Index >
		inline ConstReferenceType _channelAtIndex( const ChannelContainerType &container ) const;
		
		/// Returns a reference to the channel found at the given index into the number of channels in the layout from the container.
		template< EnumType Index >
		inline ReferenceType _channelAtIndex( ChannelPointerContainerType &container );
		
		template< EnumType Index >
		inline ConstReferenceType _channelAtIndex( const ChannelPointerContainerType &container ) const;

		/// Sets the value of the pointer to the given channel in the container.
		/// Pointers can only be set for channels that are returned from the requiredChannels() method. By setting pointers
		/// for every channel in the requiredChannels() set, the layout can provide access through the channel() and channelAtIndex()
		/// methods for every channel represented by the layout.
		inline void _setChannelPointer( ChannelPointerContainerType &container, Channel channel, PointerType pointer );
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const;
		
		/// Returns the index of a channel within the layout.	
		template< EnumType Channel, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline unsigned int _indexOfChannel() const;
};

}; // namespace Image

}; // namespace Gander

#include "GanderImage/ChannelLayout.inl"

#endif
