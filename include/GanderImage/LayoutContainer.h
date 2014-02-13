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
#ifndef __GANDERIMAGE_LAYOUTCONTAINER__
#define __GANDERIMAGE_LAYOUTCONTAINER__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Assert.h"
#include "Gander/Common.h"
#include "Gander/Tuple.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"

#include "GanderImage/LayoutContainerBase.h"


namespace Gander
{

namespace Image
{

namespace Detail
{

template< class Layout >
struct LayoutContainer : public LayoutContainerBase< LayoutContainer< Layout >, Layout >
{
	private :
		
		typedef LayoutContainerBase< LayoutContainer< Layout >, Layout > BaseType;

	public :
	
		typedef typename BaseType::StorageType StorageType;
		typedef typename BaseType::LayoutType LayoutType;
	
		using BaseType::channel;
		using BaseType::channelAtIndex;

		LayoutContainer()
		{
		}

		LayoutContainer( const Layout &layout ) :
			BaseType( layout )
		{
		}
				
		inline void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			//\todo: We should really use the layouts indexOfChannel() method here to
			// correctly order the values in m_data. However, as only have one type of
			// dynamic layout, this code will do for now... It just orders the
			// data values to be the same as that of the ChannelSet. 
			
			// Get a set of the current channels and the unique new ones.
			ChannelSet currentChannels( BaseType::m_layout.channels() );
			ChannelSet newChannels( c - currentChannels );
			
			BaseType::m_layout.addChannels( c, b );

			// Loop over the new channels and insert a new data element for each one
			// at the same index that it is stored in the channel set.
			ChannelSet::const_iterator it( newChannels.begin() );
			ChannelSet::const_iterator end( newChannels.end() );
			for( ; it != end; ++it )
			{
				currentChannels += *it;
				int index = currentChannels.index( *it );
				m_data.insert( m_data.begin() + index, typename Layout::StorageType( 0 ) );
			}
		}

		template< EnumType Index, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false, class ChannelType = StorageType >
		inline ChannelType &channelAtIndex()
		{
			return m_data[ BaseType::m_layout.template maskedChannelIndex< Index, Mask, DisableStaticAsserts >() ];
		}

		template< ChannelDefault C, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false, class ChannelType = StorageType >
		inline ChannelType &channel()
		{
			if( BaseType::m_layout.template containsChannel< C >() )
			{
				return m_data[ BaseType::m_layout.template indexOfChannel< C, Mask, DisableStaticAsserts >() ];
			}
			else
			{
				GANDER_ASSERT( DisableStaticAsserts, "Channel does not exist in the LayoutContainer." );

				// The compiler never reaches here but we need to return a valid value to prevent any warnings.
				static StorageType unusedValue = StorageType(0);
				return unusedValue;
			}
		}
	
	private :
		
		typedef Gander::template Tuple< typename Layout::StorageType, Layout::NumberOfChannels, Layout::IsDynamic > LayoutContainerType;

		LayoutContainerType m_data;
		
		template< class, class > friend class LayoutContainerBase;	
};

}; // namespace Detail

}; // namespace Image

}; // namespace Gander

#endif
