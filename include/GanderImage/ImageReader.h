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

#include "OpenImageIO/imageio.h"
OIIO_NAMESPACE_USING

namespace Gander
{

namespace Image
{

template< class ImageType >
void readImage( ImageType &image, const std::string &path )
{
	ImageInput *file = ImageInput::open( path );
	GANDER_ASSERT( ( file != 0 ), boost::str( boost::format( "Failed to open file \"%s\"" ) % path ) );
	
	const ImageSpec &spec = file->spec();

	// Create the display window. The upper bounds are inclusive so we have to subtract 1.
	Gander::Box2i displayWindow( Eigen::Vector2i( spec.full_x, spec.full_y ), Eigen::Vector2i(  spec.full_x + spec.full_width - 1, spec.full_y + spec.full_height - 1 ) );
	image.setDisplayWindow( displayWindow );
	
	// Create the data window. The upper bounds are inclusive so we have to subtract 1.
	Gander::Box2i dataWindow( Eigen::Vector2i( spec.x, spec.y ), Eigen::Vector2i( spec.x + spec.width - 1, spec.y + spec.height - 1 ) );
	image.setDataWindow( yDownToDisplaySpace( displayWindow, dataWindow ) );
	
	const std::vector< std::string > &channelNames( spec.channelnames );
	ChannelSet availableChannels;
	
	for( std::vector< std::string >::const_iterator it( channelNames.begin() ); it != channelNames.end(); ++it )
	{
		Channel channel = ChannelSet::find( it->c_str() );
		
		if( channel == Chan_None )
		{
			GANDER_ASSERT( false, "The ImageReader doesn't recognise channel \"" + ( *it ) + "\"." );
		}
			
		availableChannels += channel;
	}

	GANDER_ASSERT( ( image.channels() == availableChannels ), "The image and the file to be read have different channels." );

	float imageData[ spec.width * spec.height * spec.nchannels ];
	if( spec.tile_width == 0 )
	{
		file->read_image( TypeDesc::FLOAT, &imageData[0] );
		for( ChannelSet::const_iterator it( image.requiredChannels().begin() ); it != image.requiredChannels().end(); ++it )
		{
			float *buffer = new float[ spec.width * spec.height ];
			int offset = image.requiredChannels().index( *it );
			for( int y = 0; y < spec.height; ++y )
			{
				for( int x = 0; x < spec.width; ++x, offset += spec.nchannels )
				{
					buffer[ spec.width * y + x ] = imageData[ offset ];
				}
			}
			image.setChannelPointer( *it, buffer, -(dataWindow.size()(0)+1) );
		}
	}
	else
	{
		GANDER_ASSERT( ( 0 ), "Reading of tiled images is not supported yet." );
	}
	
	file->close();
}

}; // namespace Image

}; // namespace Gander

#endif
