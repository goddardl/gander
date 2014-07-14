//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014, Luke Goddard. All rights reserved.
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
#ifndef __GANDERIMAGE_PIXELLAYOUT__
#define __GANDERIMAGE_PIXELLAYOUT__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Assert.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/LayoutBase.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"
#include "GanderImage/CompoundLayoutContainer.h"

namespace Gander
{

namespace Image
{

/// An empty struct for use as a default template argument to the CompoundLayout class.
namespace Detail { struct None { struct LayoutTraits { }; enum { ChannelMask = 0 }; }; };

/// Forward declaration of the CompoundLayout class.
template<
	class T0 = Detail::None, class T1 = Detail::None, class T2 = Detail::None, class T3 = Detail::None,
	class T4 = Detail::None, class T5 = Detail::None, class T6 = Detail::None, class T7 = Detail::None
> struct CompoundLayout;

namespace Detail
{

/// The ChannelIndexHelper class is a helper class that returns the index of the Layout within the template arguments that contains the channel of the given index.
template< EnumType NumberOfLayouts, EnumType CompleteChannelMask, EnumType ChannelIndex, EnumType M = Mask_All,
	class T0 = None, class T1 = None, class T2 = None, class T3 = None, class T4 = None, class T5 = None, class T6 = None, class T7 = None >
struct ChannelIndexHelper;

/// Forward declaration of the ChannelToLayoutIndex class which calculates the index of the Layout within template parameters that contains the given channel.
template<
	ChannelDefault C, EnumType NumberOfLayouts,
	class T0 = None, class T1 = None, class T2 = None, class T3 = None,
	class T4 = None, class T5 = None, class T6 = None, class T7 = None
> struct ChannelToLayoutIndex;

/// The last recursive base of the ChannelToLayoutIndex helper class.
template< ChannelDefault C, EnumType NumberOfLayouts >
struct ChannelToLayoutIndex< C, NumberOfLayouts, None, None, None, None, None, None, None, None > { enum { Count = -1, Value = 0 }; };

/// The last recursive base of the ChannelToLayoutIndex helper class.
template< ChannelDefault C, EnumType NumberOfLayouts, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct ChannelToLayoutIndex : public ChannelToLayoutIndex< C, NumberOfLayouts, T1, T2, T3, T4, T5, T6, T7, None >
{
	typedef ChannelToLayoutIndex< C, NumberOfLayouts, T1, T2, T3, T4, T5, T6, T7, None> BaseType;
	enum
	{
		Count = T0::NumberOfChannels > 0 ? BaseType::Count + 1 : BaseType::Count,
		Value = ( ChannelToMask<C>::Value & T0::ChannelMask ) != 0 ? NumberOfLayouts - Count - 1 : BaseType::Value
	};
};

/// The last recursive base of the ChannelIndexHelper helper class. 
template< EnumType NumberOfLayouts, EnumType CompleteChannelMask, EnumType ChannelIndex, EnumType ChannelMask >
struct ChannelIndexHelper< NumberOfLayouts, CompleteChannelMask, ChannelIndex, ChannelMask, None, None, None, None, None, None, None, None >
{
	enum
	{
		NumberOfChannels = 0,
		NumberOfMaskedChannels = 0,
		LayoutNumber = 0,
		Value = -1,
		CompleteNumberOfMaskedChannels = Gander::template EnumHelper< CompleteChannelMask & ChannelMask >::NumberOfSetBits,
		ChannelIndexInLayout = CompleteNumberOfMaskedChannels < ( ChannelIndex + 1 ) ? ChannelIndex - CompleteNumberOfMaskedChannels : 0,
	};
};

/// The body of the ChannelIndexHelper class.
template< EnumType NumberOfLayouts, EnumType CompleteChannelMask, EnumType ChannelIndex, EnumType M, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct ChannelIndexHelper : public ChannelIndexHelper< NumberOfLayouts, CompleteChannelMask, ChannelIndex, M, T1, T2, T3, T4, T5, T6, T7, None >
{
	typedef ChannelIndexHelper< NumberOfLayouts, CompleteChannelMask, ChannelIndex, M, T1, T2, T3, T4, T5, T6, T7, None> BaseType;
	enum
	{
		NumberOfChannels = BaseType::NumberOfChannels + T0::NumberOfChannels,
		NumberOfMaskedChannelsInT0 = Gander::template EnumHelper< T0::ChannelMask & M >::NumberOfSetBits,
		NumberOfMaskedChannels = BaseType::NumberOfMaskedChannels + NumberOfMaskedChannelsInT0,
		LayoutNumber = BaseType::LayoutNumber + 1,
		InverseIndex = BaseType::CompleteNumberOfMaskedChannels - ChannelIndex,
		Value = InverseIndex <= NumberOfMaskedChannels && InverseIndex > BaseType::NumberOfMaskedChannels && NumberOfMaskedChannelsInT0 > 0 ? NumberOfLayouts - LayoutNumber : BaseType::Value,
		ChannelIndexInLayout = InverseIndex <= NumberOfMaskedChannels && InverseIndex > BaseType::NumberOfMaskedChannels && NumberOfMaskedChannelsInT0 > 0 ?
			NumberOfMaskedChannels - InverseIndex : BaseType::ChannelIndexInLayout,
	};
};

/// Provides a mechanism for switching between type declarations.
/// Given a class 'T' that defines declarations of Type0-7, TypeSwitch can be used to selectively declare
/// one of the Types as 'Type'.
/// For example :
/// TypeSwitch< T, 3 >::Type // Choose Type3 of class T.
template< class T, EnumType TemplateIndex > struct TypeSwitch{ typedef typename T::Type0 Type; };
template< class T > struct TypeSwitch<T, 0> { typedef typename T::Type0 Type; };
template< class T > struct TypeSwitch<T, 1> { typedef typename T::Type1 Type; };
template< class T > struct TypeSwitch<T, 2> { typedef typename T::Type2 Type; };
template< class T > struct TypeSwitch<T, 3> { typedef typename T::Type3 Type; };
template< class T > struct TypeSwitch<T, 4> { typedef typename T::Type4 Type; };
template< class T > struct TypeSwitch<T, 5> { typedef typename T::Type5 Type; };
template< class T > struct TypeSwitch<T, 6> { typedef typename T::Type6 Type; };
template< class T > struct TypeSwitch<T, 7> { typedef typename T::Type7 Type; };

/// Forward declaration of the recursive base of the CompoundLayout class.
template< class Derived, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct CompoundLayoutRecurse;

template < class Derived >
struct CompoundLayoutRecurseBase : public LayoutBase< Derived >
{
	public :

