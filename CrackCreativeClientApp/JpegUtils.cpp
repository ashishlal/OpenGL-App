//
//  JpegUtils.cpp
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 24/11/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#include "JpegUtils.h"

boolean JpegDecompressNOTUSED(unsigned char *src, int srcLen, unsigned char *dest, int format, unsigned int size)
{

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned char * line;
	
	cinfo.err = jpeg_std_error (&jerr);
	jpeg_create_decompress (&cinfo);
	
	//if (0 == (fd = fopen(filename, "rb")))
	//	return false;
	
	//jpeg_stdio_src (&cinfo, fd);
	jpeg_mem_src(&cinfo, src, srcLen);
	jpeg_read_header (&cinfo, TRUE);
	if ((cinfo.image_width != size) || (cinfo.image_height != size))
		return false;
	
	if (MY_GL_RGBA == format)
    {
		if (cinfo.out_color_space == JCS_GRAYSCALE)
			return false;
    }
	else
		if (cinfo.out_color_space != JCS_GRAYSCALE)
			return false;
	
	jpeg_start_decompress (&cinfo);
	
	while (cinfo.output_scanline < cinfo.output_height)
    {
		line = dest +
			(MY_GL_RGBA == format ? 3 * size : size) * cinfo.output_scanline;
		jpeg_read_scanlines (&cinfo, &line, 1);
    }
	jpeg_finish_decompress (&cinfo);
	jpeg_destroy_decompress (&cinfo);
	return true;
	
}

boolean JpegDecompress(unsigned char *src, int srcLen, unsigned char *dest, int format, 
					   unsigned int *width, unsigned int *height)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;		/* Output row buffer */
	unsigned row_stride;		/* physical row width in output buffer */
	unsigned pixelcount, memcount;
	unsigned char *out;
	int len=srcLen;
	unsigned char	*fbuffer=src;
	unsigned char  *buf=dest;
	
	cinfo.err = jpeg_std_error(&jerr);
	
	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);
	
	jpeg_mem_src(&cinfo, fbuffer, len);
	
	(void) jpeg_read_header(&cinfo, TRUE);
	
	(void) jpeg_start_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	 * with the stdio data source.
	 */
	
	/* We may need to do some setup of our own at this point before reading
	 * the data.  After jpeg_start_decompress() we have the correct scaled
	 * output image dimensions available, as well as the output colormap
	 * if we asked for color quantization.
	 * In this example, we need to make an output work buffer of the right size.
	 */ 
	/* JSAMPLEs per row in output buffer */
	
	pixelcount = cinfo.output_width * cinfo.output_height;
	
	if(!cinfo.output_width || !cinfo.output_height
	   || ((pixelcount * 4) / cinfo.output_width) / 4 != cinfo.output_height
	   || pixelcount > 0x1FFFFFFF || cinfo.output_components > 4) // 4*1FFFFFFF == 0x7FFFFFFC < 0x7FFFFFFF
	{
		return false;
		//ri.Error (ERR_DROP, "LoadJPG: %s has an invalid image size: %dx%d*4=%d, components: %d\n", filename,
		//		  cinfo.output_width, cinfo.output_height, pixelcount * 4, cinfo.output_components);
	}
	
	memcount = pixelcount * 4;
	row_stride = cinfo.output_width * cinfo.output_components;
	
	out = dest;
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	
	while (cinfo.output_scanline < cinfo.output_height) {
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could ask for
		 * more than one scanline at a time if that's more convenient.
		 */
		buf = ((out+(row_stride*cinfo.output_scanline)));
		buffer = &buf;
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
	}
	
	buf = out;
#if 0	
	// If we are processing an 8-bit JPEG (greyscale), we'll have to convert
	// the greyscale values to RGBA.
	if(cinfo.output_components == 1)
	{
		int sindex = pixelcount, dindex = memcount;
		unsigned char greyshade;
		
		// Only pixelcount number of bytes have been written.
		// Expand the color values over the rest of the buffer, starting
		// from the end.
		do
		{
			greyshade = buf[--sindex];
			
			buf[--dindex] = 255;
			buf[--dindex] = greyshade;
			buf[--dindex] = greyshade;
			buf[--dindex] = greyshade;
		} while(sindex);
	}
	else
	{
		// clear all the alphas to 255
		int	i;
		
		for ( i = 3 ; i < memcount ; i+=4 )
		{
			buf[i] = 255;
		}
	}
#endif
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return true;
}