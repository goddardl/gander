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
#ifndef __GANDERIMAGE_BROTHERSLAYOUT__
#define __GANDERIMAGE_BROTHERSLAYOUT__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Assert.h"
#include "Gander/Tuple.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/StaticLayoutBase.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{

template< class T, ChannelBrothers B >
struct BrothersLayout : public StaticLayoutBase< BrothersLayout< T, B >, T >
{

	public :

		enum
		{
			NumberOfChannels = BrotherTraits<B>::NumberOfBrothers,
			ChannelMask = BrotherTraits<B>::BrothersMask,
			NumberOfChannelPointers = 1,
		};

		typedef BrothersLayout< T, B > Type;
		typedef Type LayoutType;
		typedef StaticLayoutBase< BrothersLayout< T, B >, T > BaseType;
		typedef typename BaseType::StorageType ChannelType;
		typedef typename BaseType::StorageType StorageType;
		typedef typename BaseType::PointerType PointerType;
		typedef typename BaseType::ReferenceType ReferenceType;
		typedef Detail::ChannelContainerWrapper< Type, Gander::template Tuple< StorageType, NumberOfChannels, false > > ChannelContainerType;
		typedef Detail::ChannelPointerContainerWrapper< Type, Gander::template Tuple< PointerType, NumberOfChannelPointers, false > > ChannelPointerContainerType;

		template< ChannelDefault C = Chan_None >
		struct ChannelTraits : public Detail::ChannelTraitsInterface< Type >
		{
			typedef Type LayoutType;
			typedef T ChannelType;
			typedef typename LayoutType::StorageType StorageType;
			typedef typename LayoutType::PointerType PointerType;
			typedef typename LayoutType::ReferenceType ReferenceType;
			typedef typename LayoutType::ChannelContainerType ChannelContainerType;
			typedef typename LayoutType::ChannelPointerContainerType ChannelPointerContainerType;

			enum
			{
				LayoutIndex = 0,
			};

			ChannelTraits( const LayoutType &l, Channel channel = Chan_None ) :
				Detail::ChannelTraitsInterface< LayoutType >( l, channel )
			{
			}
		};
		
		template< int Index, EnumType Mask = Mask_All  >
		struct ChannelTraitsAtIndex : public BaseType::template LayoutTraits< 0 >
		{
			enum
			{	
				ChannelIndexInLayout = Index,
			};
		};

		using BaseType::contains;

	private :

		friend class StaticLayoutBase< BrothersLayout< T, B >, T >;	
		friend class LayoutBase< BrothersLayout< T, B > >;	
		
		template< ChannelDefault C, class ContainerType >
		inline ReferenceType _channel( ContainerType &container )
		{
			return 2.;
		}

		template< EnumType Index, class ContainerType, EnumType Mask = Mask_All >
		inline ReferenceType _channel( ContainerType &container )
		{
			return 1.;
		}


		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const
		{
			return ChannelSet( ChannelDefault( BrotherTraits<B>::FirstBrotherInBrothers ) );
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
			return BrotherTraits<B>::NumberOfBrothers;
		}
		
