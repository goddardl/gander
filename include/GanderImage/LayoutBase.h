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
#ifndef __GANDERIMAGE_LAYOUTBASE__
#define __GANDERIMAGE_LAYOUTBASE__

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
/// The LayoutBase class defines an interface for the derived classes, forcing them to implement both enum values and methods.
/// Layouts are a fundamental concept in Gander::Image. A Layout defines how a set of channels are laid out in memory along
/// with their names, storage type and whether they are dynamic - and can have more channels added to them at runtime - or not.
/// Derived classes are forced to implement a set of methods and enum values, defining a common interface that describes how
/// a set of channels should be laid out in memory. 
/// Derived classes should first decide whether they are Dynamic or Static and define the IsDynamic enum accordingly. 
/// If they are static, then they also need to define NumberOfChannels, ChannelMask and NumberOfChannelPointers.
/// If the layout is Dynamic, then it needs to override the numberOfChannels, channels and numberOfChannelPointers methods.
template< class Derived >
struct LayoutBase
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

		LayoutBase();
		
		//! @name Methods that need overloading by dynamic derived classes.
		/// All of theses methods need to be declared in the derived class if it is dynamic ( and therefore has set the enum value IsDynamic ).
		/// These methods do not need to be implemented if the derived class is static. 
		//@{
		inline unsigned int numberOfChannels() const;
		inline unsigned int numberOfChannelPointers() const;
		inline ChannelSet channels() const;
		//@}

		//! @name Dynamic methods.
		/// All of these methods should be implemented by any derived class that defines the IsDynamic flag.
		/// These methods provide an interface to the layout that allow the structure of the channels to be
		/// modified.
		//@{
		/// Adds a set of channels to the layout. Whether the channels are brothers can also optionally be defined.
		inline void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None );
		//@}
		
		//! @name Compound methods.
		/// All of these methods should be implemented by any derived class that defines the IsCompound flag.
		/// These methods allow the internal layouts, held by the derived compound layout, to be queried and
		/// accessed.
		//@{
		/// This method should return a reference to the layout at the supplied index. The type of the child
		/// layout is supplied so that it can be used for static verification or with c-style cast. The default
		/// implementation which is used by derived classes that are not compound just returns an instance of
		/// this layout.
		template< unsigned Index, bool DisableStaticAsserts = false, class ReturnType = Derived >
		inline ReturnType &child();
		//@}
		
		//! @name Channel pointer interface.
		/// These methods are used when accessing channel data using a set of pointers by the ReferenceType class.
		/// At it's most simplest, a set of channels can be accessed using one pointer to each of them. However,
		/// if the channels are interleaved together then we only need to store a pointer to the first one and an
		/// offset to the others from it. The purpose of these methods, which a derived class should implement,
		/// is to provide this information for a given channel index.
		//@{
		/// Returns the index to the base pointer for the given channel.
		template< EnumType Index >
		inline int pointerIndex() const;
		/// Returns the offset to be applied to the base pointer in order to access the given channel.
		template< EnumType Index >
		inline int pointerOffset() const;
		/// Returns the value to be added to a pointer to a channel in order to increment it to the next pixel location.
		template< ChannelDefault C = Chan_None >
		inline int8u step( Channel channel = Chan_None ) const;
		//@}
		
		/// Returns whether the layout represents the given set of channels.
		inline bool contains( ChannelSet channels ) const;
		
		/// Returns whether the layout contains the given channel. This method can be passed either a template argument or a function argument.
		/// If the template argument is different to Chan_None then the template argument should be used. If it is equal to Chan_None, the 
		/// function argument should be used.
		template< EnumType C = Chan_None >
		inline bool containsChannel( Gander::Image::Channel c = Chan_None ) const;
		
		/// Returns true if the Layout contains other layouts.
		inline bool isCompound() const;

		/// Returns true if the Layout supports the Dynamic methods that allow the number of channels and their structure to be manipulated.
		inline bool isDynamic() const;

		template< EnumType Channel, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline unsigned int indexOfChannel() const;
		
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int maskedChannelIndex() const;
		
		inline ChannelSet requiredChannels() const;
		
		template< class L > inline bool operator == ( L const &rhs ) const { return static_cast< Derived const * >( this )->equalTo( rhs ); }
		template< class L > inline bool operator != ( L const &rhs ) const { return !static_cast< Derived const * >( this )->equalTo( rhs ); }

		template< EnumType Index, bool DisableStaticAsserts = false >
		struct LayoutTraits
		{
			GANDER_IMAGE_STATIC_ASSERT( Index == 0 || DisableStaticAsserts, THE_REQUESTED_LAYOUT_AT_THE_GIVEN_INDEX_DOES_NOT_EXIST );
			typedef Derived LayoutType;
			typedef typename LayoutType::StorageType StorageType;
			typedef typename LayoutType::ReferenceType ReferenceType;
		
			enum
			{
				LayoutIndex = Index,
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
		
		template< int Index = -1, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		struct ChannelTraitsAtIndex
		{
			GANDER_IMAGE_STATIC_ASSERT( ( Index != -1 ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_TRAITS_STRUCTS_REQUIRED_BY_THE_BASE_CLASS );
			
			// Derived classes of Layout need to implement this struct and provide the following:
			//
			// - An enum that maps the index ( the 'Index' template argument ) of a channel in set of channels
			// ( defined by the 'Mask' template argument ) into the index of the same channel in the layout. 
			// enum
			// {
			//		ChannelIndexInLayout = Index,
			// };
		};
		
	private :	

		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const;

		/// Returns the step value for a given channel.
		template< ChannelDefault C = Chan_None >
		inline int8u _step( Channel channel = Chan_None ) const;

		/// Adds the channel to the Layout and logs all pertenant information.
		inline void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None );

		/// A function for comparing two Layouts. The overloaded equality operators call this method on the Derived class,
		/// allowing the Derived class to overload this function.
		template< class L >
		inline bool equalTo( const L &rhs ) const;
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const;
	
		/// Returns the index of a channel within the layout.	
		template< EnumType Channel, EnumType Mask = Mask_All, bool >
		inline unsigned int _indexOfChannel() const;
		
		/// Returns the index to the base pointer for the given channel.
		template< EnumType Index >
		inline int _pointerIndex() const;
		
		/// Returns the offset to be applied to the base pointer in order to access the given channel.
		template< EnumType Index >
		inline int _pointerOffset() const;

};

}; // namespace Image

}; // namespace Gander

// The implementation of the LayoutBase class.
#include "LayoutBase.inl"

#endif
