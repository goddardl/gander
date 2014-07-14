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
#ifndef __GANDERIMAGE_ROW__
#define __GANDERIMAGE_ROW__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Interfaces.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Pixel.h"
#include "GanderImage/PixelIterator.h"
#include "GanderImage/Image.h"

namespace Gander
{

namespace Image
{

template< class Layout >
class Row : public Gander::EqualComparisonOperators< Row< Layout > >
{
	public :
		
		typedef Layout LayoutType;
		typedef Row< LayoutType > Type;
		typedef typename Gander::Image::Image< LayoutType > ImageType;
		typedef typename Gander::Image::template Pixel< LayoutType > Pixel;
		typedef typename Gander::Image::template PixelIterator< LayoutType > PixelIterator;
		typedef typename Gander::Image::template ConstPixelIterator< LayoutType > ConstPixelIterator;
		typedef typename Gander::Image::template PixelAccessor< LayoutType > PixelAccessor;
		typedef typename Gander::Image::template ConstPixelAccessor< LayoutType > ConstPixelAccessor;

		inline Row() :
			m_width( 0 ),
			m_x( 0 )
		{
		}
		
		inline unsigned int width() const
		{
			return m_width;
		}

		inline ChannelSet channels() const
		{
			return m_start.channels();
		}

		inline unsigned int numberOfChannels() const
		{
			return m_start.numberOfChannels();
		}

		inline ConstPixelIterator &begin() const
		{
			return m_start;
		}

		inline PixelIterator &begin() 
		{
			return m_start;
		}

		inline ConstPixelIterator end() const
		{
			return PixelIterator( m_start ).increment( m_width );
		}
		
		template< class RhsLayoutType >
		inline bool equalTo( const Row< RhsLayoutType > &rhs ) const
		{
			return m_start == rhs.m_start && m_width == rhs.m_width && m_x == rhs.m_x;
		}

		inline void get( const ImageType &image, int y )
		{
			image.set( *this, y );
		}

		inline PixelAccessor operator [] ( int x )
		{
			return *( m_start + ( x - m_x ) );
		}
		
		inline ConstPixelAccessor operator [] ( int x ) const
		{
			return *( m_start + ( x - m_x ) );
		}
		
		inline int x() const
		{
			return m_x;
		}

	protected :
		
		template< class > friend class Image;
		
		PixelIterator m_start;
		unsigned int m_width;
		int m_x;

};

}; // namespace Image

}; // namespace Gander

#endif
