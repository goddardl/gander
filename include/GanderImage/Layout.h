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
#ifndef __GANDERIMAGE_LAYOUT__
#define __GANDERIMAGE_LAYOUT__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{

/// The base class for defining how a set of channels are grouped together and of what type they are.
/// The Layout class defines an interface for the derived classes using static polymorphism.
/// The Derived classes need to implement several Enum values, a type declaration for the StorageType of the channels,
/// a _channels() method, a _requiredChannels() method and a _numberOfChannels() method.
/// In addition to these, the equalTo method can be optionally overridden.
template< class Derived >
struct Layout
{
	
	public :
	
		enum
		{
			NumberOfLayouts = 1,
			IsDynamic = false,
		};

		inline ChannelSet channels() const
		{
			return static_cast< Derived const * >( this )->_channels();
		}

		inline unsigned int numberOfChannels() const
		{
			return static_cast< Derived const * >( this )->_numberOfChannels();
		}

		inline ChannelSet requiredChannels() const
		{
			return static_cast< Derived const * >( this )->_requiredChannels();
		}

		template< class L > inline bool operator == ( L const &rhs ) const { return static_cast< Derived const * >( this )->equalTo( rhs ); }
		template< class L > inline bool operator != ( L const &rhs ) const { return !static_cast< Derived const * >( this )->equalTo( rhs ); }

		template< ChannelDefault C >
		struct ChannelTraits
		{
			typedef Derived LayoutType;
			typedef typename LayoutType::StorageType StorageType;
		
			// Assert that the layout actually contains the requested channel.
			GANDER_IMAGE_STATIC_ASSERT( ( LayoutType::ChannelMask & ChannelToMask<C>::Value ) != 0, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
		};

	private :	

		/// Deried classes need to override this function to return the channels that this layout represents.
		/// If they do not, a static assertion prevents the code from compiling.
		inline ChannelSet _channels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return ChannelSet(); // We never get here.
		}

		/// Deried classes need to override this function to return the number of channels that this layout represents.
		/// If they do not, a static assertion prevents the code from compiling.
		inline unsigned int _numberOfChannels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}

		/// A function for comapring two Layouts. The overloaded equality operators call this method on the Derived class,
		/// allowing the Derived class to overload this function.
		template< class L >
		inline bool equalTo( const L &rhs ) const
		{
			return (
				rhs.channels() == channels() &&
				std::is_same< typename L::StorageType, typename Derived::StorageType >::value
		   );
		}

};

}; // namespace Image

}; // namespace Gander

#endif
