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

#include "Gander/Common.h"
#include "Gander/EnumHelper.h"
#include "Gander/Interfaces.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

struct AreEqual
{
	typedef bool ReturnType;
	template< class T, class S >
	ReturnType operator () ( T t, S s )
	{
		std::cerr << s << " == " << t << " = " << ( t == s ) << std::endl;
		return ( t == s );
	}
};

template< typename Op >
struct NoAggregation
{
	typedef typename Op::ReturnType ReturnType;
	inline void append( typename Op::ReturnType ){};
	inline ReturnType value() const { return ReturnType(); };
};

template< typename Op >
struct AndAccumulate
{
	typedef typename Op::ReturnType ReturnType;
	
	inline AndAccumulate() : m_value( ~ReturnType(0) ) {}

	inline void append( typename Op::ReturnType value ){ m_value &= value; }

	inline ReturnType value() const { return m_value; };

	ReturnType m_value;
};

template< typename Pixel1, class Pixel2, class Op, class Aggregator, Gander::EnumType FullMask, Gander::EnumType StaticMask, Gander::EnumType PartialMask > struct ForEachRecurse;

template< typename Pixel1, class Pixel2, class Op, class Aggregator, Gander::EnumType FullMask, Gander::EnumType StaticMask > struct ForEachRecurse< Pixel1, Pixel2, Op, Aggregator, FullMask, StaticMask, 0 >
{
	inline void operator () ( const Pixel1 &p1, const Pixel2 &p2, Op &op, Aggregator &a )
	{
		if( p1.isDynamic() || p2.isDynamic() )
		{
			Gander::Image::ChannelSet dynamicChannels( Gander::Image::ChannelMask( FullMask & ( ~StaticMask ) ) ); 
			dynamicChannels &= p1.channels();
			dynamicChannels &= p2.channels();
		
			write a loop here which loops over the dynamic channels and applies the op to them. 
			Then, write test cases that prove it works...	
		}
	};
};
	
template< typename Pixel1, class Pixel2, class Op, class Aggregator, Gander::EnumType FullMask, Gander::EnumType StaticMask, Gander::EnumType PartialMask >
struct ForEachRecurse
{
	enum
	{
		Index = Gander::template EnumHelper< PartialMask >::FirstSetBit,
		NextPartialMask = PartialMask & ( ( ~( Gander::EnumType(0) ) ) << ( Index + 1 ) ),
		Channel = Index + 1,
	};

	inline void operator () ( Pixel1 &p1, Pixel2 &p2, Op &op, Aggregator &a )
	{
		std::cerr << "Channel " << int( Channel ) << std::endl;
		a.append( op( p1.template channel< Gander::Image::ChannelDefault( Channel ) >(), p2.template channel< Gander::Image::ChannelDefault( Channel ) >()  ) );
		ForEachRecurse< Pixel1, Pixel2, Op, Aggregator, FullMask, StaticMask, NextPartialMask >()( p1, p2, op, a );
	}
};

template< class Pixel1, class Pixel2, class Op, class Aggregator = NoAggregation< Op > >
typename Aggregator::ReturnType forEachChannel( Pixel1 &p1, Pixel2 &p2, Op op, Aggregator a = Aggregator() )
{
	enum
	{
		FullMask = Gander::Image::Mask_All,
		StaticMask = ( Pixel1::LayoutType::ChannelMask & Pixel2::LayoutType::ChannelMask & FullMask ),
	};
	
	ForEachRecurse< Pixel1, Pixel2, Op, Aggregator, FullMask, StaticMask, StaticMask >()( p1, p2, op, a );
	
	return a.value();
}

namespace Gander
{

namespace Image
{

template< class Pixel1, class Pixel2 >
class PixelOp
{
};

template< class Derived, class Layout, class Container >
class PixelBase : public EqualComparisonOperators< Derived >
{
	private :

		typedef Container ContainerType;

	public : 
		
		typedef Layout LayoutType;
		typedef PixelBase< Derived, Layout, Container > Type;
		
		PixelBase() :
			m_container( m_layout )
		{
		}

		template< EnumType Index > struct LayoutTraits : public Layout::template LayoutTraits< Index > {};
		template< ChannelDefault C > struct ChannelTraits : public Layout::template ChannelTraits< C > {};
		
		inline unsigned int numberOfChannels() const { return m_layout.numberOfChannels(); }
		inline ChannelSet channels() const { return m_layout.channels(); }
		inline bool isDynamic() const { return m_layout.isDynamic(); }
		inline void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None ) { m_layout.template addChannels< ContainerType >( c, b ); };
	
		template< class T >
		bool equalTo( T const &rhs ) const
		{
			if( m_layout != rhs.m_layout )
			{
				return false;
			}

			return true;
		}

		template< ChannelDefault C, class ReturnType = typename Type::template ChannelTraits< C >::ReferenceType >
		inline ReturnType channel()
		{
			return m_layout.template channel< typename Derived::ContainerType, C >( m_container );
		}

	protected :
		
		LayoutType m_layout;
		ContainerType m_container;
		
};

template< class Layout >
struct Pixel : public PixelBase< Pixel< Layout >, Layout, typename Layout::ChannelContainerType >
{
	public :	

		typedef typename Layout::ChannelContainerType ContainerType;
		typedef Layout LayoutType;
		typedef Pixel< Layout > Type;

};

template< class Layout >
struct PixelAccessor : public PixelBase< PixelAccessor< Layout >, Layout, typename Layout::ChannelPointerContainerType >
{
	private :
		
		typedef PixelBase< PixelAccessor< Layout >, Layout, typename Layout::ChannelPointerContainerType > BaseType;

	public :	
		
		typedef typename Layout::ChannelPointerContainerType ContainerType;
		typedef Layout LayoutType;
		typedef PixelAccessor< Layout > Type;

		inline unsigned int numberOfChannelPointers() const { return BaseType::m_layout.numberOfChannelPointers(); };
		inline ChannelSet requiredChannels() const { return BaseType::m_layout.requiredChannels(); };

};

}; // namespace Image

}; // namespace Gander

#endif
