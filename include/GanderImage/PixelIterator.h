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
#ifndef __GANDERIMAGE_PIXELITERATOR__
#define __GANDERIMAGE_PIXELITERATOR__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/EnumHelper.h"
#include "Gander/Interfaces.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

#include "GanderImage/ForEach.h"
#include "GanderImage/ChannelOps.h"
#include "GanderImage/Pixel.h"

namespace Gander
{

namespace Image
{

// Forward declarations.
template< class > class ConstPixelIterator;
template< class > class PixelIterator;

template< class Layout >
class ConstPixelIterator :
	protected PixelAccessor< Layout >,
	public IncrementOperators< ConstPixelIterator< Layout > >,
	public DecrementOperators< ConstPixelIterator< Layout > >,
	public IntegerArithmeticOperators< ConstPixelIterator< Layout > >
{
	private :
		
		typedef PixelAccessor< Layout > BaseType;
	
	public :	
		
		typedef typename Gander::Image::template ConstPixelAccessor< Layout > ConstPixelAccessorType;	
		typedef typename Gander::Image::template PixelAccessor< Layout > PixelAccessorType;	

		typedef typename BaseType::ContainerType ContainerType;
		typedef Layout LayoutType;
		typedef ConstPixelIterator< Layout > Type;
		
		inline ConstPixelIterator() {};
		
		ConstPixelIterator( const PixelIterator< Layout > &it );
		
		ConstPixelIterator( const PixelAccessorType &accessor );
		
		ConstPixelIterator( const ConstPixelAccessorType &accessor );
		
		inline ConstPixelAccessorType &operator * ()
		{
			return *static_cast< ConstPixelAccessorType * >( this );
		}

		inline ConstPixelAccessorType *operator -> ()
		{
			return static_cast< ConstPixelAccessorType * >( this );
		}
		
		inline const ConstPixelAccessorType &operator * () const
		{
			return *static_cast< const ConstPixelAccessorType * >( this );
		}

		inline const ConstPixelAccessorType *operator -> () const
		{
			return static_cast< const ConstPixelAccessorType * >( this );
		}
		
		inline Type &increment( int v, Channel channel )
		{
			BaseType::m_layout.increment( BaseType::m_container, channel, v );
			return *this;
		}

		inline Type &increment( int v )
		{
			BaseType::m_layout.increment( BaseType::m_container, v );
			return *this;
		}

		template< class T >		
		inline bool operator == ( const T &rhs ) const
		{
			GANDER_IMAGE_STATIC_ASSERT(
				( std::is_same< T, ConstPixelIterator< typename T::LayoutType > >::value || std::is_same< T, PixelIterator< typename T::LayoutType > >::value ),
				PIXELITERATORS_CAN_ONLY_BE_COMPARED_TO_OTHER_PIXELITERATORS__DEREFERENCE_IT_IN_ORDER_TO_COMPARE_BY_VALUE
			);

			return &BaseType::template channelAtIndex<0, typename BaseType::template ChannelTraitsAtIndex<0>::ConstReferenceType >() ==
				&rhs.template channelAtIndex<0, typename T::template ChannelTraitsAtIndex<0>::ConstReferenceType >();
		}
		
		template< class T >		
		inline bool operator != ( const T &rhs ) const
		{
			return !( this->template operator == < T >( rhs ) );
		}

	private :

		template< class, class, class > friend class PixelBase;
		template< class > friend class Gander::Image::Pixel;
		template< class > friend class Gander::Image::ConstPixelAccessor;
		template< class > friend class Gander::Image::PixelAccessor;
		template< class > friend class Gander::Image::ConstPixelIterator;
		template< class > friend class Gander::Image::PixelIterator;
};

template< class Layout >
class PixelIterator :
	public ConstPixelIterator< Layout >
{
	private :
		
		typedef ConstPixelIterator< Layout > BaseType;

	public :	
	
		typedef typename Gander::Image::template ConstPixelIterator< Layout > ConstPixelIteratorType;	
		typedef typename Gander::Image::template ConstPixelAccessor< Layout > ConstPixelAccessorType;	
		typedef typename Gander::Image::template PixelAccessor< Layout > PixelAccessorType;	
		
		typedef typename BaseType::ContainerType ContainerType;
		typedef Layout LayoutType;
		typedef PixelIterator< Layout > Type;

		inline PixelIterator() {};
		
		inline PixelIterator( const PixelIterator< Layout > &it ) : BaseType( it ) {};
		
		inline PixelIterator( const PixelAccessorType &accessor ) : BaseType( accessor ) {};
		
		inline PixelIterator( const ConstPixelAccessorType &accessor )
		{
			// Use a static assert to always give an error to stop assignments to this class from a ConstPixelAccessor.
			GANDER_IMAGE_STATIC_ASSERT( ( std::is_same< ConstPixelAccessorType, PixelIterator >::value ), YOU_CANNOT_COPY_BY_REFERENCE_FROM_A_CONST_CLASS_TO_A_NONCONST_CLASS );
		};
		
		inline PixelIterator( const ConstPixelIteratorType &it )
		{
			// Use a static assert to always give an error to stop assignments to this class from a ConstPixelIterator.
			GANDER_IMAGE_STATIC_ASSERT( ( std::is_same< ConstPixelAccessorType, PixelIterator >::value ), YOU_CANNOT_COPY_BY_REFERENCE_FROM_A_CONST_CLASS_TO_A_NONCONST_CLASS );
		};
		
		inline PixelAccessorType &operator * ()
		{
			return *static_cast< PixelAccessorType * >( this );
		}

		inline PixelAccessorType *operator -> ()
		{
			return static_cast< PixelAccessorType * >( this );
		}
		
		inline const PixelAccessorType &operator * () const
		{
			return *static_cast< const PixelAccessorType * >( this );
		}

		inline const PixelAccessorType *operator -> () const
		{
			return static_cast< const PixelAccessorType * >( this );
		}
	
	private :

		template< class, class, class > friend class PixelBase;
		template< class > friend class Gander::Image::Pixel;
		template< class > friend class Gander::Image::ConstPixelAccessor;
		template< class > friend class Gander::Image::PixelAccessor;
		template< class > friend class Gander::Image::ConstPixelIterator;
		template< class > friend class Gander::Image::PixelIterator;
};

}; // namespace Image

}; // namespace Gander

// The implementation of PixelIterator and ConstPixelIterator.
#include "GanderImage/PixelIterator.inl"

#endif
