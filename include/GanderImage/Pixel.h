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

template< class T, class S, bool Condition > struct TypeSelector;
template< class T, class S > struct TypeSelector< T, S, false > { typedef T Type; };
template< class T, class S > struct TypeSelector< T, S, true > { typedef S Type; };

template< class L, EnumType N >
struct PixelRecurse;

template< class L >
struct PixelBase
{
	public :
		
		typedef L LayoutType;

		template< ChannelDefault C = Chan_None >
		struct ChannelTraits : public LayoutType::template ChannelTraits< C >
		{
			ChannelTraits( LayoutType layout, Channel channel ) :
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

		/// The runtime creator of the ChannelTraits struct.
		template< ChannelDefault C = Chan_None >
		ChannelTraits<C> channelTraits( Channel channel )
		{
			return ChannelTraits<C>( m_layout, channel );
		}
		
	protected:
		
		L m_layout;
};

template< class L >
struct PixelRecurse< L, 0 > : public PixelBase< L >
{
	public :

		typedef PixelBase< L > BaseType;

		PixelRecurse( const L &layout ) :
			BaseType( layout )
		{}
		
		PixelRecurse() {}
		
		template< class ChannelType, ChannelMask Mask, EnumType Index >
		inline ChannelType &channelAtIndex()
		{
			//\todo: The requested channel does not exist so report it with an error...
			static ChannelType t( 0 );
			return t;
		};

		template< class ChannelType, ChannelMask Mask, EnumType Channel >
		inline ChannelType &channel()
		{
			//\todo: The requested channel does not exist so report it with an error...
			static ChannelType t( 0 );
			return t;
		}
};

template< class L, EnumType N >
struct PixelRecurse : public PixelRecurse< L, N - 1 >
{
	public :

		typedef PixelRecurse< L, N - 1  > BaseType;
		typedef typename L::template LayoutTraits< L::NumberOfLayouts - N >::LayoutType LayoutType;
		typedef typename LayoutType::StorageType StorageType;
		
		PixelRecurse( const L &layout ) :
			BaseType( layout )
		{}
		
		PixelRecurse() {}

		template< class ChannelType, ChannelMask Mask, EnumType Index >
		inline ChannelType &channelAtIndex()
		{
			enum
			{
				RequestedLayoutIndex = L::template ChannelIndexHelper< Index, Mask >::Value,
				ChannelIndexInLayout = L::template ChannelIndexHelper< Index, Mask >::ChannelIndexInLayout 
			};
			
			if( RequestedLayoutIndex == EnumType( LayoutIndex ) )
			{
				return m_data[ChannelIndexInLayout];
			}
			else
			{
				return BaseType::template channelAtIndex< ChannelType, Mask, Index >();
			}
		}

		template< class ChannelType, ChannelMask Mask, EnumType Channel >
		inline ChannelType &channel()
		{
			if( BaseType::m_layout.template containsChannel< Channel >() )
			{
				return m_data[0];
			}
			else
			{
				return BaseType::template channel< ChannelType, Mask, Channel >();
			}
		}

	private :

		enum { LayoutIndex = L::NumberOfLayouts - N };

		Tuple< StorageType, LayoutType::NumberOfChannels, LayoutType::IsDynamic > m_data;
};

};

template< class L >
class Pixel : public Detail::PixelRecurse< L, L::NumberOfLayouts >
{
	public :
		
		typedef L LayoutType;
		
		/// The default constructor.
		/// The default constructor can be used for all layouts which aren't dynamic.
		/// For dynamic layouts, please use the other constructor.
		Pixel() {}

		/// The dynamic constructor.
		/// A basic constructor that just initializes the internal instance of the layout to the parameter that is passed to it.
		/// This constructor must be used when using a dynamic layout to ensure that the layout is initialized correctly.
		Pixel( const L &layout ) :
			BaseType( layout )
		{}
		
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

		template< class ChannelType, ChannelMask Mask = Mask_All >
		inline ChannelType &channel( Channel channel )
		{
			switch( channel )
			{
				case( 0 ) : GANDER_ASSERT( 0 != channel, "Channel cannot be Chan_None." ); break;
				case( 1 ) : return BaseType::template channel< ChannelType, Mask, 1 >(); break;
				case( 2 ) : return BaseType::template channel< ChannelType, Mask, 2 >(); break;
				case( 3 ) : return BaseType::template channel< ChannelType, Mask, 3 >(); break;
				case( 4 ) : return BaseType::template channel< ChannelType, Mask, 4 >(); break;
				case( 5 ) : return BaseType::template channel< ChannelType, Mask, 5 >(); break;
				case( 6 ) : return BaseType::template channel< ChannelType, Mask, 4 >(); break;
				case( 7 ) : return BaseType::template channel< ChannelType, Mask, 5 >(); break;
				case( 8 ) : return BaseType::template channel< ChannelType, Mask, 4 >(); break;
			};
			
			static ChannelType t( 0 );
			return t;
		}

	private :
	
		typedef Detail::PixelRecurse< L, L::NumberOfLayouts > BaseType;

};

}; // namespace Image

}; // namespace Gander

#endif
