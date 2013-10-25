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
#ifndef __GANDER_TYPES_H__
#define __GANDER_TYPES_H__

/// We create several defines for the basic types. This allows use to configure the types using 
/// the compiler's command line.
#ifndef GANDER_INT8
#define GANDER_INT8 signed char
#endif

#ifndef GANDER_INT8U
#define GANDER_INT8U unsigned char
#endif

#ifndef GANDER_INT16
#define GANDER_INT16 short
#endif

#ifndef GANDER_INT16U
#define GANDER_INT16U unsigned short
#endif

#ifndef GANDER_INT32
#define GANDER_INT32 int
#endif

#ifndef GANDER_INT32U
#define GANDER_INT32U unsigned int
#endif

#ifndef GANDER_INT64
#define GANDER_INT64 signed long long
#endif

#ifndef GANDER_INT64U
#define GANDER_INT64U unsigned long long
#endif

namespace Gander
{
	typedef GANDER_INT8 int8;
	typedef GANDER_INT8U int8u;
	typedef GANDER_INT16 int16;
	typedef GANDER_INT16U int16u;
	typedef GANDER_INT32 nt32;
	typedef GANDER_INT32U int32u;
	typedef GANDER_INT64 int64;
	typedef GANDER_INT64U int64u;
}; // Gander


#endif
