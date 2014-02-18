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
#ifndef __GANDERIMAGE_LAYOUTCONTAINERBASE__
#define __GANDERIMAGE_LAYOUTCONTAINERBASE__

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

namespace Gander
{

namespace Image
{

namespace Detail
{

template< class Derived, class Layout, bool IsDynamic >
struct LayoutContainerBase
{
	protected :
		
		typedef Layout LayoutType;

		LayoutContainerBase() {}
		LayoutContainerBase( LayoutType &layout ) {}
		
		LayoutType &layout()
		{
			static Layout layout;
			return layout;
		}
};

template< class Derived, class Layout >
struct LayoutContainerBase< Derived, Layout, true >
{
	protected :
		
		typedef Layout LayoutType;

		inline LayoutContainerBase() :
			m_layout( dynamicLayoutError() )
		{
		}

		inline LayoutContainerBase( LayoutType &layout ) :
			m_layout( layout )
		{
		}
		
		inline LayoutType &layout()
		{
			return m_layout;
		}

	private :
		
		inline LayoutType &dynamicLayoutError()
		{
			static LayoutType layout;
			return layout;
		}

		LayoutType &m_layout;

};

template< class Derived, class Layout >
struct LayoutContainerBaseInterface : public LayoutContainerBase< Derived, Layout, Layout::IsDynamic >
{
	private :
		
		typedef LayoutContainerBase< Derived, Layout, Layout::IsDynamic > BaseType;

	public :
		
		typedef typename Layout::StorageType StorageType;
		typedef Layout LayoutType;
		
		template< ChannelDefault C >
		struct ChannelTraits : public Layout::template ChannelTraits< C >
		{
			ChannelTraits( Layout layout, Channel channel ) :
				Layout::template ChannelTraits< C >( layout, channel )
			{
			}
		};
		
		template< EnumType LayoutIndex >
		struct LayoutTraits : public Layout::template LayoutTraits< LayoutIndex >
		{};
		
		template< int ChannelIndex, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		struct ChannelTraitsAtIndex : public Layout::template ChannelTraits< ChannelIndex, Mask, DisableStaticAsserts >
		{};

		LayoutContainerBaseInterface()
		{
			GANDER_IMAGE_STATIC_ASSERT( !Layout::IsDynamic, CLASS_CONTAINS_A_DYNAMIC_LAYOUT_PLEASE_USE_THE_CONSTRUCTOR_THAT_INITIALIZES_IT );
		}

		LayoutContainerBaseInterface( Layout &layout ) :
			BaseType( layout )
		{
		}
		
		template< class ChannelType = StorageType, ChannelMask Mask = Mask_All >
		inline ChannelType &channel( Channel c )
		{
			switch( c )
			{
				case( 1 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 1 ), Mask, true, ChannelType >(); break;
				case( 2 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 2 ), Mask, true, ChannelType >(); break;
				case( 3 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 3 ), Mask, true, ChannelType >(); break;
				case( 4 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 4 ), Mask, true, ChannelType >(); break;
				case( 5 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 5 ), Mask, true, ChannelType >(); break;
				case( 6 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 6 ), Mask, true, ChannelType >(); break;
				case( 7 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 7 ), Mask, true, ChannelType >(); break;
				case( 8 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 8 ), Mask, true, ChannelType >(); break;
				case( 9 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 9 ), Mask, true, ChannelType >(); break;
				case( 10 ) : return static_cast< Derived * >( this )->template channel< ChannelDefault( 10 ), Mask, true, ChannelType >(); break;
				default : GANDER_ASSERT( 0, "Channel does not exist in the LayoutContainer." ); break;
			}
		}
		
		template< class ChannelType = StorageType, ChannelMask Mask = Mask_All >
		inline ChannelType &channelAtIndex( unsigned int index )
		{
			switch( index )
			{
				case( 0 ) : return static_cast< Derived * >( this )->template channelAtIndex< 0, Mask, true, ChannelType >(); break;
				case( 1 ) : return static_cast< Derived * >( this )->template channelAtIndex< 1, Mask, true, ChannelType >(); break;
				case( 2 ) : return static_cast< Derived * >( this )->template channelAtIndex< 2, Mask, true, ChannelType >(); break;
				case( 3 ) : return static_cast< Derived * >( this )->template channelAtIndex< 3, Mask, true, ChannelType >(); break;
				case( 4 ) : return static_cast< Derived * >( this )->template channelAtIndex< 4, Mask, true, ChannelType >(); break;
				case( 5 ) : return static_cast< Derived * >( this )->template channelAtIndex< 5, Mask, true, ChannelType >(); break;
				case( 6 ) : return static_cast< Derived * >( this )->template channelAtIndex< 6, Mask, true, ChannelType >(); break;
				case( 7 ) : return static_cast< Derived * >( this )->template channelAtIndex< 7, Mask, true, ChannelType >(); break;
				case( 8 ) : return static_cast< Derived * >( this )->template channelAtIndex< 8, Mask, true, ChannelType >(); break;
				case( 9 ) : return static_cast< Derived * >( this )->template channelAtIndex< 9, Mask, true, ChannelType >(); break;
				case( 10 ) : return static_cast< Derived * >( this )->template channelAtIndex< 10, Mask, true, ChannelType >(); break;
				case( 11 ) : return static_cast< Derived * >( this )->template channelAtIndex< 11, Mask, true, ChannelType >(); break;
				case( 12 ) : return static_cast< Derived * >( this )->template channelAtIndex< 12, Mask, true, ChannelType >(); break;
				case( 13 ) : return static_cast< Derived * >( this )->template channelAtIndex< 13, Mask, true, ChannelType >(); break;
				case( 14 ) : return static_cast< Derived * >( this )->template channelAtIndex< 14, Mask, true, ChannelType >(); break;
				case( 15 ) : return static_cast< Derived * >( this )->template channelAtIndex< 15, Mask, true, ChannelType >(); break;
				default : GANDER_ASSERT( 0, "Index is out of bounds." ); break;
			};
		}

		inline Layout &layout()
		{
			return BaseType::layout();
		}
		
		inline void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			GANDER_IMAGE_STATIC_ASSERT( Layout::IsDynamic, THE_LAYOUT_MUST_BE_DYNAMIC_IN_ORDER_TO_ADD_CHANNELS_TO_IT )
			return static_cast< Derived * >( this )->template _addChannels( c, b );
		}
		
	protected :
		
		inline void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
		}
	
};

}; // namespace Detail

}; // namespace Image

}; // namespace Gander

#endif
