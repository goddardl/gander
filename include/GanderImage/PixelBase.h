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
#ifndef __GANDERIMAGE_PIXELBASE__
#define __GANDERIMAGE_PIXELBASE__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Tuple.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"
#include "GanderImage/PixelLayout.h"

namespace Gander
{

namespace Image
{

namespace Detail
{

template< class L, EnumType N >
struct PixelBaseRecurse;

template< class L >
struct PixelBaseRecurse< L, 0 >
{
		template< class ChannelType, ChannelMask Mask, EnumType Index >
		inline ChannelType &channelAtIndex()
		{
			//\todo: The requested channel does not exist so report it with an error...
			static ChannelType t( 0 );
			return t;
		};
};

template< class L, EnumType N >
struct PixelBaseRecurse : public PixelBaseRecurse< L, N - 1 >
{
	private :

		enum
		{
			LayoutIndex = L::NumberOfLayouts - N
		};

	public :

		typedef PixelBaseRecurse< L, N - 1  > BaseType;
		typedef typename L::template LayoutTraits< L::NumberOfLayouts - N >::LayoutType LayoutType;
		typedef typename LayoutType::StorageType StorageType;

		Tuple< StorageType, LayoutType::NumberOfChannels, LayoutType::IsDynamic > m_data;

		template< class ChannelType, ChannelMask Mask, EnumType Index >
		inline ChannelType &channelAtIndex()
		{
			enum
			{
				RequestedIndex = L::template ChannelIndexHelper< Index, Mask >::Value,
				ChannelIndexInLayout = L::template ChannelIndexHelper< Index, Mask >::ChannelIndexInLayout 
			};
			
			std::cerr << "Layout " << LayoutIndex << ", index " << Index << ", channelIndex " << ChannelIndexInLayout << " channels " << ChannelMask( LayoutType::ChannelMask ) << std::endl;

			if( RequestedIndex == EnumType( LayoutIndex ) )
			{
				return m_data[ChannelIndexInLayout];
			}
			else
			{
				return BaseType::template channelAtIndex< ChannelType, Mask, Index >();
			}
		}
};

};

template< class L >
class PixelBase : public Detail::PixelBaseRecurse< L, L::NumberOfLayouts >
{
	typedef Detail::PixelBaseRecurse< L, L::NumberOfLayouts > BaseType;
	
	public :
		
		enum
		{
			NumberOfLayouts = L::NumberOfLayouts,
		};

		typedef L LayoutType;

		template< ChannelDefault C = Chan_None >
		struct ChannelTraits : public LayoutType::template ChannelTraits< C >
		{
			ChannelTraits( L layout, Channel channel ) :
				LayoutType::template ChannelTraits< C >( layout, channel )
			{
			}
		};
		
		template< EnumType LayoutIndex >
		struct LayoutTraits : public LayoutType::template LayoutTraits< LayoutIndex >
		{};
		
		/// The default constructor.
		/// The default constructor can be used for all layouts which aren't dynamic.
		/// For dynamic layouts, please use the other constructor.
		PixelBase()
		{
			GANDER_IMAGE_STATIC_ASSERT( !L::IsDynamic, CLASS_CONTAINS_A_DYNAMIC_LAYOUT_PLEASE_USE_THE_CONSTRUCTOR_THAT_INITIALIZES_IT );
		}
		/// The dynamic constructor.
		/// A basic constructor that just initializes the internal instance of the layout to the parameter that is passed to it.
		/// This constructor must be used when using a dynamic layout to ensure that the layout is initialized correctly.
		PixelBase( const L &layout ) :
			m_layout( layout )
		{
		}
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet requiredChannels() const
		{
			return m_layout.requiredChannels();
		}

		/// The runtime creator of the channel traits struct.
		template< ChannelDefault C = Chan_None >
		ChannelTraits<C> channelTraits( Channel channel )
		{
			return ChannelTraits<C>( m_layout, channel );
		}

		template< class ChannelType, ChannelMask Mask = Mask_All >
		inline ChannelType &channelAtIndex( unsigned int index )
		{
			switch( index )
			{
				case( 0 ) : return BaseType::template channelAtIndex< ChannelType, Mask, 0 >(); break;
				case( 1 ) : return BaseType::template channelAtIndex< ChannelType, Mask, 1 >(); break;
				case( 2 ) : return BaseType::template channelAtIndex< ChannelType, Mask, 2 >(); break;
				case( 3 ) : return BaseType::template channelAtIndex< ChannelType, Mask, 3 >(); break;
				case( 4 ) : return BaseType::template channelAtIndex< ChannelType, Mask, 4 >(); break;
				case( 5 ) : return BaseType::template channelAtIndex< ChannelType, Mask, 5 >(); break;
			};
			
			static ChannelType t( 0 );
			return t;
		}

	private :

		L m_layout;

};

}; // namespace Image

}; // namespace Gander

#endif
