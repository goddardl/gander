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
#ifndef __GANDERIMAGE_DYNAMICLAYOUTBASE__
#define __GANDERIMAGE_DYNAMICLAYOUTBASE__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Assert.h"
#include "Gander/Common.h"
#include "Gander/Tuple.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/LayoutBase.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{

template< class Derived, class DataType >
struct DynamicLayoutBase : public LayoutBase< Derived >
{
	public :
		
		typedef LayoutBase< Derived > BaseType;
		typedef typename Gander::template TypeTraits< DataType >::Type ChannelType;
		typedef typename Gander::template TypeTraits< DataType >::StorageType StorageType;
		typedef typename Gander::template TypeTraits< DataType >::PointerType PointerType;
		typedef typename Gander::template TypeTraits< DataType >::ReferenceType ReferenceType;
		typedef typename Gander::template TypeTraits< DataType >::ConstReferenceType ConstReferenceType;
		
		enum
		{
			IsDynamic = true,
		};
	
		template< class ContainerType, ChannelDefault C >
		inline ReferenceType channel( ContainerType &container );
		
		template< class ContainerType, ChannelDefault C >
		inline ConstReferenceType channel( const ContainerType &container ) const;
		
		template< class ContainerType >
		inline ReferenceType channel( ContainerType &container, Channel channel );
		
		template< class ContainerType >
		inline ConstReferenceType channel( const ContainerType &container, Channel channel ) const;
		
		template< class ContainerType, EnumType Index, EnumType Mask = Mask_All >
		inline ConstReferenceType channelAtIndex( const ContainerType &container ) const;
		
		template< class ContainerType, EnumType Index, EnumType Mask = Mask_All >
		inline ReferenceType channelAtIndex( ContainerType &container );
		
		template< class ContainerType, EnumType Mask = Mask_All >
		inline ReferenceType channelAtIndex( ContainerType &container, unsigned int index );
		
		template< class ContainerType, EnumType Mask = Mask_All >
		inline ConstReferenceType channelAtIndex( const ContainerType &container, unsigned int index ) const;
		
		template< Gander::Image::ChannelMask Mask = Mask_All >
		inline unsigned int maskedChannelIndex( unsigned int index ) const;
		
		template< class ContainerType >
		inline void setChannelPointer( ContainerType &container, Channel channel, PointerType pointer );
		
		//! @name Dynamic methods.
		/// All of these methods should be implemented by any derived class.
		/// These methods provide an interface to the layout that allow the structure of the channels to be modified.
		//@{
		/// Inserts a set of channels into the passed container and also adds them to the layout. Whether the channels are brothers can also optionally be defined.
		template< class ContainerType >
		void addChannels( ContainerType &container, ChannelSet c, ChannelBrothers b = Brothers_None );
		/// Adds a set of channels to the layout. Whether the channels are brothers can also optionally be defined.
		void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None );
		//@}
		
	protected :	
		
		/// Returns the index of a channel in the layout when masked.
		template< Gander::Image::ChannelMask Mask = Mask_All >
		inline unsigned int _maskedChannelIndex( unsigned int index ) const;
		
		template< class ContainerType >
		void _addChannels( ContainerType &container, ChannelSet c, ChannelBrothers b = Brothers_None );
		
		void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None );
		
};

}; // namespace Image

}; // namespace Gander

// The implementation of the StaticLayoutBase class.
#include "DynamicLayoutBase.inl"

#endif
