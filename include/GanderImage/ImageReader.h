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
#ifndef __GANDERIMAGE_IMAGEREADER_H__
#define __GANDERIMAGE_IMAGEREADER_H__

#include <vector>

#include "GanderImage/Image.h"
#include "GanderImage/Channel.h"
#include "GanderImage/CoordinateSystems.h"

#include "OpenImageIO/imagecache.h"
OIIO_NAMESPACE_USING

namespace Gander
{

namespace Image
{

namespace Detail
{

/// \todo Add a mutex to this image cache if we intend to have multiple threads use the ImageReader at the same time.
inline static ImageCache *imageCache()
{
	static ImageCache *cache = 0;
	if( cache == 0 )
	{
		cache = ImageCache::create();
	}
	return cache;
}

} // namespace Detail

template< class ImageType >
void readImage( ImageType &image, const std::string &path )
{
	const ImageSpec *spec = Detail::imageCache()->imagespec( ustring( path.c_str() ) );
	ustring uFileName( path.c_str() );

	// Create the display window. The upper bounds are inclusive so we have to subtract 1.
	Gander::Box2i displayWindow( Eigen::Vector2i( spec->full_x, spec->full_y ), Eigen::Vector2i(  spec->full_x + spec->full_width - 1, spec->full_y + spec->full_height - 1 ) );
	
	// Create the data window. The upper bounds are inclusive so we have to subtract 1.
	Gander::Box2i dataWindow( Eigen::Vector2i( spec->x, spec->y ), Eigen::Vector2i( spec->x + spec->width - 1, spec->y + spec->height - 1 ) );
	dataWindow = yDownToDisplaySpace( displayWindow, dataWindow );
	
	const std::vector< std::string > &channelNames( spec->channelnames );
	ChannelSet availableChannels;
	
	size_t channelIndices[ image.requiredChannels().size() ];
	for( std::vector< std::string >::const_iterator it( channelNames.begin() ); it != channelNames.end(); ++it )
	{
		Channel channel = ChannelSet::find( it->c_str() );
		
		if( channel == Chan_None )
		{
			GANDER_ASSERT( false, "The ImageReader doesn't recognise channel \"" + ( *it ) + "\"." );
		}
			
		availableChannels += channel;
		
		if( image.requiredChannels().contains( channel ) )
		{
			index[ image.requiredChannels().index( channel ) ] = it - channelNames.begin();
		}
	}

	GANDER_ASSERT( ( image.channels() == availableChannels ), "The image and the file to be read have different channels." );

	unsigned int i = 0;
	for( ChannelSet::const_iterator it( image.requiredChannels.begin() ); it != image.requiredChannels.end(); ++it, ++i )
	{
		const int newY = displayToYDownSpace( displayWindow, dataWindow.max(1) );
		float *ptr = ( dataWindow.size()(0) + 1 ) * ( dataWindow.size()(1) + 1 );
		Detail::imageCache()->get_pixels(
			uFileName,
			0, 0, // subimage, miplevel
			dataWindow.min(0), dataWindow.max(0) + 1,
			newY, newY + dataWindow.size()(1) + 1,
			0, 1,
			channelIndices[i], channelIndices[i] + 1,
			TypeDesc::FLOAT,
			ptr
		);
		image.setChannelPointer( *it, ptr, dataWindow.size()(1) + 1 );
	}

}

}; // namespace Image

}; // namespace Gander

#endif
