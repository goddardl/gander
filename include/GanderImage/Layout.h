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
#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{

namespace Detail
{

template< class LayoutType >
struct ChannelTraitsInterface
{
	public :
	
		ChannelTraitsInterface( const LayoutType &l, Channel channel = Chan_None ) :
			m_step( l.template step( channel ) )
		{
		}

		int8u step() const
		{
			return m_step;
		}

	private :

		int8u m_step;
};

}; // namespace Detail

/// The base class for defining how a set of channels are grouped together and of what type they are.
/// The Layout class defines an interface for the derived classes using static polymorphism.
/// The Derived classes need to implement several Enum values, a type declaration for the StorageType of the channels,
/// a several accessor methods.
/// In addition to these, the equalTo method can be optionally overridden.
template< class Derived >
struct Layout
{

	public :

		enum
		{
			NumberOfLayouts = 1,
			IsDynamic = false,
		};
		
		template< EnumType Index, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int maskedChannelIndex() const
		{
			return static_cast< Derived const * >( this )->template _maskedChannelIndex< Index, Mask, DisableStaticAsserts >();
		}

		inline ChannelSet channels() const
		{
			return static_cast< Derived const * >( this )->_channels();
		}

		inline unsigned int numberOfChannels() const
		{
			return static_cast< Derived const * >( this )->_numberOfChannels();
		}

		inline ChannelSet requiredChannels() const
		{
			return static_cast< Derived const * >( this )->_requiredChannels();
		}
		
		inline void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			static_cast< Derived * >( this )->_addChannels( c, b );
		}
		
		/// Returns the step value for a given channel.
		template< ChannelDefault C = Chan_None >
		inline int8u step( Channel channel = Chan_None ) const
		{
			GANDER_ASSERT( ( C == Chan_None || channel == Chan_None || channel == C ), "Specify either a compile-time argument or a runtime parameter but not both." );
			return static_cast< Derived const * >( this )->_step<C>( channel );
		}
		
		/// Returns whether the layout represents the given channel.
		inline bool contains( ChannelSet channels ) const
		{
			return static_cast< Derived const * >( this )->_contains( channels );
		}
				
		template< EnumType Channel, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline unsigned int indexOfChannel() const
		{
			return static_cast< Derived const * >( this )->template _indexOfChannel< Channel, Mask, DisableStaticAsserts >();
		}

		template< EnumType C = Chan_None >
		inline bool containsChannel( Gander::Image::Channel c = Chan_None ) const
		{
			return static_cast< Derived const * >( this )->template _containsChannel< C >( c );
		}

		template< EnumType Index, bool DisableStaticAsserts = false >
		struct LayoutTraits
		{
			GANDER_IMAGE_STATIC_ASSERT( Index == 0 || DisableStaticAsserts, THE_REQUESTED_LAYOUT_AT_THE_GIVEN_INDEX_DOES_NOT_EXIST );
			typedef Derived LayoutType;
			typedef typename LayoutType::StorageType StorageType;
		
			enum
			{
				LayoutIndex = Index,
			};
		};

		template< class L > inline bool operator == ( L const &rhs ) const { return static_cast< Derived const * >( this )->equalTo( rhs ); }
		template< class L > inline bool operator != ( L const &rhs ) const { return !static_cast< Derived const * >( this )->equalTo( rhs ); }

	private :	

		/// Deried classes need to override this function to return the channels that this layout represents.
		/// If they do not, a static assertion prevents the code from compiling.
		inline ChannelSet _channels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return ChannelSet(); // We never get here.
		}

		/// Deried classes need to override this function to return the number of channels that this layout represents.
		/// If they do not, a static assertion prevents the code from compiling.
		inline unsigned int _numberOfChannels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}

		/// Returns the step value for a given channel.
		template< ChannelDefault C = Chan_None >
		inline int8u _step( Channel channel = Chan_None ) const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}

		/// Returns whether the layout represents the given set of channels.
		inline bool _contains( ChannelSet channels ) const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}
		
		/// Returns whether the layout contains the given channel. This method can be passed either a template argument or a function argument.
		/// If the template argument is different to Chan_None then the template argument should be used. If it is equal to Chan_None, the 
		/// function argument should be used.
		template< EnumType C = Chan_None >
		inline bool _containsChannel( Gander::Image::Channel c = Chan_None ) const
		{
			if( C == Chan_None )
			{
				return static_cast< Derived const * >( this )->_contains( ChannelSet( c ) );
			}
			else
			{
				return ( ChannelToMask< C >::Value & Derived::ChannelMask ) != 0;
			}
		}

		/// Adds the channel to the Layout and logs all pertenant information.
		inline void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			GANDER_IMAGE_STATIC_ASSERT( Derived::IsDynamic, THE_LAYOUT_MUST_BE_DYNAMIC_IN_ORDER_TO_ADD_CHANNELS_TO_IT )
		}

		/// A function for comapring two Layouts. The overloaded equality operators call this method on the Derived class,
		/// allowing the Derived class to overload this function.
		template< class L >
		inline bool equalTo( const L &rhs ) const
		{
			return (
				rhs.channels() == channels() &&
				std::is_same< typename L::StorageType, typename Derived::StorageType >::value
		   );
		}
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0;
		}
	
		/// Returns the index of a channel within the layout.	
		template< EnumType Channel, EnumType Mask = Mask_All, bool >
		inline unsigned int _indexOfChannel() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0;
		}
};

}; // namespace Image

}; // namespace Gander

#endif
