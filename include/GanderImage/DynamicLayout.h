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
#ifndef __GANDERIMAGE_DYNAMICLAYOUT__
#define __GANDERIMAGE_DYNAMICLAYOUT__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Assert.h"
#include "Gander/Tuple.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/DynamicLayoutBase.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{

template< class T >
struct DynamicLayout : DynamicLayoutBase< DynamicLayout< T >, T > 
{
	public :

		typedef DynamicLayout<T> Type;
		typedef Type LayoutType;
		typedef DynamicLayoutBase< DynamicLayout< T >, T > BaseType;
		typedef typename BaseType::StorageType ChannelType;
		typedef typename BaseType::StorageType StorageType;
		typedef typename BaseType::PointerType PointerType;
		typedef typename BaseType::ReferenceType ReferenceType;
		
		typedef typename Gander::template Tuple< StorageType, BaseType::NumberOfChannels, true > ChannelContainer;
		typedef typename Gander::template Tuple< StorageType *, BaseType::NumberOfChannels, true > PtrToChannelContainer;
		
		template< ChannelDefault C = Chan_None, bool DisableStaticAsserts = false >
		struct ChannelTraits : public Detail::ChannelTraitsInterface< Type >
		{
			typedef Type LayoutType;
			typedef T ChannelType;
			typedef typename LayoutType::StorageType StorageType;
			typedef typename LayoutType::PointerType PointerType;
			typedef typename LayoutType::ReferenceType ReferenceType;

			enum
			{
				LayoutIndex = 0,
			};

			ChannelTraits( const LayoutType &l, Channel channel = Chan_None ) :
				Detail::ChannelTraitsInterface< LayoutType >( l, channel )
			{
			}
		};
		
		template< int Index, EnumType Mask = Mask_All, bool DisableStaticAsserts = false  >
		struct ChannelTraitsAtIndex : public BaseType::template LayoutTraits< 0, DisableStaticAsserts >
		{
			enum
			{	
				ChannelIndexInLayout = Index,
			};
		};
	
		//! @name Methods required by the base class.
		/// These methods are required by the base class as this layout is dynamic.
		//@{
		/// Returns the channels represented by this layout.
		inline ChannelSet channels() const
		{
			return m_channels;
		}

		/// Returns the number of channels that this layout represents.
		inline unsigned int numberOfChannels() const
		{
			return m_channels.size();
		}

		/// Returns the number of channel pointers that this layout requires
		/// in order to access all of the channels that this layout represents.
		inline unsigned int numberOfChannelPointers() const
		{
			return m_channels.size();
		}
		//@}

		/// Adds the channel to the Layout and logs all pertenant information.
		void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			if( b != Brothers_None && !BrotherTraits<>::channels( b ).contains( c ) )
			{
				throw std::runtime_error( ( boost::format( "RunTimeChannelFormat: Channels \"%s\" do not belong to the specified brothers.") % c ).str() );
			}

			if( m_allBrothers.contains( c ) )
			{
				throw std::runtime_error(
					( boost::format( "RunTimeChannelFormat: Channels \"%s\" cannot be added as another set of ChannelBrothers represents it.") % c ).str()
					);
			}

			int8u step = 1;
			if( b != Brothers_None )
			{
				m_allBrothers += BrotherTraits<>::brotherMask( b );
				step = BrotherTraits<>::numberOfBrothers( b );
			}
			else
			{
				m_allBrothers += c;
			}

			ChannelSet newChannels( c - m_channels );
			ChannelSet existingChannels( c - newChannels );

			if( existingChannels.size() != 0 )
			{
				throw std::runtime_error( ( boost::format( "RunTimeChannelFormat: Cannot add channels \"%s\" as they already exist.") % existingChannels ).str() );
			}

			ChannelSet::const_iterator it( newChannels.begin() );
			ChannelSet::const_iterator end( newChannels.end() );
			for( ; it != end; ++it )
			{
				m_channels += *it;
				int index = m_channels.index( *it );
				m_steps.insert( m_steps.begin() + index, step );
			}
		}
	
	private :

		friend class DynamicLayoutBase< DynamicLayout< T >, T >;
		friend class LayoutBase< DynamicLayout< T > >;
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const
		{
			return m_channels;
		}

		/// Returns the step value for a given channel.
		template< ChannelDefault C = Chan_None >
		inline int8u _step( Channel channel = Chan_None ) const
		{
			if( C != Chan_None )
			{
				GANDER_ASSERT( m_channels.contains( C ), "Channel is not represented by this layout." );
				return m_steps[ m_channels.index( C ) ];
			}
			else
			{
				GANDER_ASSERT( m_channels.contains( channel ), "Channel is not represented by this layout." );
				return m_steps[ m_channels.index( channel ) ];
			}
			GANDER_ASSERT( 0, "No valid channel specified." )
		}

		/// Returns the index of the given channel.	
		template< EnumType C, EnumType Mask = Mask_All, bool >
		inline unsigned int _indexOfChannel() const
		{
			return m_channels.index( Channel( C ) );
		}
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const
		{
			ChannelSet i = m_channels.intersection( Mask );
			GANDER_ASSERT( Index < i.size() || DisableStaticAsserts, "Index is out of bounds when accessing a channel in a masked set." );
			return m_channels.index( i[ Index ] );
		}

		/// The channels that this format represents.
		ChannelSet m_channels;

		/// All channel brothers associated with the channels that this layout contains.
		ChannelSet m_allBrothers;

		/// The step values for each channel.
		std::vector< int8u > m_steps;
};

}; // namespace Image

}; // namespace Gander

#endif