		/// Returns the index of a channel within the layout.	
		template< EnumType Channel, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline unsigned int _indexOfChannel() const
		{
			enum
			{
				BadIndex = 5,
				
				Mask1 = ChannelToMask< BrotherTraits<B>::FirstBrotherInBrothers >::Value,
				Mask2 = ChannelToMask< BrotherTraits<B>::SecondBrotherInBrothers >::Value,
				Mask3 = ChannelToMask< BrotherTraits<B>::ThirdBrotherInBrothers >::Value,
				Mask4 = ChannelToMask< BrotherTraits<B>::FourthBrotherInBrothers >::Value,

				Contains1 = ( Mask1 & Mask ) != 0,
				Contains2 = ( Mask2 & Mask ) != 0,
				Contains3 = ( Mask3 & Mask ) != 0,
				Contains4 = ( Mask4 & Mask ) != 0,

				Value = Channel == BrotherTraits<B>::FirstBrotherInBrothers && Contains1 ? 0 :
						Channel == BrotherTraits<B>::SecondBrotherInBrothers && Contains2 ? 1 :
						Channel == BrotherTraits<B>::ThirdBrotherInBrothers && Contains3 ? 2 :
						Channel == BrotherTraits<B>::FourthBrotherInBrothers && Contains4 ? 3 : BadIndex,
			};
			
			GANDER_IMAGE_STATIC_ASSERT( Value != BadIndex || DisableStaticAsserts, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
						
			return Value;
		}
		
		template< EnumType MaskedIndex, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		struct MaskedChannelIndex
		{
			private :

				enum
				{
					Mask1 = ChannelToMask< BrotherTraits<B>::FirstBrotherInBrothers >::Value,
					Mask2 = ChannelToMask< BrotherTraits<B>::SecondBrotherInBrothers >::Value,
					Mask3 = ChannelToMask< BrotherTraits<B>::ThirdBrotherInBrothers >::Value,
					Mask4 = ChannelToMask< BrotherTraits<B>::FourthBrotherInBrothers >::Value,

					Contains1 = ( Mask1 & Mask ) != 0,
					Contains2 = ( Mask2 & Mask ) != 0,
					Contains3 = ( Mask3 & Mask ) != 0,
					Contains4 = ( Mask4 & Mask ) != 0,

					BadIndex = 5,

					Index =
						MaskedIndex == 0 ?
						Contains1 == true ? 0 :
						Contains2 == true ? 1 :
						Contains3 == true ? 2 :
						Contains4 == true ? 3 : BadIndex :
						MaskedIndex == 1 ?
						Contains2 == true && Contains1 == true ? 1 :
						Contains3 == true && ( Contains1 + Contains2 == 1 ) ? 2 :
						Contains4 == true && ( Contains1 + Contains2 + Contains3 == 1 ) ? 3 : BadIndex :
						MaskedIndex == 2 ?
						Contains3 == true && ( Contains1 + Contains2 == 2 ) ? 2 :
						Contains4 == true && ( Contains1 + Contains2 + Contains3 == 2 ) ? 3 : BadIndex :
						MaskedIndex == 3 ?
						Contains4 == true && ( Contains1 + Contains2 + Contains3 == 3 ) ? 3 : BadIndex : BadIndex
				};

				GANDER_IMAGE_STATIC_ASSERT( Index != BadIndex || DisableStaticAsserts, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );

			public :

				enum
				{
					Value = Index,
				};
		};

		template< ChannelDefault C >
		inline ReferenceType _channel( ChannelPointerContainerType &container )
		{
			return *( container[0] + BrotherTraits<B>::template IndexOfChannelInBrothers< C >::Value );
		}

		template< ChannelDefault C >
		inline ReferenceType _channel( ChannelContainerType &container )
		{
			return container[ BrotherTraits<B>::template IndexOfChannelInBrothers< C >::Value ];
		}
		
		template< EnumType Index >
		inline ReferenceType _channelAtIndex( ChannelPointerContainerType &container )
		{
			return *( container[0] + Index );
		}
		
		template< EnumType Index >
		inline ReferenceType _channelAtIndex( ChannelContainerType &container )
		{
			return container[ Index ];
		}
		
		inline void _setChannelPointer( ChannelPointerContainerType &container, Channel channel, PointerType pointer )
		{
			container[0] = pointer;
		}

		/// Returns the index to the base pointer for the given channel.
		template< EnumType Index >
		inline int _pointerIndex() const
		{
			return 0;
		}

		/// Returns the offset to be applied to the base pointer in order to access the given channel.
		template< EnumType Index >
		inline int _pointerOffset() const
		{
			return 0;
		}
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const
		{
			return MaskedChannelIndex< Index, Mask, DisableStaticAsserts >::Value;
		}
};

}; // namespace Image

}; // namespace Gander

#endif
