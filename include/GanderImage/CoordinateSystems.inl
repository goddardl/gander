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

namespace Gander
{

namespace Image
{

inline int yDownToDisplaySpace( const Gander::Box2i &displayWindow, int yDown )
{
	const int distanceFromTop = yDown - displayWindow.min(1);
	return displayWindow.max(1) - distanceFromTop;
}

inline Eigen::Vector2i yDownToDisplaySpace( const Gander::Box2i &displayWindow, const Eigen::Vector2i &yDown )
{
	return Eigen::Vector2i( yDown(0), yDownToDisplaySpace( displayWindow, yDown(1) ) );
}

inline Gander::Box2i yDownToDisplaySpace( const Gander::Box2i &displayWindow, const Gander::Box2i &yDown )
{
	Gander::Box2i result;
	result.extendBy( yDownToDisplaySpace( displayWindow, yDown.min ) );
	result.extendBy( yDownToDisplaySpace( displayWindow, yDown.max ) );
	return result;
}

inline int displayToYDownSpace( const Gander::Box2i &displayWindow, int yUp )
{
	const int distanceFromTop = displayWindow.max(1) - yUp;
	return displayWindow.min(1) + distanceFromTop;
}

inline Eigen::Vector2i displayToYDownSpace( const Gander::Box2i &displayWindow, const Eigen::Vector2i &yUp )
{
	return Eigen::Vector2i( yUp(0), displayToYDownSpace( displayWindow, yUp(1) ) );
}

inline Gander::Box2i displayToYDownSpace( const Gander::Box2i &displayWindow, const Gander::Box2i &yUp )
{
	Gander::Box2i result;
	result.extendBy( displayToYDownSpace( displayWindow, yUp.min ) );
	result.extendBy( displayToYDownSpace( displayWindow, yUp.max ) );
	return result;
}

}; // namespace Image

}; // namespace Gander

