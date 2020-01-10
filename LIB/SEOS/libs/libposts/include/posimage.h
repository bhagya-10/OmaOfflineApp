/*
 * posimage.h
 *
 *  Created on: 2013-5-17
 *      Author: justin
 */

#ifndef POSIMAGE_H_
#define POSIMAGE_H_

/* Define png_jmpbuf() in case we are using a pre-1.0.6 version of libpng */
#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif

typedef unsigned short  WORD;
typedef	unsigned int	DWORD;
typedef int				LONG;
typedef unsigned char	BYTE;
typedef void*			LPVOID;

//位图文件头
typedef struct tagBITMAPFILEHEADER{
	WORD 	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
}BITMAPFILEHEADER;


//位图信息头
typedef struct tagBITMAPINFOHEADER{
	DWORD	biSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD	biPlanes;
	WORD	biBitCount;
	DWORD	biCompression;
	DWORD	biSizeImage;
	LONG	biXPelsPerMeter;
	LONG	biYPelsPerMeter;
	DWORD	biClrUsed;
	DWORD	biClrImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD{
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
}RGBQUAD;


#endif /* POSIMAGE_H_ */
