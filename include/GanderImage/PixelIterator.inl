//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
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

template< class Layout >
template< class RhsLayout >	
ConstPixelIterator< Layout >::ConstPixelIterator( const ConstPixelIterator< RhsLayout > &it )
{
	if( std::template is_same< RhsLayout, Layout >::value )
	{
		BaseType::m_layout = it.m_layout;
	}
	else if( BaseType::m_layout.isDynamic() && it->channels().contains( BaseType::channels() ) )
	{
		ChannelSet missingChannels( it->channels() - BaseType::channels() );
		if( missingChannels.size() > 0 )
		{
			BaseType::addChannels( missingChannels );
		}

		typedef typename ConstPixelIterator< RhsLayout >::ConstPixelAccessor RhsPixelAccessor;
		static_cast< ConstPixelAccessor * >( this )->template copyFrom< RhsPixelAccessor >( static_cast< const RhsPixelAccessor & >( it ), true );
	}
	else
	{
		GANDER_ASSERT( false, "Cannot copy from an iterator that doesn't have the required channels." );
	}
};

template< class Layout >
template< class RhsLayout >	
ConstPixelIterator< Layout >::ConstPixelIterator( const PixelIterator< RhsLayout > &it )
{
	*this = static_cast< const typename Gander::Image::template ConstPixelIterator< RhsLayout > & >( it );
}

}; // namespace Image

}; // namespace Gander

