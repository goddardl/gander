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
#ifndef __GANDERIMAGE_STATICLAYOUTBASE__
#define __GANDERIMAGE_STATICLAYOUTBASE__

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
struct StaticLayoutBase : public LayoutBase< Derived >
{
	public :
		
		typedef LayoutBase< Derived > BaseType;
		typedef typename Gander::template TypeTraits< DataType >::Type ChannelType;
		typedef typename Gander::template TypeTraits< DataType >::StorageType StorageType;
		typedef typename Gander::template TypeTraits< DataType >::PointerType PointerType;
		typedef typename Gander::template TypeTraits< DataType >::ReferenceType ReferenceType;

		enum
		{
			IsDynamic = false,
		};

		template< class ContainerType, ChannelDefault C >
		inline ReferenceType channel( ContainerType &container )
		{
			return static_cast< Derived * >( this )->template _channel< C >( container );
		}

		template< class ContainerType, EnumType Index, EnumType Mask = Mask_All >
		inline ReferenceType channelAtIndex( ContainerType &container )
		{
			return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< Index, Mask >::Value >( container );
		}
		
		template< class ContainerType >
		inline ReferenceType channel( ContainerType &container, Channel c )
		{
			GANDER_ASSERT( static_cast< Derived * >( this )->channels().contains( c ), "Channel is not represented by this layout." );
			switch( c )
			{
				case( 1 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 1 ) >( container );
				case( 2 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 2 ) >( container );
				case( 3 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 3 ) >( container );
				case( 4 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 4 ) >( container );
				case( 5 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 5 ) >( container );
				case( 6 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 6 ) >( container );
				case( 7 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 7 ) >( container );
				case( 8 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 8 ) >( container );
				case( 9 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 9 ) >( container );
				case( 10 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 10 ) >( container );
				case( 11 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 11 ) >( container );
				case( 12 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 12 ) >( container );
				case( 13 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 13 ) >( container );
				case( 14 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 14 ) >( container );
				case( 15 ) : return static_cast< Derived * >( this )->template _channel< ChannelDefault( 15 ) >( container );
				default : GANDER_ASSERT( 0, "Channel does not exist in the LayoutContainer." ); break;
			}
		}

		template< class ContainerType, EnumType Mask = Mask_All >
		inline ReferenceType channelAtIndex( ContainerType &container, unsigned int index )
		{
			GANDER_ASSERT( index < static_cast< Derived * >( this )->channels().size(), "Channel is not represented by this layout." );
			switch( index )
			{
				case( 0 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 0, Mask, true >::Value >( container );
				case( 1 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 1, Mask, true >::Value >( container );
				case( 2 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 2, Mask, true >::Value >( container );
				case( 3 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 3, Mask, true >::Value >( container );
				case( 4 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 4, Mask, true >::Value >( container );
				case( 5 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 5, Mask, true >::Value >( container );
				case( 6 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 6, Mask, true >::Value >( container );
				case( 7 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 7, Mask, true >::Value >( container );
				case( 8 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 8, Mask, true >::Value >( container );
				case( 9 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 9, Mask, true >::Value >( container );
				case( 10 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 10, Mask, true >::Value >( container );
				case( 11 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 11, Mask, true >::Value >( container );
				case( 12 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 12, Mask, true >::Value >( container );
				case( 13 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 13, Mask, true >::Value >( container );
				case( 14 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 14, Mask, true >::Value >( container );
				case( 15 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 15, Mask, true >::Value >( container );
				default : GANDER_ASSERT( 0, "Index is out of bounds." ); break;
			};
		}
		
		template< class ContainerType >
		inline void setChannelPointer( ContainerType &container, Channel channel, PointerType pointer );

		template< EnumType Index, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		struct MaskedChannelIndex
		{
			GANDER_IMAGE_STATIC_ASSERT( ( Index != -1 ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_TRAITS_STRUCTS_REQUIRED_BY_THE_BASE_CLASS );
			
			// Derived classes of StaticLayoutBase need to implement this struct and provide the following:
			//
			// - An enum that maps the index ( the 'Index' template argument ) of a channel in set of channels
			// ( defined by the 'Mask' template argument ) into the index of the same channel in the layout. 
			// enum
			// {
			//		Value = Index,
			// };
		};

};

}; // namespace Image

}; // namespace Gander

// The implementation of the StaticLayoutBase class.
#include "StaticLayoutBase.inl"

#endif
