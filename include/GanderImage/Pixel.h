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
#ifndef __GANDERIMAGE_PIXEL__
#define __GANDERIMAGE_PIXEL__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/EnumHelper.h"
#include "Gander/Interfaces.h"

#include "GanderImage/Common.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

#include "GanderImage/ForEach.h"
#include "GanderImage/ChannelOps.h"

namespace Gander
{

namespace Image
{

// Forward declaration of the Iterator class.
template< class > class ConstPixelIterator;

template< class Derived, class Layout, class Container >
class PixelBase
{
	private :

		typedef Container ContainerType;

	public : 
		
		typedef Layout LayoutType;
		typedef PixelBase< Derived, Layout, Container > Type;
		
		inline PixelBase() {}

		template< class T >
		inline const Derived &operator = ( const T &rhs )
		{
			return static_cast< Derived * >( this )->template copyByValueFrom< T >( rhs );
		}
		
		template< EnumType Index > struct LayoutTraits : public Layout::template LayoutTraits< Index > {};
		template< ChannelDefault C, bool DisableStaticAsserts = false > struct ChannelTraits : public Layout::template ChannelTraits< C, DisableStaticAsserts > {};
		static inline unsigned int numberOfChannels() { return Layout::numberOfChannels(); }
		static inline ChannelSet channels() { return Layout::channels(); }
		
		template< class T >
		inline bool operator == ( const T &rhs ) const
		{
			return this->template equalTo< T >( rhs );
		}
		
		template< class T >
		inline bool operator != ( const T &rhs ) const
		{
			return !this->template equalTo< T >( rhs );
		}

		/// The copyByValueFrom method is the implementation of the assignment operator.		
		template< class T >
		inline const Derived & copyByValueFrom( const T &rhs, bool copyAvailableChannels = false )
		{
			GANDER_ASSERT( static_cast< Derived * >( this )->channels() == rhs.channels() || copyAvailableChannels,
				"Cannot copy one pixel to another if they have different channels."
			);
			forEachChannel( *this, rhs, Copy() );
			return *static_cast< const Derived * >( this );
		}
		
		template< class Type = DefaultChannelType >
		inline void setChannel( Channel channel, const Type &value )
		{
			return LayoutType::template setChannel< Type >( &m_container, channel, value );
		}

		template< class Type = DefaultChannelType >
		inline Type getChannel( Channel channel ) const
		{
			return LayoutType::template getChannel< Type >( &m_container, channel );
		}

		template< ChannelDefault C, bool DisableStaticAsserts = false, class ReturnType = typename Type::template ChannelTraits< C, DisableStaticAsserts >::ReferenceType >
		inline ReturnType channel()
		{
			return LayoutType::template channel< C, DisableStaticAsserts >( &m_container );
		}

		template< ChannelDefault C, bool DisableStaticAsserts = false, class ReturnType = typename Type::template ChannelTraits< C, DisableStaticAsserts >::ConstReferenceType >
		inline ReturnType channel() const
		{
			return LayoutType::template channel< C, DisableStaticAsserts >( &m_container );
		}
		
		template< class Type = DefaultChannelType >
		inline Type operator [] ( Channel channel ) const
		{
			return getChannel< Type >( channel );
		}

		template< class > friend class ConstPixelIterator;

	protected :

		/// The equalTo method is the implementation of the equality interface.	
		template< class T >
		bool equalTo( T const &rhs ) const
		{
			if( std::is_same< LayoutType, typename T::LayoutType >::value )
			{
				return false;
			}

			IsEqual op;
			forEachChannel( *this, rhs, op );
			return op.value();
		}

		ContainerType m_container;

		template< class, class, class > friend class PixelBase;
		template< class > friend class Pixel;
		template< class > friend class PixelAccessor;
		template< class > friend class ConstPixelAccessor;
};

template< class Layout >
struct Pixel : public PixelBase< Pixel< Layout >, Layout, typename Layout::ChannelContainerType >
{
	private :

		typedef PixelBase< Pixel< Layout >, Layout, typename Layout::ChannelContainerType > BaseType;

	public :	

		typedef typename Layout::ChannelContainerType ContainerType;
		typedef Layout LayoutType;
		typedef Pixel< Layout > Type;

		template< class T >
		inline const Pixel &operator = ( const T &rhs )
		{
			return BaseType::template copyByValueFrom< T >( rhs );
		}
	
	private :
		
		template< class, class, class > friend class PixelBase;
		template< class > friend class Pixel;
		template< class > friend class PixelAccessor;
		template< class > friend class ConstPixelAccessor;
};

template< class Layout >
struct ConstPixelAccessor : public PixelBase< ConstPixelAccessor< Layout >, Layout, typename Layout::ChannelPointerContainerType >
{
	private :
		
		typedef PixelBase< ConstPixelAccessor< Layout >, Layout, typename Layout::ChannelPointerContainerType > BaseType;

	public :	
		
		typedef typename Layout::ChannelPointerContainerType ContainerType;
		typedef Layout LayoutType;
		typedef ConstPixelAccessor< Layout > Type;

		inline void setChannelPointer( Channel channel, void *pointer )
		{
//			BaseType::LayoutType::template setChannelPointer< ContainerType >( BaseType::m_container, channel, pointer );
		}
		
		template< class T >		
		inline const Type & operator = ( const T &rhs ) const
		{
			// Use a static assert to always give an error to stop assignments to this class.
			GANDER_IMAGE_STATIC_ASSERT( ( !std::is_same< T, T >::value ), YOU_CANNOT_COPY_BY_VALUE_TO_A_CONST_CLASS );

			return *this;
		}
		
	private :

		template< class, class, class > friend class PixelBase;
		template< class > friend class Pixel;
		template< class > friend class PixelAccessor;
		template< class > friend class ConstPixelAccessor;
};

template< class Layout >
struct PixelAccessor : public ConstPixelAccessor< Layout >
{
	private :
		
		typedef ConstPixelAccessor< Layout > BaseType;

	public :	
		
		typedef typename Layout::ChannelPointerContainerType ContainerType;
		typedef Layout LayoutType;
		typedef PixelAccessor< Layout > Type;

		template< class T >
		inline const PixelAccessor & operator = ( const T &rhs )
		{
			BaseType::template copyByValueFrom< T >( rhs );
			return *this;
		}
		
	private :

		template< class, class, class > friend class PixelBase;
		template< class > friend class Pixel;
		template< class > friend class PixelAccessor;
		template< class > friend class ConstPixelAccessor;
};

}; // namespace Image

}; // namespace Gander

#endif
