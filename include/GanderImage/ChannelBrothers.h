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
#ifndef __GANDERIMAGE_CHANNELBROTHERS__
#define __GANDERIMAGE_CHANNELBROTHERS__

#include <iostream>
#include <map>

#include "Gander/Common.h"
#include "GanderImage/Channel.h"

namespace Gander
{

namespace Image
{
    
/// Defines the different ways a set of channels can be interleaved together.
/// These are used by the Gander::Image::ImageFormat class to define the underlying structure
/// of an image's channel data.
/// For exmaple, if we define the image :
/// typedef Gander::Image::ImageFormat< Brothers_RGB > FormatRGB;
/// Then we are stating that the RGB channels of the image are actually interleaved
/// together.
/// If you wish to add define new channel brother values, ensure that you also add
/// the BrotherTraits specialization in "include/Gander/Image/BrotherTraits.inl"
/// or you will encounter a compile error prompting you to do so!
enum ChannelBrothers
{
    Brothers_None = 0,
    Brothers_RGB = 1,
    Brothers_RGBA = 2,
    Brothers_BGR = 3,
    Brothers_BGRA = 4,
    Brothers_UV = 5,
    Brothers_VU = 6,
};

/// A base class for the BrothersTraits classes to provide a registration mechanism for accessing the values of a BrotherTraits class at runtime.
/// BrotherTraitsRegistry is used as the base class for all BrotherTraits classes. It defines a registration mechanism which the enters the
/// classes create a static member of. When the static member is initialized at runtime, the constructor enters the useful enum values of the
/// data of a BrotherTraits class into a map which is keyed by the ChannelBrothers enum. This allows runtime code to query values such as
/// the number of brothers by calling a static function on the BrotherTraits<> class.
/// For example, to get a ChannelMask that contains a set of ChannelBrothers which are only computed at runtime, the following code can be used:
/// BrotherTraits<>::brotherMask( Brothers_RGB );
struct BrotherTraitsRegistry
{
	public :
		
		static inline ChannelSet channels( ChannelBrothers b )
		{
			return ChannelSet( traitsMap()[b].brotherMask );
		}

		static inline ChannelMask brotherMask( ChannelBrothers b )
		{
			return traitsMap()[b].brotherMask;
		}

		static inline int8u numberOfBrothers( ChannelBrothers b )
		{
			return traitsMap()[b].numberOfBrothers;
		}
		
		static inline Channel firstBrotherInBrothers( ChannelBrothers b )
		{
			return traitsMap()[b].firstBrotherInBrothers;
		}

		static inline int8u channelPositionInBrothers( ChannelBrothers b, Channel c )
		{
			if( traitsMap()[b].brotherOfLowestValue == 0 || b == Brothers_None )
			{
				return 0;
			}
			
			BOOST_ASSERT( channels( b ).contains( c ) );

			return traitsMap()[b].brotherOrder[ c - traitsMap()[b].brotherOfLowestValue ];
		}

	protected :

		template< ChannelBrothers B >
		struct BrotherTraitsRegistration
		{
			BrotherTraitsRegistration();
		};

	private :

		struct Traits
		{
			Channel brotherOfLowestValue;
			Channel firstBrotherInBrothers;
			int8u numberOfBrothers;
			ChannelMask brotherMask;
			int8u brotherOrder[4]; 

			/// The default constructor sets the number of brothers to 1
			/// as any channel is always a brother of itself.
			Traits() :
				brotherOfLowestValue( Chan_None ),
				firstBrotherInBrothers( Chan_None ),
				numberOfBrothers( 1 ),
				brotherMask( Mask_None )
			{
				brotherOrder[0] = 0;
				brotherOrder[1] = 1;
				brotherOrder[2] = 2;
				brotherOrder[3] = 3;
			};
		};

		static inline std::map< ChannelBrothers, Traits > &traitsMap()
		{
			static std::map< ChannelBrothers, Traits > g_brotherTraitsMap;
			return g_brotherTraitsMap;
		}
};

/// The definition of a struct that defines helpful enums that describe
/// the characteristics of a set of ChannelBrothers.
/// This class should be specialized by the developer and appropriate
/// values given to the various enums.
template< ChannelBrothers = Brothers_RGB >
struct BrotherTraits : public BrotherTraitsRegistry
{
	private :

		enum
		{
			INVALID_CHANNELBROTHERS__MAKE_SURE_A_SPECIALIZATION_EXISTS	= 1 >> -1, 
			Fail = INVALID_CHANNELBROTHERS__MAKE_SURE_A_SPECIALIZATION_EXISTS,
		};

		enum
		{
        /// A value that defines the offset required to right shift (>>) the
        /// channel with the lowest value in the set of brothers to bit 0.
        BrotherOfLowestValue = Fail,
        
        /// The number of channels in the set of brothers.
        /// For example, RGB would be 3.
        NumberOfBrothers = Fail,
        
        /// A series of enum, one for each channel in the set of brothers,  which specifies the
        /// position of a channel within a set of brothers.
        /// Set the enum to 0 for BrotherOrderX values where 'X' is greater than the number of brothers.
        /// ChannelReorderX : Defines the position of a channel in the brother set.
        ///                   If for example we wish to change the order of the Red channel from 
        ///                   index 0 to 3, we set the ChannelReorder'X' enum to 3,
        ///                   where 'X' = ( Chan_Red >> ( BrotherOfLowestValue - 1 ) ).
        BrotherOrder1 =	Fail,
        BrotherOrder2 =	Fail,
        BrotherOrder3 =	Fail,
        BrotherOrder4 =	Fail,
    };
};

/// Include the BroterTrait specializations.
#include "GanderImage/BrotherTraits.inl"

/// The BrotherTraitsRegistry constructor which enters the information from a BrotherTraits class into the registry.
template< ChannelBrothers B >
BrotherTraitsRegistry::BrotherTraitsRegistration<B>::BrotherTraitsRegistration()
{
	typedef BrotherTraits<B> Traits;
	BrotherTraitsRegistry::Traits &registryEntry = traitsMap()[B];
	registryEntry.numberOfBrothers = Traits::NumberOfBrothers;
	registryEntry.brotherMask = ChannelMask( Traits::BrothersMask );
	registryEntry.brotherOfLowestValue = Channel( Traits::BrotherOfLowestValue );
	registryEntry.brotherOrder[0] = Traits::BrotherOrder1;
	registryEntry.brotherOrder[1] = Traits::BrotherOrder2;
	registryEntry.brotherOrder[2] = Traits::BrotherOrder3;
	registryEntry.brotherOrder[3] = Traits::BrotherOrder4;
	registryEntry.firstBrotherInBrothers = Channel( Traits::FirstBrotherInBrothers );
};

}; // namespace Image

}; // namespace Gander

#endif