		typedef float StorageType;
			
		template< EnumType LayoutIndexValue, bool DisableStaticAsserts = false >
		struct LayoutTraits
		{
			GANDER_IMAGE_STATIC_ASSERT(
				( EnumType( Derived::NumberOfLayouts ) > LayoutIndexValue || DisableStaticAsserts ),
				THE_REQUESTED_LAYOUT_AT_THE_GIVEN_INDEX_DOES_NOT_EXIST
			);

			enum
			{
				LayoutIndex = LayoutIndexValue,
			};

			typedef typename Detail::TypeSwitch< Derived, LayoutIndex >::Type LayoutType;
			typedef typename LayoutType::ChannelType ChannelType;
			typedef typename LayoutType::StorageType StorageType;
			typedef typename LayoutType::PointerType PointerType;
			typedef typename LayoutType::ReferenceType ReferenceType;
			typedef typename LayoutType::ConstReferenceType ConstReferenceType;
			typedef typename LayoutType::ChannelContainerType ChannelContainerType;
			typedef typename LayoutType::ChannelPointerContainerType ChannelPointerContainerType;
		};

	protected :

		enum
		{
			Iteration = 0,
			NumberOfChannels = 0,
			NumberOfChannelPointers = 0,
			ChannelMask = 0,
			ChannelPointerMask = 0,
		};
	
		template< EnumType ChannelIndex, EnumType M = Mask_All >
		struct ChannelIndexHelper
		{
			private :

				typedef typename Detail::ChannelIndexHelper
				<
					Derived::NumberOfLayouts,
					Derived::ChannelMask,
					ChannelIndex,
					M,
					typename Derived::Type0, typename Derived::Type1, typename Derived::Type2, typename Derived::Type3,
					typename Derived::Type4, typename Derived::Type5, typename Derived::Type6, typename Derived::Type7
				> IndexHelper;

			public :

