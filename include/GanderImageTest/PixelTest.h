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
#ifndef __GANDERTEST_PIXELTEST_H__
#define __GANDERTEST_PIXELTEST_H__

#include "GanderImage/Pixel.h"
#include "GanderImage/Channel.h"
#include "GanderImage/DynamicLayout.h"
#include "GanderImage/ChannelLayout.h"
#include "GanderImage/BrothersLayout.h"
#include "GanderImage/CompoundLayout.h"

#include <vector>

#include "boost/test/unit_test.hpp"


namespace Gander
{

namespace ImageTest
{

namespace Detail
{

template< class Pixel >
void initPixel( Pixel &pixel )
{
	pixel.template channel< Gander::Image::Chan_Red >() = 1.;
	pixel.template channel< Gander::Image::Chan_Green >() = 2.;
	pixel.template channel< Gander::Image::Chan_Blue >() = 3.;
	pixel.template channel< Gander::Image::Chan_Alpha >() = 4.;
	pixel.template channel< Gander::Image::Chan_Z >() = 5.;
	pixel.template channel< Gander::Image::Chan_U >() = 6.;
	pixel.template channel< Gander::Image::Chan_V >() = 7.;
	pixel.template channel< Gander::Image::Chan_Backward >() = 8.;
}

}; // namespace Detail

void addPixelTest( boost::unit_test::test_suite *test );

}; // namespace ImageTest

}; // namespace Gander

#endif // __GANDERTEST_PIXELBASETEST_H__
