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
#ifndef __GANDERIMAGE_DYNAMICLAYOUT__
#define __GANDERIMAGE_DYNAMICLAYOUT__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Assert.h"
#include "Gander/Tuple.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/DynamicLayoutBase.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{

template< class T >
struct DynamicLayout : DynamicLayoutBase< DynamicLayout< T >, T > 
{
	public :

		typedef DynamicLayout<T> Type;
		typedef Type LayoutType;
		typedef DynamicLayoutBase< DynamicLayout< T >, T > BaseType;
		typedef typename BaseType::StorageType ChannelType;
		typedef typename BaseType::StorageType StorageType;
		typedef typename BaseType::PointerType PointerType;
		typedef typename BaseType::ReferenceType ReferenceType;
		typedef typename BaseType::ConstReferenceType ConstReferenceType;
		typedef Detail::ChannelContainerWrapper< Type, Gander::template Tuple< StorageType, BaseType::NumberOfChannels, true > > ChannelContainerType;
		typedef Detail::ChannelPointerContainerWrapper< Type, Gander::template Tuple< PointerType, BaseType::NumberOfChannelPointers, true > > ChannelPointerContainerType;
		
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
		
		template< int Index, EnumType Mask = Mask_All, bool DisableStaticAsserts = false  >
		struct ChannelTraitsAtIndex : public BaseType::template LayoutTraits< 0, DisableStaticAsserts >
		{
			enum
			{	
				ChannelIndexInLayout = Index,
			};
		};
		
		//! @name Methods required by the base class.
		/// These methods are required by the base class as this layout is dynamic.
		//@{
		/// Returns the channels represented by this layout.
		inline ChannelSet channels() const;

		/// Returns the number of channels that this layout represents.
		inline unsigned int numberOfChannels() const;

		/// Returns the number of channel pointers that this layout requires
		/// in order to access all of the channels that this layout represents.
		inline unsigned int numberOfChannelPointers() const;
		//@}

		/// Increments all channel pointers in the container by v.
		inline void increment( ChannelPointerContainerType &container, int v );
		
		/// Decrements all channel pointers in the container by v.
		inline void decrement( ChannelPointerContainerType &container, int v );

	private :

		friend class DynamicLayoutBase< DynamicLayout< T >, T >;
		friend class LayoutBase< DynamicLayout< T > >;
	
		/// Inserts a new channel into the container and also adds the channel to the Layout, logging all pertenant information.
		template< class ContainerType >
		void _addChannels( ContainerType &container, ChannelSet c, ChannelBrothers b = Brothers_None );

		/// Adds the channel to the Layout and logs all pertenant information.
		void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None );
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const;

		/// Returns the index of a channel in the layout when masked.
		template< Gander::Image::ChannelMask Mask = Mask_All >
		inline unsigned int _maskedChannelIndex( unsigned int index ) const;

		/// Returns a reference to the given channel from the container.
		inline ReferenceType _channel( ChannelContainerType &container, Channel c );
		inline ConstReferenceType _channel( const ChannelContainerType &container, Channel c ) const;
		
		/// Returns a reference to the given channel from the container.
		inline ReferenceType _channel( ChannelPointerContainerType &container, Channel c );
		inline ConstReferenceType _channel( const ChannelPointerContainerType &container, Channel c ) const;

		/// Returns a reference to the channel found at the given index into the number of channels in the layout from the container.
		inline ReferenceType _channelAtIndex( ChannelContainerType &container, unsigned int index );
		inline ConstReferenceType _channelAtIndex( const ChannelContainerType &container, unsigned int index ) const;
		
		/// Returns a reference to the channel found at the given index into the number of channels in the layout from the container.
		inline ReferenceType _channelAtIndex( ChannelPointerContainerType &container, unsigned int index );
		inline ConstReferenceType _channelAtIndex( const ChannelPointerContainerType &container, unsigned int index ) const;
		
		/// Sets the value of the pointer to the given channel in the container.
		/// Pointers can only be set for channels that are returned from the requiredChannels() method. By setting pointers
		/// for every channel in the requiredChannels() set, the layout can provide access through the channel() and channelAtIndex()
		/// methods for every channel represented by the layout.
		inline void _setChannelPointer( ChannelPointerContainerType &container, Channel channel, PointerType pointer );

		/// The channels that this format represents.
		ChannelSet m_channels;

		/// All channel brothers associated with the channels that this layout contains.
		ChannelSet m_allBrothers;

		/// The step values for each channel.
		std::vector< int8u > m_steps;
};

}; // namespace Image

}; // namespace Gander

#include "GanderImage/DynamicLayout.inl"

#endif