				enum
				{
					Value = IndexHelper::Value,
					ChannelIndexInLayout = IndexHelper::ChannelIndexInLayout,
				};
		};
		
		template< ChannelDefault C >
		struct ChannelToLayoutIndex
		{
			enum
			{
				Value = Detail::ChannelToLayoutIndex
				<
					C,
					Derived::NumberOfLayouts,
					typename Derived::Type0, typename Derived::Type1, typename Derived::Type2, typename Derived::Type3,
					typename Derived::Type4, typename Derived::Type5, typename Derived::Type6, typename Derived::Type7
				>::Value,
			};
		};
	
	public :

		/// The public interface to a Channel's traits.
		/// This struct - templated using a ChannelDefault - declares a range of Types and values that can be used to 
		/// describe the characteristics (traits) of a particular channel within the layout of the image.
		/// The most useful type that this struct declares is ChannelType. ChannelType is the type used to represent the
		/// channel within the image. ChannelTraits also reveals the LayoutType that the channel was declared within
		/// along with useful enum values such the Channel's ChannelBrothers.
		template< ChannelDefault C = Chan_None, bool DisableStaticAsserts = false  >
		struct ChannelTraits  
		{
			public :
	
				typedef typename LayoutTraits< ChannelToLayoutIndex< C >::Value, DisableStaticAsserts >::LayoutType LayoutType;
				typedef typename LayoutType::ChannelType ChannelType;
				typedef typename LayoutType::StorageType StorageType;
				typedef typename LayoutType::PointerType PointerType;
				typedef typename LayoutType::ReferenceType ReferenceType;
				typedef typename LayoutType::ConstReferenceType ConstReferenceType;
				typedef typename LayoutType::ChannelContainerType ChannelContainerType;
				typedef typename LayoutType::ChannelPointerContainerType ChannelPointerContainerType;
				
				enum
				{
					LayoutIndex = ChannelToLayoutIndex< C >::Value
				};

			private :
			
				// Assert that the layout actually contains the requested channel.
				GANDER_IMAGE_STATIC_ASSERT(
					( ( ( LayoutType::ChannelMask & ChannelToMask<C>::Value ) != 0 ) || ( C == Chan_None ) || ( DisableStaticAsserts ) ),
					CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT
				);
		};
		
		template< int ChannelIndex, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		struct ChannelTraitsAtIndex : public LayoutTraits< ChannelIndexHelper< ChannelIndex, Mask >::Value, DisableStaticAsserts >
		{
			private :

				typedef ChannelIndexHelper< ChannelIndex, Mask > BaseType;

			public :

				enum
				{
					LayoutIndex = BaseType::Value,
					ChannelIndexInLayout = BaseType::ChannelIndexInLayout,
				};
		};

		template< class L > friend class Pixel;
		template< class L, EnumType N > friend class PixelRecurse;

};

/// The last recursive base of the CompoundLayout class.
template < class Derived >
struct CompoundLayoutRecurse< Derived, None, None, None, None, None, None, None, None > : public CompoundLayoutRecurseBase< Derived >
{
	private :

		typedef CompoundLayoutRecurseBase< Derived > BaseType;

	public :

		/// Increments all channel pointers in the container by v.
		template< class CompoundChannelPointerContainer >
		inline void increment( CompoundChannelPointerContainer &container, int v )
		{
		}
		
		/// Increments the given channel pointer in the container by v.
		template< class CompoundChannelPointerContainer >
		inline void increment( CompoundChannelPointerContainer &container, Channel channel, int v )
		{
		}

	protected :
		
		template< unsigned Index, bool DisableStaticAsserts = false, class ReturnType = typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType >
		inline ReturnType &child()
		{
			GANDER_ASSERT( 0, "The requested layout does not exist at the given index" );

			static typename BaseType::template LayoutTraits< Index, DisableStaticAsserts > l; // We never get here but we still have to return something to keep the compiler happy.
			return ( ReturnType & ) l;
		}
		
