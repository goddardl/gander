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
/// 
/// The Layout class defines an interface for the derived classes, forcing them to implement both enum values and methods.
/// Layouts are a fundamental concept in Gander::Image. A Layout defines how a set of channels are laid out in memory along
/// with their names, storage type and whether they are dynamic - and can have more channels added to them at runtime - or not.
/// Derived classes are forced to implement a set of methods and enum values, defining a common interface that describes how
/// a set of channels should be laid out in memory. 
/// Derived classes should first decide whether they are Dynamic or Static and define the IsDynamic enum accordingly. 
/// If they are static, then they also need to define NumberOfChannels, ChannelMask and NumberOfChannelPointers.
/// If the layout is Dynamic, then it needs to override the numberOfChannels, channels and numberOfChannelPointers methods.
template< class Derived >
struct Layout
{
		
		enum
		{
			NumberOfChannels = 0,		// Required if IsDynamic == false
			ChannelMask = Mask_None,	// Required if IsDynamic == false
			NumberOfChannelPointers = 0,// Required if IsDynamic == false
			IsCompound = 0,				// Required if the derived type holds both static and dynamic layouts.
		};

	public :
			
		enum
		{
			NumberOfLayouts = 1,
			IsDynamic = false,
		};

		Layout()
		{
			GANDER_IMAGE_STATIC_ASSERT(
				( 
					( Derived::IsDynamic && ( Derived::NumberOfChannels == 0 ) && ( Derived::NumberOfChannelPointers == 0 ) && ( EnumType( Derived::ChannelMask ) == EnumType( Mask_None ) ) )
					|| ( !Derived::IsDynamic ) || ( Derived::IsCompound )
				),
				THE_DERIVED_LAYOUT_IS_DYNAMIC_AND_THEREFORE_MUST_NOT_DECLARE_ENUM_VALUES_THAT_ARE_TO_BE_USED_BY_STATIC_LAYOUTS_ONLY
			);

			GANDER_IMAGE_STATIC_ASSERT(
				(
					( ( !Derived::IsDynamic ) && ( Derived::NumberOfChannels != 0 ) && ( Derived::NumberOfChannelPointers != 0 ) && ( EnumType( Derived::ChannelMask ) != EnumType( Mask_None ) ) )
					|| Derived::IsDynamic
				),
				THE_DERIVED_LAYOUT_HASNT_DECLARED_ALL_REQUIRED_ENUM_VALUES
			);
		}
		
		//! @name Methods that need overloading by dynamic derived classes.
		/// All of theses methods need to be declared in the derived class if it is dynamic ( and therefore has set the enum value IsDynamic ).
		/// These methods do not need to be implemented if the derived class is static. 
		//@{
		inline unsigned int numberOfChannels() const
		{
			GANDER_IMAGE_STATIC_ASSERT( ( !Derived::IsDynamic ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return Derived::NumberOfChannels;
		}

		inline unsigned int numberOfChannelPointers() const
		{
			GANDER_IMAGE_STATIC_ASSERT( ( !Derived::IsDynamic ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return Derived::NumberOfChannelPointers;
		}

		inline ChannelSet channels() const
		{
			GANDER_IMAGE_STATIC_ASSERT( ( !Derived::IsDynamic ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return ChannelSet( static_cast<Gander::Image::ChannelMask>( Derived::ChannelMask ) );
		}
		//@}

		/// Returns whether the layout represents the given set of channels.
		inline bool contains( ChannelSet channels ) const
		{
			return static_cast< Derived const * >( this )->channels().contains( channels );
		}
		
		/// Returns whether the layout contains the given channel. This method can be passed either a template argument or a function argument.
		/// If the template argument is different to Chan_None then the template argument should be used. If it is equal to Chan_None, the 
		/// function argument should be used.
		template< EnumType C = Chan_None >
		inline bool containsChannel( Gander::Image::Channel c = Chan_None ) const
		{
			if( C == Chan_None || Derived::IsDynamic )
			{
				return static_cast< Derived const * >( this )->contains( ChannelSet( c ) );
			}
			else
			{
				return ( ChannelToMask< C >::Value & Derived::ChannelMask ) != 0;
			}
		}
		
		inline bool isCompound() const
		{
			return Derived::IsCompound;
		}

		inline bool isDynamic() const
		{
			return Derived::IsDynamic;
		}

		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int maskedChannelIndex() const
		{
			return static_cast< Derived const * >( this )->template _maskedChannelIndex< Index, Mask, DisableStaticAsserts >();
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

		template< EnumType Channel, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline unsigned int indexOfChannel() const
		{
			return static_cast< Derived const * >( this )->template _indexOfChannel< Channel, Mask, DisableStaticAsserts >();
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
		
		template< unsigned Index, bool DisableStaticAsserts = false, class ReturnType = Derived >
		inline ReturnType &child()
		{
			return *( static_cast< Derived * >( this ) );
		}

		template< class L > inline bool operator == ( L const &rhs ) const { return static_cast< Derived const * >( this )->equalTo( rhs ); }
		template< class L > inline bool operator != ( L const &rhs ) const { return !static_cast< Derived const * >( this )->equalTo( rhs ); }

	private :	

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
				rhs.channels() == static_cast< Derived const * >( this )->channels() &&
				std::is_same< typename L::StorageType, typename Derived::StorageType >::value
		   );
		}
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
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
