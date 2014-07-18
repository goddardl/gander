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
#ifndef __GANDERIMAGE_LAYOUT__
#define __GANDERIMAGE_LAYOUT__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Assert.h"
#include "Gander/Common.h"
#include "Gander/Tuple.h"
#include "Gander/StaticAssert.h"
#include "Gander/Cache.h"

#include "GanderImage/Common.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{


template< class Derived >
struct Layout
{
	public :
			
		enum
		{
			NumberOfLayouts = 0
		};

		typedef Layout< Derived > Type;
		
		template< EnumType Index, bool DisableStaticAsserts = false >
		struct LayoutTraits
		{
			GANDER_IMAGE_STATIC_ASSERT( Index == 0 || DisableStaticAsserts, THE_REQUESTED_LAYOUT_AT_THE_GIVEN_INDEX_DOES_NOT_EXIST );
			typedef Derived LayoutType;
			typedef typename LayoutType::ChannelType ChannelType;
			typedef typename LayoutType::PointerType PointerType;
			typedef typename LayoutType::ReferenceType ReferenceType;
			typedef typename LayoutType::ConstReferenceType ConstReferenceType;
			typedef typename LayoutType::ChannelContainerType ChannelContainerType;
			typedef typename LayoutType::ChannelPointerContainerType ChannelPointerContainerType;
		
			enum
			{
				LayoutIndex = Index,
			};
		};
		
		template< ChannelDefault C >
		struct ChannelToLayoutIndex
		{
			enum
			{
				Value = 0
			};
		};
		
		template< ChannelDefault C = Chan_None >
		struct ChannelTraits
		{
			GANDER_IMAGE_STATIC_ASSERT( ( C != Chan_None ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_TRAITS_STRUCTS_REQUIRED_BY_THE_BASE_CLASS );

			// Derived classes of Layout need to implement this struct and provide the following:
			//
			// - A typedef of the Layout that contains the ChannelTraits struct.
			// typedef Type LayoutType;
			// 
			// - A typedef that defines the type of the channels that the layout represents.
			// typedef T StorageType;
			// 
			// - An enum that defines the index of the layout that the channel C belongs to. If the 
			// layout is not a compound layout the this can be set to 0.	
			// enum
			// {
			//		LayoutIndex = 0,
			// };
		};
		
		//! @name Channel methods.
		//@{
		static inline unsigned int numberOfChannels() { return Gander::template EnumHelper<Derived::ChannelMask>::NumberOfSetBits; }
		static inline ChannelSet channels() { return ChannelSet( Gander::Image::ChannelMask( Derived::ChannelMask ) ); }
		static inline bool contains( Gander::Image::ChannelDefault channel ) { return ChannelSet( Gander::Image::ChannelMask( Derived::ChannelMask ) ).contains( channel ); }
		static inline bool contains( Gander::Image::ChannelSet set ) { return ChannelSet( Gander::Image::ChannelMask( Derived::ChannelMask ) ).contains( set ); }
		static inline bool contains( Gander::Image::ChannelMask mask ) { return ChannelSet( Gander::Image::ChannelMask( Derived::ChannelMask ) ).contains( ChannelSet( mask ) ); }
		//@}
		
		// Note: We set the ContainerType argument to None as it is resolved by the type of the argument that is passed
		// into it. Doing this allows the return type to be optionally specified without having to set the ContainerType
		template< class Type = DefaultChannelType, class ContainerType = float >
		inline static void setChannel( ContainerType *container, Channel c, const Type &value )
		{
			GANDER_ASSERT( Derived::contains( c ), "Channel does not exist in this Layout." );
			switch( c )
			{
				case( 1 ) : Derived::template channel< ChannelDefault( 1 ), true >( container ) = value; break;
				case( 2 ) : Derived::template channel< ChannelDefault( 2 ), true >( container ) = value; break;
				case( 3 ) : Derived::template channel< ChannelDefault( 3 ), true >( container ) = value; break;
				case( 4 ) : Derived::template channel< ChannelDefault( 4 ), true >( container ) = value; break;
				case( 5 ) : Derived::template channel< ChannelDefault( 5 ), true >( container ) = value; break;
				case( 6 ) : Derived::template channel< ChannelDefault( 6 ), true >( container ) = value; break;
				case( 7 ) : Derived::template channel< ChannelDefault( 7 ), true >( container ) = value; break;
				case( 8 ) : Derived::template channel< ChannelDefault( 8 ), true >( container ) = value; break;
				case( 9 ) : Derived::template channel< ChannelDefault( 9 ), true >( container ) = value; break;
				case( 10 ) : Derived::template channel< ChannelDefault( 10 ), true >( container ) = value; break;
				default : GANDER_ASSERT( 0, "Channel does not exist in this Layout." ); break;
			}
		}
	
		// Note: We set the ContainerType argument to None as it is resolved by the type of the argument that is passed
		// into it. Doing this allows the return type to be optionally specified without having to set the ContainerType
		template< class Type = DefaultChannelType, class ContainerType = Detail::None >
		inline static Type getChannel( const ContainerType * const container, Channel c )
		{
			GANDER_ASSERT( Derived::contains( c ), "Channel does not exist in this Layout." );
			switch( c )
			{
				case( 1 ) : return Derived::template channel< ChannelDefault( 1 ), true >( container ); break;
				case( 2 ) : return Derived::template channel< ChannelDefault( 2 ), true >( container ); break;
				case( 3 ) : return Derived::template channel< ChannelDefault( 3 ), true >( container ); break;
				case( 4 ) : return Derived::template channel< ChannelDefault( 4 ), true >( container ); break;
				case( 5 ) : return Derived::template channel< ChannelDefault( 5 ), true >( container ); break;
				case( 6 ) : return Derived::template channel< ChannelDefault( 6 ), true >( container ); break;
				case( 7 ) : return Derived::template channel< ChannelDefault( 7 ), true >( container ); break;
				case( 8 ) : return Derived::template channel< ChannelDefault( 8 ), true >( container ); break;
				case( 9 ) : return Derived::template channel< ChannelDefault( 9 ), true >( container ); break;
				case( 10 ) : return Derived::template channel< ChannelDefault( 10 ), true >( container ); break;
				default : GANDER_ASSERT( 0, "Channel does not exist in this Layout." ); break;
			}
		}

		template< class T >
		inline bool operator == ( const T &rhs ) const
		{
			return std::template is_same< Derived, T >::value;
		}
		
		template< class T >
		inline bool operator != ( const T &rhs ) const
		{
			return !std::template is_same< Derived, T >::value;
		}

};

}; // namespace Image

}; // namespace Gander

#endif
