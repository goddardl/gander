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
#include "Gander/Box.h"

#include "GanderImage/Pixel.h"
#include "GanderImage/PixelIterator.h"

namespace Gander
{

namespace Image
{

// Forward declaration of the Row class.
template< class Layout > class Row;

template< class Layout >
class Image
{
	public : 
		
		typedef Layout LayoutType;
		typedef Image< Layout > Type;
		typedef typename Gander::Image::Pixel< Layout > Pixel;
		typedef typename Gander::Image::PixelAccessor< Layout > PixelAccessor;
		typedef typename Gander::Image::PixelIterator< Layout > PixelIterator;
		typedef typename Gander::Image::ConstPixelIterator< Layout > ConstPixelIterator;
		typedef typename Gander::Image::Row< Layout > Row;
		
		template< EnumType Index > struct LayoutTraits : public Layout::template LayoutTraits< Index > {};
		template< ChannelDefault C, bool DisableStaticAsserts = false > struct ChannelTraits : public Layout::template ChannelTraits< C, DisableStaticAsserts > {};
		template< EnumType Index > struct ChannelTraitsAtIndex : public Layout::template ChannelTraitsAtIndex< Index > {};
		
		/// Constructs a new image of an unspecified size.
		inline Image()
		{
			init();
		}

		/// Constructs a new image of a specified width and height.
		inline Image( int32u width, int32 height )
		{
			init();
			
			Gander::Box2i box( Eigen::Vector2i( 0, 0 ), Eigen::Vector2i( width - 1, height - 1 ) );
			setDataWindow( box );
			setDisplayWindow( box );
		}

		inline bool isValid() const
		{
			if( m_rowAccessors.empty() )
			{
				return false;
			}

			return requiredChannels() == m_availableChannels && width() > 0 && height() > 0 && !m_dataWindow.isEmpty();
		}

		bool operator == ( const Type &rhs ) const
		{
			if( rhs.getDataWindow() != getDataWindow() || rhs.getDisplayWindow() != getDisplayWindow() || rhs.m_rowAccessors.size() != m_rowAccessors.size() )
			{
				return false;
			}

			for( int i = getDataWindow().min(1); i <= getDataWindow().max(1); ++i )
			{
				for( int j = getDataWindow().min(0); j <= getDataWindow().max(0); ++j )
				{
					if( (*this)[i][j] != rhs[i][j] )
					{
						return false;
					}
				}
			}
		
			return true;
		}

		static inline unsigned int numberOfChannels() { return LayoutType::numberOfChannels(); }
		static inline ChannelSet channels() { return LayoutType::channels(); }
		static inline unsigned int numberOfChannelPointers() { return LayoutType::numberOfChannelPointers(); };
		static inline ChannelSet requiredChannels() { return LayoutType::requiredChannels(); };
		inline unsigned int width() const { return m_dataWindow.size()(0)+1; };
		inline unsigned int height() const { return m_dataWindow.size()(1)+1; };
		inline void setDataWindow( const Gander::Box2i &box ) { init(); m_dataWindow = box; }
		inline void setDisplayWindow( const Gander::Box2i &box ) { m_displayWindow = box; }
		inline const Gander::Box2i &getDataWindow() const { return m_dataWindow; }
		inline const Gander::Box2i &getDisplayWindow() const { return m_displayWindow; }

		/// Sets the channel pointer that represents a particular channel.
		/// @param buf A pointer to the channel data.
		/// @param stride The distance between the first element in a row and the first in the next.
		void setChannelPointer( Channel channel, void *buf, int stride )
		{
			GANDER_ASSERT( m_availableChannels != requiredChannels(), "This image has already been initialized." );
			if( requiredChannels().contains( channel ) )
			{
				// Find out if we are replacing a channel or inserting a new one.
				unsigned int index = requiredChannels().index( channel );
				m_availableChannels += channel;

				if( m_rowAccessors.empty() )
				{
					GANDER_ASSERT( !m_dataWindow.isEmpty(), "The data window must be set before the channel pointers." );
					m_rowAccessors.resize( m_dataWindow.size()(1)+1 );
				}
				
				if( stride < 0 )
				{
					m_strides[index] = -stride;
					for( int i = 0; i <= m_dataWindow.size()(1); ++i )
					{
						m_rowAccessors[i].setChannelPointer( channel, buf );
						m_rowAccessors[i].increment( ( m_dataWindow.size()(1) - i ) * (-stride), channel );
					}
				}
				else
				{
					m_strides[index] = stride;
					for( int i = 0; i <= m_dataWindow.size()(1); ++i )
					{
						m_rowAccessors[i].setChannelPointer( channel, buf );
						m_rowAccessors[i].increment( stride * i, channel );
					}
				}

			}
			else
			{
				GANDER_ASSERT( false, "This image does not contain the requested channel in it's layout." );
			}
		}

		inline void set( Row &row, int y ) const
		{
			int index = y - m_dataWindow.min(1);

			GANDER_ASSERT( isValid(), "The image is not valid." );
			GANDER_ASSERT( index < int( m_rowAccessors.size() ) && index >= 0, "Row access is out of bounds." );

			row.m_width = width();
			row.m_x = getDataWindow().min(0);
			row.m_start = m_rowAccessors[index];
		}

		inline Row operator [] ( int y ) const
		{
			Row row;
			set( row, y );
			return row;
		}

	private :

		inline void init()
		{	
			m_rowAccessors.clear();
			m_availableChannels.clear();
			m_strides.resize( requiredChannels().size() );
		}
	
		/// The data window of the image.
		Gander::Box2i m_dataWindow;
		
		/// The display window of the image.	
		Gander::Box2i m_displayWindow;

		/// Holds a pointer to the start of each row in the image.	
		std::vector<PixelIterator> m_rowAccessors;

		/// Holds a list of the stride values for each channel. The values are ordered by channel index.
		std::vector< int > m_strides;

		/// Holds a set of all of the channels that have been set. This value is used
		/// to verify if the image is valid or not.
		ChannelSet m_availableChannels;
};

}; // namespace Image

}; // namespace Gander

#endif
