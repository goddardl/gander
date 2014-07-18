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
#ifndef __GANDERIMAGE_CACHE__
#define __GANDERIMAGE_CACHE__

namespace Gander
{

// The cache manages accesses to shared memory. Data can be requested using the read() and write() methods.
// Theses methods return a reference counted accessor that grants access to the data. A write accessor
// cannot be granted for any data entry that is already being read. The allocate() method creates entries in the
// cache and deallocate() removes them. The cache should copy data between devices as required. For example, if
// data is requested (using request()) or read from a device that is not the cpu, the data has to be copied.
// Memory limits should be able to be set for each device and an LRU system used when copying data to and from them.
// The hashes supplied with the methods are appended to the size of the data that is required.
class Cache
{

	enum
	{
		DEVICE_CPU = 0,
		DEVICE_GPU0 = 1,
		DEVICE_GPU1 = 2,
		DEVICE_GPU2 = 3,
		DEVICE_GPU3 = 4,
	};

	/*
	void deallocate( MurmurHash hash = MurmurHash() );
	void allocate( size_t sizeInBytes, MurmurHash hash = MurmurHash(), int deviceID = DEVICE_CPU );
	WriteAccessor write( MurmurHash hash, int deviceID = DEVICE_CPU );
	ReadAccessor read( MurmurHash hash, int deviceID = DEVICE_CPU );
	void request( MurmurHash hash, int deviceID = DEVICE_CPU );
	*/

}; // namespace Cache

}; // namespace Gander

#endif

