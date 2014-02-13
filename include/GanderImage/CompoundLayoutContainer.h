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
#ifndef __GANDERIMAGE_COMPOUNDLAYOUTCONTAINER__
#define __GANDERIMAGE_COMPOUNDLAYOUTCONTAINER__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Assert.h"
#include "Gander/Common.h"
#include "Gander/Tuple.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

#include "GanderImage/LayoutContainerBase.h"


namespace Gander
{

namespace Image
{

namespace Detail
{

template< class CompoundLayout, EnumType N >
struct CompoundLayoutContainerRecurse;

template< class CompoundLayout >
struct CompoundLayoutContainer;

template< class CompoundLayout >
struct CompoundLayoutContainerRecurse< CompoundLayout, 0 > : public LayoutContainerBase< CompoundLayoutContainer< CompoundLayout >, CompoundLayout >
{
	private :

		typedef LayoutContainerBase< CompoundLayoutContainerRecurse< CompoundLayout, 0 >, CompoundLayout > BaseType;
	
	protected :
		
		template< class ReturnType, EnumType Index >
		inline ReturnType &child()
		{
			GANDER_ASSERT( Index >= 0 || Index < CompoundLayout::NumberOfLayouts, "Index is out of bounds." );
			static ReturnType r; // We never get here but we still need to return something to appease the compiler.
			return r;
		};
		
		inline void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
		}
};

template< class CompoundLayout, EnumType N >
struct CompoundLayoutContainerRecurse : public CompoundLayoutContainerRecurse< CompoundLayout, N - 1 >
{
	private :

		typedef CompoundLayoutContainerRecurse< CompoundLayout, N - 1  > BaseType;

	public :

		enum { LayoutIndex = CompoundLayout::NumberOfLayouts - N };
		typedef LayoutContainer< typename CompoundLayout::template LayoutTraits< LayoutIndex >::LayoutType > ContainerType;
		typedef typename ContainerType::LayoutType LayoutType;
		typedef typename LayoutType::StorageType StorageType;

	protected :
		
		inline void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			if( !LayoutType::IsDynamic )
			{
				BaseType::_addChannels( c, b );
			}
			else
			{
				m_container.addChannels( c, b );
			}
		}
		
		template< class ReturnType, EnumType Index >
		inline ReturnType &child()
		{
			if( Index == LayoutIndex )
			{
				return ( ReturnType & ) m_container;
			}
			else
			{
				return BaseType::template child< ReturnType, Index >();
			}
		};

	private :

		ContainerType m_container;
		
		template< class, class > friend class LayoutContainerBase;	
};

template< class CompoundLayout >
class CompoundLayoutContainer : public Gander::Image::Detail::CompoundLayoutContainerRecurse< CompoundLayout, CompoundLayout::NumberOfLayouts >
{
	private :

		typedef typename Gander::Image::Detail::CompoundLayoutContainerRecurse< CompoundLayout, CompoundLayout::NumberOfLayouts > BaseType;

	public :
	
		using BaseType::channel;
		using BaseType::channelAtIndex;
		using BaseType::addChannels;
		
		template< EnumType Index >
		inline LayoutContainer< typename CompoundLayout::template LayoutTraits< Index, true >::LayoutType > &child()
		{
			return BaseType::template child< LayoutContainer< typename CompoundLayout::template LayoutTraits< Index, true >::LayoutType >, Index >();
		};

		template< EnumType Index, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false, class ChannelType = typename CompoundLayout::template ChannelTraitsAtIndex< Index, Mask >::StorageType >
		inline ChannelType &channelAtIndex()
		{
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutIndex,
				ChannelIndexInLayout = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ChannelIndexInLayout,
			};
			
			GANDER_ASSERT(
				( std::is_same< ChannelType, typename CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::StorageType >::value ),
				"Incorrect return type specified."
			);
		
			return ( ChannelType & ) child< ChildIndex >().template channelAtIndex< ChannelIndexInLayout, Mask, DisableStaticAsserts >();
		}
		
		template< ChannelDefault C, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false, class ChannelType = typename CompoundLayout::template ChannelTraits< C, true >::StorageType >
		inline ChannelType &channel()
		{
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraits< C, DisableStaticAsserts >::LayoutIndex,
			};
			
			GANDER_ASSERT( ( std::is_same< ChannelType, typename CompoundLayout::template ChannelTraits< C, DisableStaticAsserts >::StorageType >::value ), "Incorrect return type specified." );
			
			return ( ChannelType & ) child< ChildIndex >().template channel< C, Mask, DisableStaticAsserts >();
		}
	
	protected :
		
		inline void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			BaseType::_addChannels( c, b );
		}

	private :

		template< class, class > friend class LayoutContainerBase;	
};

}; // namespace Detail

}; // namespace Image

}; // namespace Gander

#endif
