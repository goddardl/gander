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
#ifndef __GANDERIMAGE_COMPOUNDLAYOUT__
#define __GANDERIMAGE_COMPOUNDLAYOUT__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Assert.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Layout.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"
#include "GanderImage/CompoundLayoutContainer.h"

namespace Gander
{

namespace Image
{

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
struct CompoundLayoutRecurseBase : public Layout< Derived >
{
	public :

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
			ChannelMask = 0,
			NumberOfChannels = 0,
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
};

/// The last recursive base of the CompoundLayout class.
template < class Derived >
struct CompoundLayoutRecurse< Derived, None, None, None, None, None, None, None, None > : public CompoundLayoutRecurseBase< Derived >
{
	private :

		typedef CompoundLayoutRecurseBase< Derived > BaseType;

	protected :
			
		enum
		{
			ChannelMask = 0,
			NumberOfChannels = 0,
		};

		static inline void increment( void *ptrContainer, int ){}
};

/// The body of the recursive CompoundLayoutRecurse class.
template< class Derived, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct CompoundLayoutRecurse : public CompoundLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None >
{
	protected :

		typedef CompoundLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None> BaseType;
		typedef typename Gander::Image::Detail::CompoundLayoutContainer< Derived, Gander::Image::Detail::ChannelContainer > ChannelContainerType;
		typedef typename Gander::Image::Detail::CompoundLayoutContainer< Derived, Gander::Image::Detail::ChannelPointerContainer > ChannelPointerContainerType;

		enum
		{
			Iteration = BaseType::Iteration + 1,
			ChannelMask = BaseType::ChannelMask | T0::ChannelMask,
			NumberOfChannels = BaseType::NumberOfChannels + T0::NumberOfChannels,
		};

		static inline void increment( ChannelPointerContainerType * ptrContainer, int i = 1 )
		{
			BaseType::template LayoutTraits< Iteration - 1 >::LayoutType::increment(
				&( ptrContainer->template child< Iteration - 1 >() ), i
			);
		
			BaseType::increment( ptrContainer, i );
		}
		
		// Assert that the template arguments have been supplied with the Pixels representing channels ordered
		// from left to right in order of the channels of the lowest value first.
		GANDER_IMAGE_STATIC_ASSERT(
			( ( EnumType( T0::ChannelMask ) < EnumType( T1::ChannelMask ) ) || ( std::is_same< T1, Detail::None >::value == true ) ),
			TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_BE_SUPPLIED_IN_ORDER_OF_CHANNEL_VALUE
		);

		// Assert that the template arguments each relate to unique channels.
		GANDER_IMAGE_STATIC_ASSERT( ( ( EnumType( T0::ChannelMask ) & BaseType::ChannelMask ) == 0 ), TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_ALL_REPRESENT_UNIQUE_CHANNELS );
};

}; // namespace Detail

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct CompoundLayout : public Detail::CompoundLayoutRecurse< CompoundLayout< T0, T1, T2, T3, T4, T5, T6, T7 >, T0, T1, T2, T3, T4, T5, T6, T7 >
{
	private :

		typedef Detail::CompoundLayoutRecurse< CompoundLayout< T0, T1, T2, T3, T4, T5, T6, T7 >, T0, T1, T2, T3, T4, T5, T6, T7 > BaseType;
		typedef CompoundLayout<T0, T1, T2, T3, T4, T5, T6, T7> Type;
	
		// We create a typedef for each of the template parameters so that we can use the
		// TypeSwitch class to selectively choose one of the template arguments using an index.
		typedef T0 Type0;	typedef T1 Type1;	typedef T2 Type2;	typedef T3 Type3;
		typedef T4 Type4;	typedef T5 Type5;	typedef T6 Type6;	typedef T7 Type7;
		
		template< class T, EnumType TemplateIndex > friend class Detail::TypeSwitch;
		template< class Derived > friend class Detail::CompoundLayoutRecurseBase;
		template< class Derived, class S0, class S1, class S2, class S3, class S4, class S5, class S6, class S7 > friend class Detail::CompoundLayoutRecurse;
		template< class S0, class S1, class S2, class S3, class S4, class S5, class S6, class S7 > friend class CompoundLayout;
		template< ChannelDefault > friend class BaseType::ChannelToLayoutIndex;
		template< EnumType, EnumType > friend class BaseType::ChannelIndexHelper;
		template< EnumType, bool > friend class LayoutTraits;

	public :
		
		enum
		{
			NumberOfLayouts = BaseType::Iteration,
			ChannelMask = BaseType::ChannelMask,
			NumberOfChannels = Gander::EnumHelper<ChannelMask>::NumberOfSetBits,
		};
		
		typedef typename BaseType::ChannelContainerType ChannelContainerType;
		typedef typename BaseType::ChannelPointerContainerType ChannelPointerContainerType;

		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ReferenceType
		>
		static inline ReferenceType channel( ChannelContainerType *container )
		{
			return (
				BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType::template channel< C, DisableStaticAsserts >(
					&( container->template child< BaseType::template ChannelToLayoutIndex<C>::Value >() )
				)
			);
		}

		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ConstReferenceType
		>
		static inline ConstReferenceType channel( const ChannelContainerType * const container )
		{
			return channel< C, DisableStaticAsserts >( const_cast<ChannelContainerType*>( container ) );
		}

		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ReferenceType
		>
		static inline ReferenceType channel( ChannelPointerContainerType *ptrContainer )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			
			return (
				BaseType::template ChannelTraits< C, DisableStaticAsserts >::LayoutType::template channel< C, DisableStaticAsserts >(
					&( ptrContainer->template child< BaseType::template ChannelToLayoutIndex<C>::Value >() )
				)
			);
		}
		
		template<
			ChannelDefault C,
			bool DisableStaticAsserts = false,
			class ConstReferenceType = typename BaseType::template ChannelTraits< C, DisableStaticAsserts >::ConstReferenceType
		>
		static inline ConstReferenceType channel( const ChannelPointerContainerType * const ptrContainer )
		{
			return channel< C, DisableStaticAsserts >( const_cast<ChannelPointerContainerType*>( ptrContainer ) );
		}
		
		static inline void increment( ChannelPointerContainerType * ptrContainer, int i = 1 )
		{
			BaseType::increment( ptrContainer, i );
		}
		
		static inline void increment( ChannelPointerContainerType & ptrContainer, int i = 1 )
		{
			BaseType::increment( &ptrContainer, i );
		}

};

}; // namespace Image

}; // namespace Gander

#endif