		template< unsigned Index, bool DisableStaticAsserts = false, class ReturnType = typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType >
		inline const ReturnType &child() const
		{
			GANDER_ASSERT( 0, "The requested layout does not exist at the given index" );

			static typename BaseType::template LayoutTraits< Index, DisableStaticAsserts > l; // We never get here but we still have to return something to keep the compiler happy.
			return ( const ReturnType & ) l;
		}
		
};

/// The body of the recursive CompoundLayoutRecurse class.
template< class Derived, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct CompoundLayoutRecurse : public CompoundLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None >
{
	public :

		typedef CompoundLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None> BaseType;

		enum
		{
			Iteration = BaseType::Iteration + 1,
			NumberOfChannels = BaseType::NumberOfChannels + T0::NumberOfChannels,
			ChannelMask = BaseType::ChannelMask | T0::ChannelMask,
			NumberOfChannelPointers = BaseType::NumberOfChannelPointers + T0::NumberOfChannelPointers,
			ChannelPointerMask = T0::ChannelPointerMask | BaseType::ChannelPointerMask,
		};

		// Assert that the template arguments have been supplied with the Pixels representing channels ordered
		// from left to right in order of the channels of the lowest value first.
		GANDER_IMAGE_STATIC_ASSERT(
			( ( EnumType( T0::ChannelMask ) < EnumType( T1::ChannelMask ) ) || ( std::is_same< T1, Detail::None >::value == true ) ),
			TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_BE_SUPPLIED_IN_ORDER_OF_CHANNEL_VALUE
		);

		// Assert that the template arguments each relate to unique channels.
		GANDER_IMAGE_STATIC_ASSERT( ( ( EnumType( T0::ChannelMask ) & BaseType::ChannelMask ) == 0 ), TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_ALL_REPRESENT_UNIQUE_CHANNELS );

		/// Increments all channel pointers in the container by v.
		template< class CompoundChannelPointerContainer >
		inline void increment( CompoundChannelPointerContainer &container, int v )
		{
			m_layout.increment( container.template child< CompoundChannelPointerContainer::LayoutType::NumberOfLayouts - Iteration >(), v );
			BaseType::template increment< CompoundChannelPointerContainer >( container, v );
		}
		
		/// Increments the given channel pointer in the container by v.
		template< class CompoundChannelPointerContainer >
		inline void increment( CompoundChannelPointerContainer &container, Channel channel, int v )
		{
			if( m_layout.contains( channel ) )
			{
				m_layout.increment( container.template child< CompoundChannelPointerContainer::LayoutType::NumberOfLayouts - Iteration >(), channel, v );
			}
			else
			{
				BaseType::template increment< CompoundChannelPointerContainer >( container, channel, v );
			}
		}
		
		template< unsigned Index, bool DisableStaticAsserts = false, class ReturnType = typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType >
		inline ReturnType &child()
		{
			if( std::is_same< ReturnType, T0 >::value )
			{
				return ( ReturnType & ) m_layout;
			}
			else
			{
				return ( ReturnType & ) BaseType::template child< Index, DisableStaticAsserts, ReturnType >();
			}
		}

		template< unsigned Index, bool DisableStaticAsserts = false, class ReturnType = typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType >
		inline const ReturnType &child() const
		{
			if( std::is_same< ReturnType, T0 >::value )
			{
				return ( const ReturnType & ) m_layout;
			}
			else
			{
				return ( const ReturnType & ) BaseType::template child< Index, DisableStaticAsserts, ReturnType >();
			}
		}

		T0 m_layout;
};

}; // namespace Detail

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct CompoundLayout : public Detail::CompoundLayoutRecurse<
	 CompoundLayout< T0, T1, T2, T3, T4, T5, T6, T7 >,
	 T0, T1, T2, T3, T4, T5, T6, T7
>
{
	// We create a typedef for each of the template parameters so that we can use the
	// TypeSwitch class to selectively choose one of the template arguments using an index.
	typedef T0 Type0;	typedef T1 Type1;	typedef T2 Type2;	typedef T3 Type3;
	typedef T4 Type4;	typedef T5 Type5;	typedef T6 Type6;	typedef T7 Type7;

	typedef Detail::CompoundLayoutRecurse<
		CompoundLayout< T0, T1, T2, T3, T4, T5, T6, T7 >,
		T0, T1, T2, T3, T4, T5, T6, T7
	> BaseType;
	
	typedef CompoundLayout< T0, T1, T2, T3, T4, T6, T7 > Type;

	enum
	{
		NumberOfChannelPointers = BaseType::NumberOfChannelPointers,
		NumberOfLayouts = BaseType::Iteration,
		NumberOfChannels = BaseType::NumberOfChannels,
		ChannelMask = BaseType::ChannelMask,
		ChannelPointerMask = BaseType::ChannelPointerMask,
	};

	public :
		
		typedef typename Gander::Image::Detail::CompoundLayoutContainer< Type, Gander::Image::Detail::ChannelContainer >  ChannelContainerType;
		typedef typename Gander::Image::Detail::CompoundLayoutContainer< Type, Gander::Image::Detail::ChannelPointerContainer >  ChannelPointerContainerType;

		/// Increments all channel pointers in the container by v.
		inline void increment( ChannelPointerContainerType &container, int v )
		{
			BaseType::template increment< ChannelPointerContainerType >( container, v );
		}
		
		/// Increments the given channel pointer in the container by v.
		inline void increment( ChannelPointerContainerType &container, Channel channel, int v )
		{
			BaseType::template increment< ChannelPointerContainerType >( container, channel, v );
		}
		
		template< unsigned Index, bool DisableStaticAsserts = true >
		inline typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType &child()
		{
			return BaseType::template child< Index, DisableStaticAsserts, typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType >();
		}
		
		template< unsigned Index, bool DisableStaticAsserts = true >
		inline const typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType &child() const
		{
			return BaseType::template child< Index, DisableStaticAsserts, typename BaseType::template LayoutTraits< Index, DisableStaticAsserts >::LayoutType >();
		}
		
		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ConstReferenceType
		>
		inline ConstReferenceType channel( const ChannelPointerContainerType &container ) const
		{
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType::ChannelPointerContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraits< C, DisableStaticAsserts >::LayoutIndex,
			};
			
			GANDER_ASSERT( ( std::is_same< ConstReferenceType, typename LayoutType::ConstReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ConstReferenceType & ) child< ChildIndex, true >().template channel< ContainerType, C >( container.template child< ChildIndex >() );
		}
		
		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ReferenceType
		>
		inline ReferenceType channel( ChannelPointerContainerType &container )
		{
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType::ChannelPointerContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraits< C, DisableStaticAsserts >::LayoutIndex,
			};
			
			GANDER_ASSERT( ( std::is_same< ReferenceType, typename LayoutType::ReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ReferenceType & ) child< ChildIndex, true >().template channel< ContainerType, C >( container.template child< ChildIndex >() );
		}
		
		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ReferenceType
		>
		inline ReferenceType channel( ChannelContainerType &container )
		{

			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType::ChannelContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraits< C, DisableStaticAsserts >::LayoutIndex,
			};
			
			GANDER_ASSERT( ( std::is_same< ReferenceType, typename LayoutType::ReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ReferenceType & ) child< ChildIndex, true >().template channel< ContainerType, C >( container.template child< ChildIndex >() );
		}
		
		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ConstReferenceType
		>
		inline ConstReferenceType channel( const ChannelContainerType &container ) const
		{

			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType::ChannelContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraits< C, DisableStaticAsserts >::LayoutIndex,
			};
			
			GANDER_ASSERT( ( std::is_same< ConstReferenceType, typename LayoutType::ConstReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ConstReferenceType & ) child< ChildIndex, true >().template channel< ContainerType, C >( container.template child< ChildIndex >() );
		}

		template<
			EnumType Index,
			EnumType Mask = Mask_All,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ConstReferenceType
		>
		inline ConstReferenceType channelAtIndex( const ChannelPointerContainerType &container ) const
		{
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType::ChannelPointerContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutIndex,
				ChannelIndexInLayout = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ChannelIndexInLayout,
			};
			
			GANDER_ASSERT( ( std::is_same< ConstReferenceType, typename LayoutType::ConstReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ConstReferenceType & ) child< ChildIndex, true >().template channelAtIndex< ContainerType, ChannelIndexInLayout, Mask >( container.template child< ChildIndex >() );
		}
		
		template<
			EnumType Index,
			EnumType Mask = Mask_All,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ReferenceType
		>
		inline ReferenceType channelAtIndex( ChannelPointerContainerType &container )
		{
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType::ChannelPointerContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutIndex,
				ChannelIndexInLayout = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ChannelIndexInLayout,
			};
			
			GANDER_ASSERT( ( std::is_same< ReferenceType, typename LayoutType::ReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ReferenceType & ) child< ChildIndex, true >().template channelAtIndex< ContainerType, ChannelIndexInLayout, Mask >( container.template child< ChildIndex >() );
		}
		
		template<
			EnumType Index,
			EnumType Mask = Mask_All,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ReferenceType
		>
		inline ReferenceType channelAtIndex( ChannelContainerType &container )
		{
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType::ChannelContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutIndex,
				ChannelIndexInLayout = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ChannelIndexInLayout,
			};
			
			GANDER_ASSERT( ( std::is_same< ReferenceType, typename LayoutType::ReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ReferenceType & ) child< ChildIndex, true >().template channelAtIndex< ContainerType, ChannelIndexInLayout, Mask >( container.template child< ChildIndex >() );
		}
		
		template<
			EnumType Index,
			EnumType Mask = Mask_All,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ConstReferenceType
		>
		inline ConstReferenceType channelAtIndex( const ChannelContainerType &container ) const
		{
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutType::ChannelContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::LayoutIndex,
				ChannelIndexInLayout = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ChannelIndexInLayout,
			};
			
			GANDER_ASSERT( ( std::is_same< ConstReferenceType, typename LayoutType::ConstReferenceType >::value ), "Incorrect return type specified." );
	
			return ( ConstReferenceType & ) child< ChildIndex, true >().template channelAtIndex< ContainerType, ChannelIndexInLayout, Mask >( container.template child< ChildIndex >() );
		}
	
