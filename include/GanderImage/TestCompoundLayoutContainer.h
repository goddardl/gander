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
#ifndef __GANDERIMAGE_TESTCOMPOUNDLAYOUTCONTAINER__
#define __GANDERIMAGE_TESTCOMPOUNDLAYOUTCONTAINER__

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

typedef enum
{
	TestChannelContainer,
	TestChannelPointerContainer,
} TestContainerName;

template< class CompoundLayout, EnumType N, TestContainerName Container >
struct TestCompoundLayoutContainerRecurse;

template< class CompoundLayout, TestContainerName Container >
struct TestCompoundLayoutContainer;

template< class CompoundLayout, TestContainerName Container >
struct TestCompoundLayoutContainerRecurse< CompoundLayout, 0, Container >
{
	protected :

		inline unsigned int size() const
		{
			return 0;
		}
		
		template< class ReturnType, EnumType Index >
		inline ReturnType &child()
		{
			GANDER_ASSERT( Index >= 0 || Index < CompoundLayout::NumberOfLayouts, "Index is out of bounds." );

			// We never get here but we still need to return something to appease the compiler.
			static ReturnType r;
			return r; // Return it.
		};
		
		template< class ReturnType, EnumType Index >
		inline const ReturnType &child() const
		{
			GANDER_ASSERT( Index >= 0 || Index < CompoundLayout::NumberOfLayouts, "Index is out of bounds." );

			// We never get here but we still need to return something to appease the compiler.
			static ReturnType r;
			return r; // Return it.
		};
		
};

template< class CompoundLayout, EnumType N, TestContainerName Container >
struct TestCompoundLayoutContainerRecurse : public TestCompoundLayoutContainerRecurse< CompoundLayout, N - 1, Container >
{
	public :

		enum { LayoutIndex = CompoundLayout::NumberOfLayouts - N };
		
		typedef TestCompoundLayoutContainerRecurse< CompoundLayout, N, Container > Type;
		typedef TestCompoundLayoutContainerRecurse< CompoundLayout, N - 1, Container > BaseType;
		typedef typename CompoundLayout::template LayoutTraits< LayoutIndex >::LayoutType LayoutType;	
		typedef typename LayoutType::ChannelType ChannelType;
		typedef typename LayoutType::PointerType PointerType;
		typedef typename LayoutType::ReferenceType ReferenceType;
		typedef typename LayoutType::ConstReferenceType ConstReferenceType;
	
		typedef typename LayoutType::ChannelContainerType ChannelContainerType;
		typedef typename LayoutType::ChannelPointerContainerType ChannelPointerContainerType;
		typedef typename std::conditional< Container == TestChannelContainer, ChannelContainerType, ChannelPointerContainerType >::type ContainerType;

	protected :
	
		inline unsigned int size() const
		{
			return m_container.size() + BaseType::size();
		}

		template< class ReturnType, EnumType Index >
		inline ReturnType &child()
		{
			if( Index == LayoutIndex )
			{
				return ( ReturnType & ) m_container;
			}
			else
			{
				return BaseType::template child< ReturnType, Index >();
			}
		};

		template< class ReturnType, EnumType Index >
		inline const ReturnType &child() const
		{
			if( Index == LayoutIndex )
			{
				return ( const ReturnType & ) m_container;
			}
			else
			{
				return BaseType::template child< ReturnType, Index >();
			}
		};

	private :

		ContainerType m_container;
		
};

template< class CompoundLayout, TestContainerName Container >
class TestCompoundLayoutContainer : public Gander::Image::Detail::TestCompoundLayoutContainerRecurse< CompoundLayout, CompoundLayout::NumberOfLayouts, Container >
{
	private :

		typedef typename Gander::Image::Detail::TestCompoundLayoutContainerRecurse< CompoundLayout, CompoundLayout::NumberOfLayouts, Container > BaseType;

	public :
	
		typedef CompoundLayout LayoutType;
			
		template< EnumType Index >
		struct ChildTraitsAtIndex
		{
			typedef typename std::conditional< Container == TestChannelContainer, 
				typename CompoundLayout::template LayoutTraits< Index, true >::LayoutType::ChannelContainerType,
				typename CompoundLayout::template LayoutTraits< Index, true >::LayoutType::ChannelPointerContainerType
			>::type ContainerType;
		};
	
		template< EnumType Index >
		inline typename ChildTraitsAtIndex< Index >::ContainerType &child()
		{
			return BaseType::template child< typename ChildTraitsAtIndex< Index >::ContainerType, Index >();
		};

		template< EnumType Index >
		inline const typename ChildTraitsAtIndex< Index >::ContainerType &child() const
		{
			return BaseType::template child< typename ChildTraitsAtIndex< Index >::ContainerType, Index >();
		};

		inline unsigned int size() const
		{
			return BaseType::size();
		}
	
};

}; // namespace Detail

}; // namespace Image

}; // namespace Gander

#endif
