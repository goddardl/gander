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

		template< ChannelDefault C, class ContainerType >
		inline ReferenceType channel( ContainerType &container )
		{
			return ReferenceType(0);
		//	return static_cast< Derived * >( this )->template _channel< C, ContainerType >( container );
		}

		template< EnumType Index, class ContainerType, EnumType Mask = Mask_All >
		inline ReferenceType channel( ContainerType &container )
		{
			return ReferenceType(0);
			//return static_cast< Derived * >( this )->template _channel< Derived::template MaskedChannelIndex< Index, Mask >::Value, ContainerType, Mask >( container );
		}

		template< EnumType Index, EnumType Mask = Mask_All >
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

#endif