		template<
			class ContainerType, 
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ReferenceType
		>
		inline ReferenceType channel( ContainerType &container )
		{
			return this->template channel< C, DisableStaticAsserts, ReferenceType >( container );
		}
		
		template<
			class ContainerType, 
			EnumType Index,
			EnumType Mask = Mask_All,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ReferenceType
		>
		inline ReferenceType channelAtIndex( ContainerType &container )
		{
			return this->template channelAtIndex< Index, Mask, DisableStaticAsserts, ReferenceType >( container );
		}
		
		template<
			class ContainerType, 
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ConstReferenceType
		>
		inline ConstReferenceType channel( const ContainerType &container ) const
		{
			return this->template channel< C, DisableStaticAsserts, ConstReferenceType >( container );
		}
		
		template<
			class ContainerType, 
			EnumType Index,
			EnumType Mask = Mask_All,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraitsAtIndex< Index, Mask, DisableStaticAsserts >::ConstReferenceType
		>
		inline ConstReferenceType channelAtIndex( const ContainerType &container ) const
		{
			return this->template channelAtIndex< Index, Mask, DisableStaticAsserts, ConstReferenceType >( container );
		}

		///\ todo See the comments below. 
		/*
		 * This hack could be used to remove the ammount of duplicated code by reuiring that only the const versions of the channel() methods need to be implemented.
		 * However, it is quite nasty and I am not sure about it yet....
		 *
		 *
		template<
			class ContainerType, 
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ReferenceType
		>
		inline ReferenceType channel( ContainerType &container )
		{
			return const_cast<ReferenceType>(
				static_cast< const Type * >( this )->template channel< C, DisableStaticAsserts, typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ConstReferenceType >( container )
			);
		}
		*/

