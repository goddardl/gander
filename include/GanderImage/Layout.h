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
#include "GanderImage/CompoundLayout.h"
#include "GanderImage/ChannelBrothers.h"
#include "GanderImage/TestCompoundLayoutContainer.h"

namespace Gander
{

namespace ImageTest
{

struct ChannelLayoutTest;
 
}; // namespace ImageTest

namespace Image
{

/// Forward declaration of the TestCompoundLayout class.
template<
	class T0 = Detail::None, class T1 = Detail::None, class T2 = Detail::None, class T3 = Detail::None,
	class T4 = Detail::None, class T5 = Detail::None, class T6 = Detail::None, class T7 = Detail::None
> struct TestCompoundLayout;

template< class Derived >
struct TestLayoutBase
{
	public :
			
		enum
		{
			NumberOfLayouts = 0
		};

		typedef TestLayoutBase< Derived > Type;
		
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
		static inline bool contains( Channel channel ) { return ChannelSet( Gander::Image::ChannelMask( Derived::ChannelMask ) ).contains( channel ); }
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
};

namespace Detail
{

/// Forward declaration of the recursive base of the TestCompoundLayout class.
template< class Derived, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct TestCompoundLayoutRecurse;

template < class Derived >
struct TestCompoundLayoutRecurseBase : public TestLayoutBase< Derived >
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

/// The last recursive base of the TestCompoundLayout class.
template < class Derived >
struct TestCompoundLayoutRecurse< Derived, None, None, None, None, None, None, None, None > : public TestCompoundLayoutRecurseBase< Derived >
{
	private :

		typedef TestCompoundLayoutRecurseBase< Derived > BaseType;

	protected :
			
		enum
		{
			ChannelMask = 0,
			NumberOfChannels = 0,
		};

		static inline void increment( void *ptrContainer, int ){}
};

/// The body of the recursive TestCompoundLayoutRecurse class.
template< class Derived, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct TestCompoundLayoutRecurse : public TestCompoundLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None >
{
	protected :

		typedef TestCompoundLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None> BaseType;
		typedef typename Gander::Image::Detail::TestCompoundLayoutContainer< Derived, Gander::Image::Detail::TestChannelContainer >  ChannelContainerType;
		typedef typename Gander::Image::Detail::TestCompoundLayoutContainer< Derived, Gander::Image::Detail::TestChannelPointerContainer >  ChannelPointerContainerType;

		enum
		{
			Iteration = BaseType::Iteration + 1,
			ChannelMask = BaseType::ChannelMask | T0::ChannelMask,
			NumberOfChannels = BaseType::NumberOfChannels + T0::NumberOfChannels,
		};

		static inline void increment( ChannelPointerContainerType *ptrContainer, int i = 1 )
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
struct TestCompoundLayout : public Detail::TestCompoundLayoutRecurse< TestCompoundLayout< T0, T1, T2, T3, T4, T5, T6, T7 >, T0, T1, T2, T3, T4, T5, T6, T7 >
{
	private :

		typedef Detail::TestCompoundLayoutRecurse< TestCompoundLayout< T0, T1, T2, T3, T4, T5, T6, T7 >, T0, T1, T2, T3, T4, T5, T6, T7 > BaseType;
		typedef TestCompoundLayout<T0, T1, T2, T3, T4, T5, T6, T7> Type;
	
		// We create a typedef for each of the template parameters so that we can use the
		// TypeSwitch class to selectively choose one of the template arguments using an index.
		typedef T0 Type0;	typedef T1 Type1;	typedef T2 Type2;	typedef T3 Type3;
		typedef T4 Type4;	typedef T5 Type5;	typedef T6 Type6;	typedef T7 Type7;
		
		template< class T, EnumType TemplateIndex > friend class Detail::TypeSwitch;
		template< class Derived > friend class Detail::TestCompoundLayoutRecurseBase;
		template< class Derived, class S0, class S1, class S2, class S3, class S4, class S5, class S6, class S7 > friend class Detail::TestCompoundLayoutRecurse;
		template< class S0, class S1, class S2, class S3, class S4, class S5, class S6, class S7 > friend class TestCompoundLayout;
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

		//! @name Channel methods.
		//@{
		static inline unsigned int numberOfChannels() { return Gander::template EnumHelper<ChannelMask>::NumberOfSetBits; }
		static inline ChannelSet channels() { return ChannelSet( Gander::Image::ChannelMask( ChannelMask ) ); }
		static inline bool contains( Channel channel ) { return ChannelSet( Gander::Image::ChannelMask( ChannelMask ) ).contains( channel ); }
		//@}
		
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
				BaseType::template ChannelTraits< C >::LayoutType::template channel< C, DisableStaticAsserts >(
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

};

template< class T, ChannelDefault S >
struct TestChannelLayout : public TestLayoutBase< TestChannelLayout< T, S > >
{
	public :

		enum
		{
			ChannelMask = ChannelToMask<S>::Value,
			NumberOfChannels = 1,
		};

		typedef TestChannelLayout< T, S > Type;
		typedef TestLayoutBase< TestChannelLayout< T, S > > BaseType;
		typedef typename Gander::template TypeTraits< T >::Type ChannelType; 
		typedef typename Gander::template TypeTraits< T >::PointerType PointerType;
		typedef typename Gander::template TypeTraits< T >::ReferenceType ReferenceType;
		typedef typename Gander::template TypeTraits< T >::ConstReferenceType ConstReferenceType;
		typedef Gander::template Tuple< ChannelType, NumberOfChannels, false > ChannelContainerType;
		typedef Gander::template Tuple< PointerType, BaseType::NumberOfLayouts, false > ChannelPointerContainerType;
		
		template< ChannelDefault C = Chan_None, bool DisableStaticAsserts = false >
		struct ChannelTraits
		{
			typedef Type LayoutType;
			typedef typename Type::ChannelType ChannelType;
			typedef typename Type::PointerType PointerType;
			typedef typename Type::ReferenceType ReferenceType;
			typedef typename Type::ConstReferenceType ConstReferenceType;
			typedef typename Type::ChannelContainerType ChannelContainerType;
			typedef typename Type::ChannelPointerContainerType ChannelPointerContainerType;
		};

		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ReferenceType channel( ChannelContainerType *container )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return (*container)[0];
		}

		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ConstReferenceType channel( const ChannelContainerType * const container )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return (*container)[0];
		}
		
		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ReferenceType channel( ChannelPointerContainerType *ptrContainer )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return *(*ptrContainer)[0];
		}
		
		template< ChannelDefault C, bool DisableStaticAsserts = false >
		static inline ConstReferenceType channel( const ChannelPointerContainerType * const ptrContainer )
		{
			GANDER_IMAGE_STATIC_ASSERT( ( MaskContainsChannel< ChannelMask, C >::Value || DisableStaticAsserts ), CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
			return *(*ptrContainer)[0];
		}

		static inline void increment( ChannelPointerContainerType * ptrContainer, int i = 1 )
		{
			(*ptrContainer)[0] += i;
		}
				
};

}; // namespace Image

}; // namespace Gander

#endif
