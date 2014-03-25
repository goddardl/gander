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
#ifndef __GANDERIMAGE_IMAGE_H__
#define __GANDERIMAGE_IMAGE_H__

#include <vector>

#include "Gander/Common.h"

#include "GanderImage/Pixel.h"
#include "GanderImage/Row.h"

namespace Gander
{

namespace Image
{

template< class Layout >
class Image
{
	public : 
		
		typedef Layout LayoutType;
		typedef Image< Layout > Type;
		typedef typename Gander::Image::Pixel< Layout > Pixel;
		typedef typename Gander::Image::PixelAccessor< Layout > PixelAccessor;
		typedef typename Gander::Image::Row< Layout > Row;
		
		template< EnumType Index > struct LayoutTraits : public Layout::template LayoutTraits< Index > {};
		template< ChannelDefault C, bool DisableStaticAsserts = false > struct ChannelTraits : public Layout::template ChannelTraits< C, DisableStaticAsserts > {};
		template< EnumType Index > struct ChannelTraitsAtIndex : public Layout::template ChannelTraitsAtIndex< Index > {};

		/// Constructs a new image of a specified width and height.
		inline Image( int32u width, int32 height ) : m_width( width ), m_height( height ) {}

		inline bool isValid() const
		{
			return m_pixelAccessor.requiredChannels() == m_availableChannels;
		}

		inline unsigned int numberOfChannels() const { return m_pixelAccessor.numberOfChannels(); }
		inline ChannelSet channels() const { return m_pixelAccessor.channels(); }
		inline bool isDynamic() const { return m_pixelAccessor.isDynamic(); }
		inline unsigned int numberOfChannelPointers() const { return m_pixelAccessor.numberOfChannelPointers(); };
		inline ChannelSet requiredChannels() const { return m_pixelAccessor.requiredChannels(); };
		inline unsigned int width() const { return m_width; };
		inline unsigned int height() const { return m_height; };

		/// Adds a new channel to the image if the layout is dynamic.
		/// @param channel The channel to add to the image.
		void addChannels( ChannelSet channels, ChannelBrothers brothers = Brothers_None )
		{
			if( !m_pixelAccessor.channels().contains( channels ) )
			{
				m_pixelAccessor.addChannels( channels, brothers );
			}
		}

		/// Sets the channel pointer that represents a particular channel.
		/// @param buf A pointer to the channel data.
		/// @param stride The distance in bytes between the first element in a row and the first in the next.
		/// If the stride is negative then the image is flipped vertically.
		void setChannelPointer( Channel channel, void *buf, size_t stride )
		{
			if( stride < 0 ) 
			{ 
				buf = reinterpret_cast< unsigned char * >( buf ) - ( static_cast<size_t>( m_height ) - 1 ) * stride;
			}

			if( requiredChannels().contains( channel ) )
			{
				// Find out if we are replacing a channel or inserting a new one.
				if( m_availableChannels.contains( channel ) ) // Exists.
				{
					unsigned int index = m_availableChannels.index( channel );
					m_strides[index] = stride;
				}
				else // Does not exist.
				{
					m_availableChannels += channel;
					unsigned int index = m_availableChannels.index( channel );
					m_strides.insert( m_strides.begin() + index, stride );
				}
				
				m_pixelAccessor.setChannelPointer( channel, buf );
			}
			else
			{
				GANDER_ASSERT( false, "This image does not contain the requested channel in it's layout. If the layout is dynamic, add the channel before trying to set it's pointer." );
			}
		}

	private :

		/// Holds a pointer to the start of each channel in the image.	
		PixelAccessor m_pixelAccessor;

		/// Holds a list of the stride values for each channel. The values are ordered by channel index.
		std::vector< size_t > m_strides;

		/// Holds a set of all of the channels that have been set. This value is used
		/// to verify if the image is valid or not.
		ChannelSet m_availableChannels;

		unsigned int m_width;
		unsigned int m_height;
};

}; // namespace Image

}; // namespace Gander

#endif
