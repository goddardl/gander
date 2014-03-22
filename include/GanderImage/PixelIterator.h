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
		
		typedef const typename Gander::Image::template PixelAccessor< Layout > ConstPixelAccessor;	
		typedef typename BaseType::ContainerType ContainerType;
		typedef Layout LayoutType;
		typedef ConstPixelIterator< Layout > Type;
		
		inline ConstPixelIterator() {};
		
		template< class RhsLayout >	
		ConstPixelIterator( const ConstPixelIterator< RhsLayout > &it );
		
		template< class RhsLayout >	
		ConstPixelIterator( const PixelIterator< RhsLayout > &it );
		
		inline const PixelAccessor< Layout > &operator * () const
		{
			return *static_cast< const PixelAccessor< Layout > * >( this );
		}

		inline const PixelAccessor< Layout > *operator -> () const
		{
			return static_cast< const PixelAccessor< Layout > * >( this );
		}
		
		template< class T >
		inline const ConstPixelIterator & operator = ( const T &rhs )
		{
			return BaseType::template copyFrom< T >( rhs );
		}
		
		inline Type &increment( int v )
		{
			BaseType::m_layout.increment( BaseType::m_container, v );
			return *this;
		}

		inline Type &decrement( int v )
		{
			BaseType::m_layout.decrement( BaseType::m_container, v );
			return *this;
		}

		inline bool operator == ( const Type &rhs ) const
		{
			return &BaseType::template channelAtIndex<0>() == &rhs.template channelAtIndex<0>();
		}

		inline bool operator != ( const Type &rhs ) const
		{
			return &BaseType::template channelAtIndex<0>() != &rhs.template channelAtIndex<0>();
		}

	private :

		template< class, class, class > friend class PixelBase;
		template< class > friend class Pixel;
		template< class > friend class PixelAccessor;
		template< class > friend class ConstPixelIterator;
		template< class > friend class PixelIterator;
};

template< class Layout >
class PixelIterator : public ConstPixelIterator< Layout >
{
	private :
		
		typedef ConstPixelIterator< Layout > BaseType;

	public :	
	
		typedef typename Gander::Image::template PixelAccessor< Layout > PixelAccessor;	
		typedef typename BaseType::ConstPixelAccessor ConstPixelAccessor;	
		typedef typename BaseType::ContainerType ContainerType;
		typedef Layout LayoutType;
		typedef PixelIterator< Layout > Type;

		inline PixelIterator() {};
	
		template< class RhsLayout >	
		PixelIterator( const ConstPixelIterator< RhsLayout > &it ) : BaseType( it ) {};
		
		template< class RhsLayout >	
		PixelIterator( const typename Gander::Image::template PixelIterator< RhsLayout > &it ) :
			BaseType( static_cast< const typename Gander::Image::template ConstPixelIterator< RhsLayout > & >( it ) )
		{}
		
		inline PixelAccessor &operator * ()
		{
			return *static_cast< PixelAccessor * >( this );
		}

		inline PixelAccessor *operator -> ()
		{
			return static_cast< PixelAccessor * >( this );
		}
		
	private :

		template< class, class, class > friend class PixelBase;
		template< class > friend class Pixel;
		template< class > friend class Gander::Image::PixelAccessor;
		template< class > friend class ConstPixelIterator;
		template< class > friend class PixelIterator;
};

}; // namespace Image

}; // namespace Gander

// The implementation of PixelIterator and ConstPixelIterator.
#include "GanderImage/PixelIterator.inl"

#endif
