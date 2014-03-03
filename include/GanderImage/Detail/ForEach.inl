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


#include "Gander/Assert.h"

/// This file contains the implementation of the ForEach method.
/// The ForEach method is implemented using a recursive template that calls itself for
/// every pair of similar channels in the two pixel classes. For every call to itself,
/// the template applies the functor to the pair of channels.
namespace Gander
{

namespace Image
{

namespace Detail
{

template< class Pixel1, class Pixel2, class Op >
void ForEachOpAtRuntime1( const Pixel1 &p1, const Pixel2 &p2, Op &op, Channel channel )
{
}

template< class Pixel1, class Pixel2, class Op, Gander::EnumType FullMask, Gander::EnumType StaticMask, Gander::EnumType PartialMask > struct ForEachRecurse;

template< class Pixel1, class Pixel2, class Op, Gander::EnumType FullMask, Gander::EnumType StaticMask > struct ForEachRecurse< Pixel1, Pixel2, Op, FullMask, StaticMask, 0 >
{
	inline void operator () ( const Pixel1 &p1, const Pixel2 &p2, Op &op ) const
	{
		if( p1.isDynamic() || p2.isDynamic() )
		{
			Gander::Image::ChannelSet dynamicChannels( Gander::Image::ChannelMask( FullMask & ( ~StaticMask ) ) ); 
			dynamicChannels &= p1.channels();
			dynamicChannels &= p2.channels();

			GANDER_ASSERT( 0, "Dynamic channels are not full supported yet. Seet the todo in Gander::Image::ForEachRecurse." );
			for( ChannelSet::const_iterator it( dynamicChannels.begin() ); it != dynamicChannels.end(); ++it )
			{
				/*
				switch( *it )
				{
					case( 1 ) : op( p1.template channel< ChannelDefault( 1 ) >(), p2.template channel< ChannelDefault( 1 ) >() ); break;
					case( 2 ) : op( p1.template channel< ChannelDefault( 2 ) >(), p2.template channel< ChannelDefault( 2 ) >() ); break;
					case( 3 ) : op( p1.template channel< ChannelDefault( 3 ) >(), p2.template channel< ChannelDefault( 3 ) >() ); break;
					case( 4 ) : op( p1.template channel< ChannelDefault( 4 ) >(), p2.template channel< ChannelDefault( 4 ) >() ); break;
					case( 5 ) : op( p1.template channel< ChannelDefault( 5 ) >(), p2.template channel< ChannelDefault( 5 ) >() ); break;
					case( 6 ) : op( p1.template channel< ChannelDefault( 6 ) >(), p2.template channel< ChannelDefault( 6 ) >() ); break;
					case( 7 ) : op( p1.template channel< ChannelDefault( 7 ) >(), p2.template channel< ChannelDefault( 7 ) >() ); break;
					case( 8 ) : op( p1.template channel< ChannelDefault( 8 ) >(), p2.template channel< ChannelDefault( 8 ) >() ); break;
					case( 9 ) : op( p1.template channel< ChannelDefault( 9 ) >(), p2.template channel< ChannelDefault( 9 ) >() ); break;
					case( 10 ) : op( p1.template channel< ChannelDefault( 10 ) >(), p2.template channel< ChannelDefault( 10 ) >() ); break;
					default : GANDER_ASSERT( 0, "Channel does not exist in the LayoutContainer." ); break;
				}
				*/
			}
		}
	};

	inline void operator () ( Pixel1 &p1, Pixel2 &p2, Op &op )
	{
		if( p1.isDynamic() || p2.isDynamic() )
		{
			Gander::Image::ChannelSet dynamicChannels( Gander::Image::ChannelMask( FullMask & ( ~StaticMask ) ) ); 
			dynamicChannels &= p1.channels();
			dynamicChannels &= p2.channels();

			GANDER_ASSERT( 0, "Dynamic channels are not full supported yet. Seet the todo in Gander::Image::ForEachRecurse." );
			for( ChannelSet::const_iterator it( dynamicChannels.begin() ); it != dynamicChannels.end(); ++it )
			{
				/*
				switch( *it )
				{
					case( 1 ) : op( p1.template channel< ChannelDefault( 1 ) >(), p2.template channel< ChannelDefault( 1 ) >() ); break;
					case( 2 ) : op( p1.template channel< ChannelDefault( 2 ) >(), p2.template channel< ChannelDefault( 2 ) >() ); break;
					case( 3 ) : op( p1.template channel< ChannelDefault( 3 ) >(), p2.template channel< ChannelDefault( 3 ) >() ); break;
					case( 4 ) : op( p1.template channel< ChannelDefault( 4 ) >(), p2.template channel< ChannelDefault( 4 ) >() ); break;
					case( 5 ) : op( p1.template channel< ChannelDefault( 5 ) >(), p2.template channel< ChannelDefault( 5 ) >() ); break;
					case( 6 ) : op( p1.template channel< ChannelDefault( 6 ) >(), p2.template channel< ChannelDefault( 6 ) >() ); break;
					case( 7 ) : op( p1.template channel< ChannelDefault( 7 ) >(), p2.template channel< ChannelDefault( 7 ) >() ); break;
					case( 8 ) : op( p1.template channel< ChannelDefault( 8 ) >(), p2.template channel< ChannelDefault( 8 ) >() ); break;
					case( 9 ) : op( p1.template channel< ChannelDefault( 9 ) >(), p2.template channel< ChannelDefault( 9 ) >() ); break;
					case( 10 ) : op( p1.template channel< ChannelDefault( 10 ) >(), p2.template channel< ChannelDefault( 10 ) >() ); break;
					default : GANDER_ASSERT( 0, "Channel does not exist in the LayoutContainer." ); break;
				}
				*/
			}
		}
	};
};
	
template< class Pixel1, class Pixel2, class Op, Gander::EnumType FullMask, Gander::EnumType StaticMask, Gander::EnumType PartialMask >
struct ForEachRecurse
{
	enum
	{
		Index = Gander::template EnumHelper< PartialMask >::FirstSetBit,
		NextPartialMask = PartialMask & ( ( ~( Gander::EnumType(0) ) ) << ( Index + 1 ) ),
		Channel = Index + 1,
	};
	
	inline void operator () ( Pixel1 &p1, Pixel2 &p2, Op &op )
	{
		op( p1.template channel< Gander::Image::ChannelDefault( Channel ) >(), p2.template channel< Gander::Image::ChannelDefault( Channel ) >()  );
		ForEachRecurse< Pixel1, Pixel2, Op, FullMask, StaticMask, NextPartialMask >()( p1, p2, op );
	}

	inline void operator () ( const Pixel1 &p1, const Pixel2 &p2, Op &op ) const
	{
		op( p1.template channel< Gander::Image::ChannelDefault( Channel ) >(), p2.template channel< Gander::Image::ChannelDefault( Channel ) >()  );
		ForEachRecurse< Pixel1, Pixel2, Op, FullMask, StaticMask, NextPartialMask >()( p1, p2, op );
	}
};

}; // namespace Detail

}; // namespace Image

}; // namespace Gander

