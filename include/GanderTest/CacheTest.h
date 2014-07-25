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
#ifndef __GANDERTEST_CACHETEST_H__
#define __GANDERTEST_CACHETEST_H__

#include "boost/test/unit_test.hpp"

#include "Gander/Cache.h"

namespace Gander
{

namespace Test
{

namespace Detail
{

enum
{
	TestMemoryInterfaceTypeId1 = 0x10000001,
	TestMemoryInterfaceTypeId2 = 0x10000002,
	TestMemoryInterfaceTypeId3 = 0x10000003,
};

struct TestMemoryInterface1 : public MemoryInterface
{
	public :
	
		TestMemoryInterface1( int instanceId ) :
			MemoryInterface( instanceId )
		{
		}

	private :

		static MemoryInterfaceRegistration<TestMemoryInterface1> m_registration;
};

struct TestMemoryInterface2 : public MemoryInterface
{
	public :
	
		TestMemoryInterface2( int instanceId ) :
			MemoryInterface( instanceId )
		{
		}

	private :

		static MemoryInterfaceRegistration<TestMemoryInterface2> m_registration;
};

struct TestMemoryInterface3 : public MemoryInterface
{
	public :
	
		TestMemoryInterface3( int instanceId ) :
			MemoryInterface( instanceId )
		{
		}

	private :

		static MemoryInterfaceRegistration<TestMemoryInterface3> m_registration;
};

}; // namespace Detail

void addCacheTest( boost::unit_test::test_suite *test );

}; // namespace Test

}; // namespace Gander

#endif // __GANDERTEST_CACHETEST_H__
