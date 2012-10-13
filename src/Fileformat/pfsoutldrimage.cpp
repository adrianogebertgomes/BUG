/**
 * @brief Writing QImage from PFS stream (which is a tonemapped LDR)
 *
 * This file is a part of Qtpfsgui package.
 * ----------------------------------------------------------------------
 * Copyright (C) 2006 Giuseppe Rota
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ----------------------------------------------------------------------
 *
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 *
 */

#include <iostream>
#include "../Libpfs/pfs.h"
#include <QImage>

#include <QSysInfo>

static inline unsigned char clamp( const float v, const unsigned char minV, const unsigned char maxV )
{
    if( v < /*(float)*/minV ) return minV;
    if( v > /*(float)*/maxV ) return maxV;
    return (unsigned char)v;
}


#include "../Threads/tonemapperThread.h"
QImage TonemapperThread::fromLDRPFStoQImage( pfs::Frame* inpfsframe ) {
	assert(inpfsframe!=NULL);

	pfs::DOMIO pfsio;
	pfs::Channel *R, *G, *B;
	inpfsframe->getRGBChannels( R,G,B );
	assert( R!=NULL && G!=NULL && B!=NULL );

	//inpfsframe's colorspace is either sRGB or RGB.
	//sRGB is used for compatibility with pfstmo.
	//fattal, reinhard05 and ashickmin (somewhat) prefer a RGB colorspace

	int width = R->getCols();
	int height =  R->getRows();
	uchar *data=new uchar[width*height*4]; //this will contain the image data: data must be 32-bit aligned, in Format: 0xffRRGGBB
	for( int y = 0; y < height; y++ ) { // For each row of the image
		for( int x = 0; x < width; x++ ) {
			if (QSysInfo::ByteOrder == QSysInfo::LittleEndian) {
			*(data + 0 + (y*width+x)*4) = ( clamp( (*B)( x, y )*255.f, 0, 255) );
			*(data + 1 + (y*width+x)*4) = ( clamp( (*G)( x, y )*255.f, 0, 255) );
			*(data + 2 + (y*width+x)*4) = ( clamp( (*R)( x, y )*255.f, 0, 255) );
			*(data + 3 + (y*width+x)*4) = 0xff;
			} else {
			*(data + 3 + (y*width+x)*4) = ( clamp( (*R)( x, y )*255.f, 0, 255) );
			*(data + 2 + (y*width+x)*4) = ( clamp( (*G)( x, y )*255.f, 0, 255) );
			*(data + 1 + (y*width+x)*4) = ( clamp( (*B)( x, y )*255.f, 0, 255) );
			*(data + 0 + (y*width+x)*4) = 0xff;
			}
		}
	}

	return QImage (const_cast<uchar *>(data),width,height,QImage::Format_ARGB32);
}