		template< class ContainerType, class Type >
		inline void setChannel( ContainerType &container, Channel c, const Type &value )
		{
			switch( c )
			{
				case( 1 ) : channel< ContainerType, ChannelDefault( 1 ), true >( container ) = value; break;
				case( 2 ) : channel< ContainerType, ChannelDefault( 2 ), true >( container ) = value; break;
				case( 3 ) : channel< ContainerType, ChannelDefault( 3 ), true >( container ) = value; break;
				case( 4 ) : channel< ContainerType, ChannelDefault( 4 ), true >( container ) = value; break;
				case( 5 ) : channel< ContainerType, ChannelDefault( 5 ), true >( container ) = value; break;
				case( 6 ) : channel< ContainerType, ChannelDefault( 6 ), true >( container ) = value; break;
				case( 7 ) : channel< ContainerType, ChannelDefault( 7 ), true >( container ) = value; break;
				case( 8 ) : channel< ContainerType, ChannelDefault( 8 ), true >( container ) = value; break;
				case( 9 ) : channel< ContainerType, ChannelDefault( 9 ), true >( container ) = value; break;
				case( 10 ) : channel< ContainerType, ChannelDefault( 10 ), true >( container ) = value; break;
				default : GANDER_ASSERT( 0, "Channel does not exist in the CompoundLayout." ); break;
			}
		}
		
