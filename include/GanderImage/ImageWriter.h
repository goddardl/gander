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
#ifndef __GANDERIMAGE_IMAGEWRITER_H__
#define __GANDERIMAGE_IMAGEWRITER_H__

#include <vector>

#include "boost/format.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"

#include "GanderImage/Image.h"
#include "GanderImage/Row.h"
#include "GanderImage/Channel.h"
#include "GanderImage/CoordinateSystems.h"

#include "Gander/Assert.h"

#include "OpenImageIO/imageio.h"
#include "OpenImageIO/imagecache.h"
OIIO_NAMESPACE_USING

namespace Gander
{

namespace Image
{

template< class ImageType >
void writeImage( ImageType &image, const std::string &path )
{

	boost::shared_ptr< ImageOutput > out( ImageOutput::create( path.c_str() ) );
	if ( !out )
	{
		GANDER_ASSERT( 0, boost::str( boost::format( "Invalid filename: %s" ) % path.c_str() ) );
	}

	// Get the image's display window.
	Gander::Box2i displayWindow = image.getDisplayWindow();
	const int displayWindowWidth = displayWindow.size()(0)+1;
	const int displayWindowHeight = displayWindow.size()(1)+1;
	
	// Get the image's data window.
	Gander::Box2i dataWindow = displayToYDownSpace( displayWindow, image.getDataWindow() );
	const int dataWindowWidth = dataWindow.size()(0)+1;
	const int dataWindowHeight = dataWindow.size()(1)+1;
		
	// The number of channels to write.
	const int nChannels = image.numberOfChannels();
	
	// Create the image header. 
	ImageSpec spec( dataWindowWidth, dataWindowHeight, image.numberOfChannels(), TypeDesc::FLOAT );

	// Specify the display window.
	spec.full_x = displayWindow.min(0);
	spec.full_y = displayWindow.min(1);
	spec.full_width = displayWindowWidth;
	spec.full_height = displayWindowHeight;
	spec.x = dataWindow.min(0);
	spec.y = dataWindow.min(1);
	spec.width = dataWindow.size()(0) + 1;
	spec.height = dataWindow.size()(1) + 1;

	// Add the channel names to the header whilst getting pointers to the channel data.
	std::vector<const float*> channelPtrs;
	spec.channelnames.clear();
		
	for ( ChannelSet::Iterator channelIt( image.channels().begin() ); channelIt != image.channels().end(); channelIt++ )
	{
		spec.channelnames.push_back( ChannelSet::name( ChannelDefault( *channelIt ) ) );

		// OIIO has a special attribute for the Alpha and Z channels. If we find some, we should tag them...
		if ( *channelIt == Chan_Alpha )
		{
			spec.alpha_channel = image.channels().index( *channelIt );
		}
		else if ( *channelIt == Chan_Z )
		{
			spec.z_channel = image.channels().index( *channelIt );
		}

	}

	if ( !out->open( path, spec ) )
	{
		GANDER_ASSERT( 0, boost::str( boost::format( "Could not open \"%s\", error = %s" ) % path % out->geterror() ) );
	}

	// Create a buffer for the scanline.
	float scanline[ nChannels*dataWindowWidth ];

	bool imageIsBlack = 0;
	if ( imageIsBlack )
	{
		memset( scanline, 0, sizeof(float) * nChannels * dataWindowWidth );

		for ( int y = spec.y; y < spec.y + dataWindowHeight; ++y )
		{
			if ( !out->write_scanline( y, 0, TypeDesc::FLOAT, &scanline[0] ) )
			{
				GANDER_ASSERT( 0, boost::str( boost::format( "Could not write scanline to \"%s\", error = %s" ) % path % out->geterror() ) );
			}
		}
	}
	else
	{
		typename ImageType::Row row;

		// Interleave the channel data and write it by scanline to the file.
		for ( int y = image.getDataWindow().min(1); y <= image.getDataWindow().max(1); ++y )
		{
			image.set( row, y );
			typename ImageType::Row::ConstPixelIterator end( row.end() );
			
			for ( ChannelSet::Iterator channelIt( image.channels().begin() ); channelIt != image.channels().end(); ++channelIt )
			{
				float *outPtr = &scanline[0] + image.channels().index( *channelIt ); // The pointer that we are writing to.
				for ( typename ImageType::Row::ConstPixelIterator it( row.begin() ); it != end; ++it, outPtr += nChannels )
				{
					*( outPtr ) = it->getChannel( *channelIt );
				}
			}

			if ( !out->write_scanline( y, 0, TypeDesc::FLOAT, &scanline[0] ) )
			{
				GANDER_ASSERT( 0, boost::str( boost::format( "Could not write scanline to \"%s\", error = %s" ) % path % out->geterror() ) );
			}
		}
	}
	out->close();
}

}; // namespace Image

}; // namespace Gander

#endif
