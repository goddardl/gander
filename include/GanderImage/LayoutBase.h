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

/// This simple little struct is used to mask the default constructor on the container class
/// and to assert that it is initialized with it's associated layout.
template< class Layout, class Container >
struct ChannelPointerContainerWrapper : public Container
{
	public :
		typedef Layout LayoutType;

		ChannelPointerContainerWrapper( const Layout &layout ) :
			Container( layout.numberOfChannelPointers() )
		{
		}
};

/// This simple little struct is used to mask the default constructor on the container class
/// and to assert that it is initialized with it's associated layout.
template< class Layout, class Container >
struct ChannelContainerWrapper : public Container
{
	public :
	
		typedef Layout LayoutType;

		ChannelContainerWrapper( const Layout &layout ) :
			Container( layout.numberOfChannels() )
		{
		}
};

}; // namespace Detail

/// The base class for defining how a set of channels are grouped together and of what type they are.
/// 
/// The LayoutBase class defines an interface for the derived classes, forcing them to implement both enum values and methods.
/// Layouts are a fundamental concept in Gander::Image. A Layout defines how a set of channels are laid out in memory along
/// with their names and storage type. Derived classes are forced to implement a set of methods and enum values, defining a
/// common interface that describes how a set of channels should be laid out in memory. 
template< class Derived, class DataType = float >
struct LayoutBase
{
		
		enum
		{
			NumberOfChannels = 0,
			ChannelMask = Mask_None,
			NumberOfChannelPointers = 0,
			IsCompound = 0,
		};

	public :
		
		typedef typename Gander::template TypeTraits< DataType >::Type ChannelType;
		typedef typename Gander::template TypeTraits< DataType >::StorageType StorageType;
		typedef typename Gander::template TypeTraits< DataType >::PointerType PointerType;
		typedef typename Gander::template TypeTraits< DataType >::ReferenceType ReferenceType;
		typedef typename Gander::template TypeTraits< DataType >::ConstReferenceType ConstReferenceType;
		
		enum
		{
			NumberOfLayouts = 1,
		};

		LayoutBase();
		
		//! @name Channel methods.
		//@{
		static inline unsigned int numberOfChannels();
		static inline unsigned int numberOfChannelPointers();
		static inline ChannelSet channels();
		static inline ChannelSet requiredChannels();
		/// Returns whether the layout represents the given set of channels.
		static inline bool contains( ChannelSet channels );
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
		
		template< class ContainerType, ChannelDefault C, bool DisableStaticAsserts = false >
		inline ReferenceType channel( ContainerType &container );

		template< class ContainerType, ChannelDefault C, bool DisableStaticAsserts = false >
		inline ConstReferenceType channel( const ContainerType &container ) const;

		template< class ContainerType, EnumType Index, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline ReferenceType channelAtIndex( ContainerType &container );
		
		template< class ContainerType, EnumType Index, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline ConstReferenceType channelAtIndex( const ContainerType &container ) const;
		
		template< class ContainerType, EnumType Mask = Mask_All >
		inline ReferenceType channelAtIndex( ContainerType &container, unsigned int index );
		
		template< class ContainerType, EnumType Mask = Mask_All >
		inline ConstReferenceType channelAtIndex( const ContainerType &container, unsigned int index ) const;
		
		template< class ContainerType, class Type >
		inline void setChannel( ContainerType &container, Channel c, const Type &value );
		
		template< class ContainerType, class Type >
		inline Type getChannel( const ContainerType &container, Channel c ) const;

		template< class ContainerType >
		inline void setChannelPointer( ContainerType &container, Channel channel, PointerType pointer );

		template< class ContainerType >
		inline void setChannelPointer( ContainerType &container, Channel channel, void *pointer );

		template< EnumType MaskedIndex, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		struct MaskedChannelIndex;
		
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

		/// A function for comparing two Layouts. The overloaded equality operators call this method on the Derived class,
		/// allowing the Derived class to overload this function.
		template< class L >
		inline bool equalTo( const L &rhs ) const;
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, Gander::Image::ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const;
};

}; // namespace Image

}; // namespace Gander

// The implementation of the LayoutBase class.
#include "LayoutBase.inl"

#endif