		template< class ContainerType, class Type >
		inline Type getChannel( const ContainerType &container, Channel c ) const
		{
			switch( c )
			{
				case( 1 ) : return channel< ContainerType, ChannelDefault( 1 ), true >( container ); break;
				case( 2 ) : return channel< ContainerType, ChannelDefault( 2 ), true >( container ); break;
				case( 3 ) : return channel< ContainerType, ChannelDefault( 3 ), true >( container ); break;
				case( 4 ) : return channel< ContainerType, ChannelDefault( 4 ), true >( container ); break;
				case( 5 ) : return channel< ContainerType, ChannelDefault( 5 ), true >( container ); break;
				case( 6 ) : return channel< ContainerType, ChannelDefault( 6 ), true >( container ); break;
				case( 7 ) : return channel< ContainerType, ChannelDefault( 7 ), true >( container ); break;
				case( 8 ) : return channel< ContainerType, ChannelDefault( 8 ), true >( container ); break;
				case( 9 ) : return channel< ContainerType, ChannelDefault( 9 ), true >( container ); break;
				case( 10 ) : return channel< ContainerType, ChannelDefault( 10 ), true >( container ); break;
				default : GANDER_ASSERT( 0, "Channel does not exist in the CompoundLayout." ); break;
			}
		}
		
		template< class ContainerType = ChannelPointerContainerType >
		inline void setChannelPointer( ChannelPointerContainerType &container, Channel channel, void *pointer )
		{
			GANDER_ASSERT(
				( container.size() == BaseType::numberOfChannelPointers() ),
				"Container has a different number of elements to the Layout's number of channels."
			);

			switch( channel )
			{
				case( 1 ) :
					setChannelPointer< ChannelDefault( 1 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 1 ), true >::PointerType& >( pointer ) );
					break;
				case( 2 ) :
					setChannelPointer< ChannelDefault( 2 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 2 ), true >::PointerType& >( pointer ) );
					break;
				case( 3 ) :
					setChannelPointer< ChannelDefault( 3 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 3 ), true >::PointerType& >( pointer ) );
					break;
				case( 4 ) :
					setChannelPointer< ChannelDefault( 4 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 4 ), true >::PointerType& >( pointer ) );
					break;
				case( 5 ) :
					setChannelPointer< ChannelDefault( 5 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 5 ), true >::PointerType& >( pointer ) );
					break;
				case( 6 ) :
					setChannelPointer< ChannelDefault( 6 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 6 ), true >::PointerType& >( pointer ) );
					break;
				case( 7 ) :
					setChannelPointer< ChannelDefault( 7 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 7 ), true >::PointerType& >( pointer ) );
					break;
				case( 8 ) :
					setChannelPointer< ChannelDefault( 8 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 8 ), true >::PointerType& >( pointer ) );
					break;
				case( 9 ) :
					setChannelPointer< ChannelDefault( 9 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 9 ), true >::PointerType& >( pointer ) );
					break;
				case( 10 ) :
					setChannelPointer< ChannelDefault( 10 ), true >( container, reinterpret_cast< typename BaseType::template ChannelTraits< ChannelDefault( 10 ), true >::PointerType& >( pointer ) );
					break;
				default : GANDER_ASSERT( 0, "Channel does not exist in the CompoundLayout." ); break;
			}
		}

	private :

		friend class LayoutBase< Type >;	

		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class PointerType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::PointerType
		>	
		inline void setChannelPointer( ChannelPointerContainerType &container, PointerType &pointer )
		{
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType LayoutType;	
			typedef typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType::ChannelPointerContainerType ContainerType;	
			
			enum
			{
				ChildIndex = CompoundLayout::template ChannelTraits< C, DisableStaticAsserts >::LayoutIndex,
			};
			
			GANDER_ASSERT( ( std::is_same< PointerType, typename LayoutType::PointerType >::value ), "Incorrect return type specified." );
	
			child< ChildIndex, DisableStaticAsserts >().template setChannelPointer< ContainerType >( container.template child< ChildIndex >(), C, pointer );
		}
	
};

}; // namespace Image

}; // namespace Gander

#endif
