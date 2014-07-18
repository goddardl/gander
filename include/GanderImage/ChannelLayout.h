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

		enum
		{
			ChannelMask = ChannelToMask<S>::Value,
			NumberOfChannels = 1,
		};

		typedef ChannelLayout< T, S > Type;
		typedef Layout< ChannelLayout< T, S > > BaseType;
		typedef typename Gander::template TypeTraits< T >::Type ChannelType; 
		typedef typename Gander::template TypeTraits< T >::PointerType PointerType;
		typedef typename Gander::template TypeTraits< T >::ReferenceType ReferenceType;
		typedef typename Gander::template TypeTraits< T >::ConstReferenceType ConstReferenceType;
		typedef Gander::template Tuple< ChannelType, NumberOfChannels, false > ChannelContainerType;
		typedef Gander::template Tuple< PointerType, BaseType::NumberOfLayouts, false > ChannelPointerContainerType;
		
		template< ChannelDefault C = Chan_None, bool DisableStaticAsserts = false >
		struct ChannelTraits
		{
			typedef Type LayoutType;
			typedef typename Type::ChannelType ChannelType;
			typedef typename Type::PointerType PointerType;
			typedef typename Type::ReferenceType ReferenceType;
			typedef typename Type::ConstReferenceType ConstReferenceType;
			typedef typename Type::ChannelContainerType ChannelContainerType;
			typedef typename Type::ChannelPointerContainerType ChannelPointerContainerType;
		};

		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ReferenceType channel( ChannelContainerType *container )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return (*container)[0];
		}

		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ConstReferenceType channel( const ChannelContainerType * const container )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return (*container)[0];
		}
		
		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ReferenceType channel( ChannelPointerContainerType *ptrContainer )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return *(*ptrContainer)[0];
		}
		
		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ConstReferenceType channel( const ChannelPointerContainerType * const ptrContainer )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return *(*ptrContainer)[0];
		}

		static inline void increment( ChannelPointerContainerType * ptrContainer, int i = 1 )
		{
			(*ptrContainer)[0] += i;
		}
		
		static inline void increment( ChannelPointerContainerType & ptrContainer, int i = 1 )
		{
			ptrContainer[0] += i;
		}
				
};

}; // namespace Image

}; // namespace Gander

#endif
