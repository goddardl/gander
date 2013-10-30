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

#include <string.h>

template< class T, class FlagDefaultsEnum, unsigned nDefaultFlags, class FlagSetInitEnum >
const char *Gander::Flags<T, FlagDefaultsEnum, nDefaultFlags, FlagSetInitEnum>::Flag::name( Flag z )
{
	if( z.m_value < static_cast<int>( g_flagMappings.size() ) )
	{
		return g_flagMappings[z.m_value];
	}
	return "unused";
}

template<class T, class FlagDefaultsEnum, unsigned nDefaultFlags, class FlagSetInitEnum>
typename Gander::Flags<T, FlagDefaultsEnum, nDefaultFlags, FlagSetInitEnum>::Flag Gander::Flags<T, FlagDefaultsEnum, nDefaultFlags, FlagSetInitEnum>::Flag::flag( const char *name )
{
	for( T i = 0; i < g_flagMappings.size(); ++i )
	{
		if( strcmp( g_flagMappings[i], name ) == 0 )
		{
			return Flag( i );
		}
	}
	g_flagMappings.push_back( name );
	return Flag( g_flagMappings.size() - 1 );
}

template<class T, class FlagDefaultsEnum, unsigned nDefaultFlags, class FlagSetInitEnum>
typename Gander::Flags<T, FlagDefaultsEnum, nDefaultFlags, FlagSetInitEnum>::Flag Gander::Flags<T, FlagDefaultsEnum, nDefaultFlags, FlagSetInitEnum>::Flag::findFlag( const char *name )
{
	for( T i = 0; i < g_flagMappings.size(); ++i )
	{
		if( strcmp( g_flagMappings[i], name ) == 0 )
		{
			return Flag( i );
		}
	}
	return Flag( 0 );
}

template<class T, class FlagDefaultsEnum, unsigned nDefaultFlags, class FlagSetInitEnum>
inline T Gander::Flags<T, FlagDefaultsEnum, nDefaultFlags,  FlagSetInitEnum>::Flag::index( Flag z )
{
	return z.m_value;
}


