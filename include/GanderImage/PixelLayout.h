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
#ifndef __GANDERIMAGE_PIXELLAYOUT__
#define __GANDERIMAGE_PIXELLAYOUT__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Layout.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

/// Forward declaration of the test class. 
namespace ImageTest { class PixelLayoutTest; };

namespace Image
{

/// An empty struct for use as a default template argument to the PixelLayout class.
namespace Detail { struct None { struct LayoutTraits { }; }; };

/// Forward declaration of the PixelLayout class.
template<
	class T0 = Detail::None, class T1 = Detail::None, class T2 = Detail::None, class T3 = Detail::None,
	class T4 = Detail::None, class T5 = Detail::None, class T6 = Detail::None, class T7 = Detail::None
> struct PixelLayout;

namespace Detail
{

/// The ChannelIndexToLayoutIndex class is a helper class that returns the index of the Layout within the template arguments that contains the channel of the given index.
template< EnumType Index, class T0 = None, class T1 = None, class T2 = None, class T3 = None, class T4 = None, class T5 = None, class T6 = None, class T7 = None >
struct ChannelIndexToLayoutIndex;

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
		Count = BaseType::Count + 1,
		Value = ( ( ChannelToMask<C>::Value & T0::ChannelMask ) != 0 ) ? NumberOfLayouts - Count - 1 : BaseType::Value
	};
};

/// The last recursive base of the ChannelIndexToLayoutIndex helper class. 
template< EnumType Index >
struct ChannelIndexToLayoutIndex< Index, None, None, None, None, None, None, None, None > { enum { NumberOfChannels = 0, Count = -1, Value = 0 }; };

/// The body of the ChannelIndexToLayoutIndex class.
template< EnumType Index, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct ChannelIndexToLayoutIndex : public ChannelIndexToLayoutIndex< Index, T1, T2, T3, T4, T5, T6, T7, None >
{
	typedef ChannelIndexToLayoutIndex< Index, T1, T2, T3, T4, T5, T6, T7, None> BaseType;
	enum
	{
		NumberOfChannels = BaseType::NumberOfChannels + T0::NumberOfChannels,
		Count = BaseType::Count + 1,
		Value = ( Index <= NumberOfChannels && Index > BaseType::NumberOfChannels ) ? Count : BaseType::Value,
	};
};

/// Provides a mechanism for switching between type declarations.
/// Given a class 'T' that defines declarations of Type0-7, TypeSwitch can be used to selectively declare
/// one of the Types as 'Type'.
/// For example :
/// TypeSwitch< T, 3 >::Type // Choose Type3 of class T.
template< class T, EnumType Index > struct TypeSwitch {};
template< class T > struct TypeSwitch<T, 0> { typedef typename T::Type0 Type; };
template< class T > struct TypeSwitch<T, 1> { typedef typename T::Type1 Type; };
template< class T > struct TypeSwitch<T, 2> { typedef typename T::Type2 Type; };
template< class T > struct TypeSwitch<T, 3> { typedef typename T::Type3 Type; };
template< class T > struct TypeSwitch<T, 4> { typedef typename T::Type4 Type; };
template< class T > struct TypeSwitch<T, 5> { typedef typename T::Type5 Type; };
template< class T > struct TypeSwitch<T, 6> { typedef typename T::Type6 Type; };
template< class T > struct TypeSwitch<T, 7> { typedef typename T::Type7 Type; };

/// Forward declaration of the recursive base of the PixelLayout class.
template< class Derived, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct PixelLayoutRecurse;

/// The last recursive base of the PixelLayout class.
template < class Derived >
struct PixelLayoutRecurse< Derived, None, None, None, None, None, None, None, None > : public Layout< Derived >
{
	protected :

		enum
		{
			NumberOfLayouts = 0,
			NumberOfChannels = 0,
			ChannelMask = 0,
		};
	
		template< EnumType LayoutIndex >
		struct LayoutTraits
		{
			typedef typename Detail::TypeSwitch< Derived, LayoutIndex >::Type LayoutType;
			typedef typename LayoutType::StorageType StorageType;
		};
	
		template< EnumType ChannelIndex >
		struct ChannelIndexToLayoutIndex
		{
			enum
			{
				Value = Detail::ChannelIndexToLayoutIndex
				<
					ChannelIndex,
					typename Derived::Type0, typename Derived::Type1, typename Derived::Type2, typename Derived::Type3,
					typename Derived::Type4, typename Derived::Type5, typename Derived::Type6, typename Derived::Type7
				>::Value,
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
		template< ChannelDefault C >
		struct ChannelTraits
		{
			typedef typename LayoutTraits< ChannelToLayoutIndex<C>::Value >::StorageType StorageType;
			typedef typename LayoutTraits< ChannelToLayoutIndex<C>::Value >::LayoutType LayoutType;

			enum
			{
				ChannelBrothers = LayoutType::ChannelBrothers,
			};
		};
};

/// The body of the recursive PixelLayoutRecurse class.
template< class Derived, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct PixelLayoutRecurse : public PixelLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None >
{
	protected :
		typedef PixelLayoutRecurse< Derived, T1, T2, T3, T4, T5, T6, T7, None> BaseType;

		enum
		{
			NumberOfLayouts = BaseType::NumberOfLayouts + 1,
			NumberOfChannels = BaseType::NumberOfChannels + T0::NumberOfChannels,
			ChannelMask = BaseType::ChannelMask + T0::ChannelMask,
		};

		// Assert that the template arguments have been supplied with the Pixels representing channels ordered
		// from left to right in order of the channels of the lowest value first.
		GANDER_IMAGE_STATIC_ASSERT(
			( EnumType( T0::ChannelMask ) < BaseType::ChannelMask ) | ( BaseType::ChannelMask == 0 ),
			TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_BE_SUPPLIED_IN_ORDER_OF_CHANNEL_VALUE
		);

		// Assert that the template arguments each relate to unique channels.
		GANDER_IMAGE_STATIC_ASSERT( ( ( EnumType( T0::ChannelMask ) & BaseType::ChannelMask ) == 0 ), TEMPLATE_ARGUMENTS_TO_IMAGELAYOUT_MUST_ALL_REPRESENT_UNIQUE_CHANNELS );
};

}; // namespace Detail

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
struct PixelLayout : public Detail::PixelLayoutRecurse< PixelLayout< T0, T1, T2, T3, T4, T5, T6, T7 >, T0, T1, T2, T3, T4, T5, T6, T7 >
{
	// We create a typedef for each of the template parameters so that we can use the
	// TypeSwitch class to selectively choose one of the template arguments using an index.
	typedef T0 Type0;	typedef T1 Type1;	typedef T2 Type2;	typedef T3 Type3;
	typedef T4 Type4;	typedef T5 Type5;	typedef T6 Type6;	typedef T7 Type7;
	typedef Detail::PixelLayoutRecurse< PixelLayout< T0, T1, T2, T3, T4, T5, T6, T7 >, T0, T1, T2, T3, T4, T5, T6, T7 > BaseType;
		
	enum
	{
		NumberOfLayouts = BaseType::NumberOfLayouts,
		NumberOfChannels = BaseType::NumberOfChannels,
		ChannelMask = BaseType::ChannelMask,
	};
};

}; // namespace Image

}; // namespace Gander

#endif
