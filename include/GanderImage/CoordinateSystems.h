//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014, Luke Goddard. All rights reserved.
//  Copyright (c) 2012-2013 Image Engine Design Inc. All rights reserved.
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
#ifndef __GANDERIMAGE_COORDINATESYSTEMS_H__
#define __GANDERIMAGE_COORDINATESYSTEMS_H__

#include <vector>

#include "Gander/Common.h"
#include "Gander/Box.h"

namespace Gander
{

namespace Image
{

/// @name Coordinate system conversions.
/// The image coordinate system used by Gander has the origin at the
/// bottom, with increasing Y coordinates going up. The OpenEXR
/// coordinate system has the origin at the top with increasing Y
/// coordinates going down. These methods assist in converting between
/// the two coordinate systems. They assume that the display window is
/// using the corresponding Y-down space - note that this means it
/// is not necessary to perform any conversion on the display window
/// itself.
////////////////////////////////////////////////////////////////////
//@{
/// Converts from the Y-down coordinate space to the Y-up space of
/// the Format.
inline int yDownToDisplaySpace( const Gander::Box2i &displayWindow, int yDown );
inline Eigen::Vector2i yDownToDisplaySpace( const Gander::Box2i &displayWindow, const Eigen::Vector2i &yDown );
inline Gander::Box2i yDownToDisplaySpace( const Gander::Box2i &displayWindow, const Gander::Box2i &yDown );
/// Converts from the Y-up space of the display to the Y-down
/// coordinate space.
inline int displayToYDownSpace( const Gander::Box2i &displayWindow, int yUp );
inline Eigen::Vector2i displayToYDownSpace( const Gander::Box2i &displayWindow, const Eigen::Vector2i &yUp );
inline Gander::Box2i displayToYDownSpace( const Gander::Box2i &displayWindow, const Gander::Box2i &yUp );
//@}

}; // namespace Image

}; // namespace Gander

#include "GanderImage/CoordinateSystems.inl"

#endif
