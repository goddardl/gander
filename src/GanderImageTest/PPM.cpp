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

#include <stdexcept>

#include "GanderImageTest/PPM.h"

using namespace Gander;
using namespace Gander::ImageTest;

namespace Gander
{

namespace ImageTest
{

PPM::~PPM()
{
	if( buf != NULL )
	{
		delete [] buf;
	}
}

bool PPM::operator == ( const PPM &rhs ) const
{
	if( width != rhs.width || height != rhs.height )
	{
		return false;
	}

	const int8u *end = buf + width * height * 3;
	const int8u *in1 = buf;
	const int8u *in2 = rhs.buf;
	while( in1 != end )
	{
		if( *in1++ != *in2++ )
		{
			return false;
		}
	}
	return true;
}

void PPM::read( std::string file )
{
	char buff[3];

	// Open PPM file for reading.
	FILE *fp = fopen( file.c_str(), "rb" );

	if( !fp )
	{
		throw std::runtime_error( "Failed to open file for reading." );
	}

	// Read image format.
	if( !fgets( buff, sizeof( buff ), fp ) )
	{
		perror( file.c_str() );
		throw std::runtime_error( "Failed to read file header." );
	}

	// Check the image format.
	if( buff[0] != 'P' || buff[1] != '6' )
	{
		throw std::runtime_error( "Invalid image format (must be 'P6')." );
	}

	// Read image size information.
	int rgbType = 0;
	if( fscanf( fp, "%d %d %d", &width, &height, &rgbType ) != 3 )
	{
		throw std::runtime_error( "Invalid image header." );
	}

	while( fgetc( fp ) != ' ' );

	if( buf != NULL ) delete [] buf;
		
	// Memory allocation for pixel data.
	buf = new int8u[ width * height * 3 ];

	// Read pixel data from file
	size_t count = sizeof( int8u ) * width * height;
	size_t bytes = fread( buf, sizeof( int8u ) * 3, width * height, fp );
	if( bytes != count )
	{
		throw std::runtime_error( "Error reading pixel data." );
	}

	fclose(fp);
}

bool PPM::write( std::string file ) const
{
	FILE* fd = fopen( file.c_str(), "wb" );
	if( fd )
	{
		fprintf(fd, "P6 %d %d 255 ", width, height);
		fwrite( buf, sizeof( int8u ), width * height * 3, fd );
		fclose(fd);
		return true;
	}
	return false;
}

} // namespace ImageTest

} // namespace Gander

