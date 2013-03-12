#include "CCJpeg.h"
#include <iostream>

#ifdef WIN32
#include <stdlib.h>
#include <cstring>
#endif

#ifdef LINUX
#include <stdlib.h>
#include <cstring>
#endif

extern int window_width; // The width of our window
extern int window_height; // The height of our window
extern int resized_window_width;
extern int resized_window_height;
//#define TEST_JPEG


namespace Jpeg {

mem_dest_ptr dest;

/*********************************************************************************************************
 **		
 ** Function: 	CreateJpeg
 ** Returns:	ICCJpeg *
 ** Parameters:	None
 ** Notes: Creates a JPEG interface ICCJpeg. Returns a pointer to interface created.
 **
 *********************************************************************************************************/

ICCJpeg *CreateJpeg() 
{
	return new CCJpeg();
}

/*********************************************************************************************************
 **		
 ** Function: 	JpegCompressPixelData
 ** Returns:	void
 ** Parameters:	void *threadArg
 ** Notes: This is the public funciton of ICCJpeg called from the OpenGL thread. It takes an argument
 ** that contains pixel data buffers, input, output and window width and height.
 **
 *********************************************************************************************************/
void CCJpeg::JpegCompressPixelData(void *threadArg)
{
	JpegThreadArg *arg = (JpegThreadArg *)threadArg;
#ifndef TEST_JPEG
	jpeg_compress((unsigned char *)(arg->inBuffer), (unsigned char *)(arg->outBuffer), 
		(unsigned long)(arg->outLen), &(arg->jpegLength),
		&(arg->out_width), &(arg->out_height), arg->window_width, arg->window_height);
#else
	jpeg_compress_in_file((unsigned char *)(arg->inBuffer), (unsigned char *)(arg->outBuffer), 
		(unsigned long)(arg->outLen), &(arg->jpegLength), arg->window_width, arg->window_height);
#endif
}

/*********************************************************************************************************
 **		
 ** Function: 	init_destination
 ** Returns:	void
 ** Parameters:	j_compress_ptr cinfo
 ** Notes: This function initializes the mem_dest_ptr to be used in jpeg compression
 **
 *********************************************************************************************************/

void init_destination(j_compress_ptr cinfo)
{
        dest = (mem_dest_ptr) cinfo->dest;

        dest->pub.next_output_byte = dest->buf;
        dest->pub.free_in_buffer = dest->bufsize;
        dest->jpegsize = 0;
}

/*********************************************************************************************************
 **		
 ** Function: 	empty_output_buffer
 ** Returns:	boolean
 ** Parameters:	j_compress_ptr cinfo
 ** Notes: This function resets the values of mem_dest_ptr and returns false always.
 **
 *********************************************************************************************************/
boolean empty_output_buffer(j_compress_ptr cinfo)
{
        dest = (mem_dest_ptr) cinfo->dest;

        dest->pub.next_output_byte = dest->buf;
        dest->pub.free_in_buffer = dest->bufsize;

        return FALSE;
        ERREXIT(cinfo, JERR_BUFFER_SIZE);
}

/*********************************************************************************************************
 **		
 ** Function: 	term_destination
 ** Returns:	void
 ** Parameters:	j_compress_ptr cinfo
 ** Notes: This function sets the size of the jpeg compression in memory
 **
 *********************************************************************************************************/
void term_destination(j_compress_ptr cinfo)
{
        dest = (mem_dest_ptr) cinfo->dest;
        dest->jpegsize = dest->bufsize - dest->pub.free_in_buffer;
}


/*********************************************************************************************************
 **		
 ** Function: 	jpeg_compress
 ** Returns:	boolean, true on success, false on failure
 ** Parameters:	unsigned char *buffer: input buffer with raw pixel data
 ** 		unsigned char *outbuffer: output buffer
 ** 		unsigned long outlen: output buffer length
 ** 		int *jpegLength: length in bytes of jpeg compressed memory
 ** 		unsigned int buf_win_width: window width with input pixel buffer was read
 ** 		unsigned int buf_win_height: window height when input pixel buffer was read
 ** Notes: This function compresses the raw pixel data in memory using the givn parameters.
 **
 *********************************************************************************************************/
boolean CCJpeg::jpeg_compress(unsigned char *buffer, unsigned char *outbuffer, 
	unsigned long outlen, int *jpegLength, unsigned int *ow, unsigned int *oh,
	unsigned int buf_win_width, unsigned int buf_win_height)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	cinfo.X_density = 1;
	cinfo.Y_density = 1;
	jpeg_mem_dest (&cinfo,outbuffer, (size_t)outlen );
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	/*set the quality [0..100]  */
	jpeg_set_quality (&cinfo, 75, true);
	
	cinfo.image_width      = buf_win_width;
	cinfo.image_height     = buf_win_height;
	//cinfo.image_width      = 256;
	//cinfo.image_height     = 256;
	
	jpeg_start_compress(&cinfo, true);
#if 1
	for(unsigned int h=0; h < cinfo.image_height/2; h++) {
	
		unsigned char *outBuf = (buffer + (h * (cinfo.image_width *4)));
		unsigned char *inBuf = (buffer + (cinfo.image_height-1-h)*(cinfo.image_width *4));
		
		for(unsigned int w=0; w < cinfo.image_width; w++) {
			
			unsigned char temp;
			temp = *outBuf;
			*outBuf = *inBuf;
			outBuf++;
			*inBuf = temp;
			inBuf++;
			
			temp = *outBuf;
			*outBuf = *inBuf;
			outBuf++;
			*inBuf = temp;
			inBuf++;
			
			temp = *outBuf;
			*outBuf = *inBuf;
			outBuf++;
			*inBuf = temp;
			inBuf++;
			
			temp = *outBuf;
			*outBuf = *inBuf;
			outBuf++;
			*inBuf = temp;
			inBuf++;
		}
	}
#endif
	JSAMPLE *image_buffer_pixel_data= (JSAMPLE *)buffer;

	JSAMPLE* image_buffer_row,*orig_ibr;
	image_buffer_row = (unsigned char *)malloc(cinfo.image_width*4*sizeof(unsigned char));
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		
		memset(image_buffer_row, 0, cinfo.image_width*4*sizeof(unsigned char));
		if (image_buffer_row == 0) 
		{ 
			return false;
		}
		orig_ibr = image_buffer_row;
		for (unsigned int x=0 ; x < cinfo.image_width ; x++)
		{ 
			*orig_ibr = *image_buffer_pixel_data;
			orig_ibr++; 
			image_buffer_pixel_data++;
			*orig_ibr = *image_buffer_pixel_data;
			orig_ibr++; 
			image_buffer_pixel_data++;
			*orig_ibr = *image_buffer_pixel_data;
			orig_ibr++;
			image_buffer_pixel_data++;
			image_buffer_pixel_data++;			//skip low order byte.
			
		}
		
		(void) jpeg_write_scanlines(&cinfo, &image_buffer_row, 1);
		
		
	}
	free(image_buffer_row);
	//Finally, call finish 
	jpeg_finish_compress(&cinfo);
	*jpegLength = jpeg_mem_size(&cinfo);
	
	jpeg_destroy_compress(&cinfo);
	
	return true;
}

/*********************************************************************************************************
 **		
 ** Function: 	jpeg_compress_in_file
 ** Returns:	boolean, true on success, false on failure
 ** Parameters:	unsigned char *buffer: input buffer with raw pixel data
 ** 		unsigned char *outbuffer: output buffer
 ** 		unsigned long outlen: output buffer length
 ** 		int *jpegLength: length in bytes of jpeg compressed memory
 ** 		unsigned int buf_win_width: window width with input pixel buffer was read
 ** 		unsigned int buf_win_height: window height when input pixel buffer was read
 ** Notes: This function compresses the raw pixel data using the given parameters and outputs a given file.
 **
 *********************************************************************************************************/
boolean CCJpeg::jpeg_compress_in_file(unsigned char *inBuffer, unsigned char *outbuffer, 
	unsigned long outlen, int *jpegLength, unsigned int buf_win_width, unsigned int buf_win_height)
{
	static int i=0;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
 	
 	i++;
 	if(i >= 10)
 		return false;
 	FILE* outfile = fopen("/Users/Ashish/test/compress.jpeg", "wb");
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
	//jpeg_mem_dest (&cinfo,outbuffer, (size_t)outlen );
 	jpeg_stdio_dest(&cinfo, outfile);
 	
	cinfo.image_width      = window_width;
	cinfo.image_height     = window_height;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;
	
	jpeg_set_defaults(&cinfo);
	/*set the quality [0..100]  */
	jpeg_set_quality (&cinfo, 75, true);
	jpeg_start_compress(&cinfo, true);
#ifdef TEST_CODE	
	JSAMPLE * row_pointer;          /* pointer to a single row */
 	int row_stride = window_width * 3;	/* JSAMPLEs per row in image_buffer */
	while (cinfo.next_scanline < cinfo.image_height) {
		//row_pointer = (JSAMPROW)& buffer[((cinfo.image_height-1)*row_stride)-cinfo.next_scanline * row_stride];
		row_pointer = (JSAMPLE *) &inBuffer[cinfo.next_scanline* row_stride];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}
#else
	JSAMPLE* image_buffer_row,*orig_ibr;
	JSAMPLE *image_buffer_bad= (JSAMPLE *)inBuffer;
	int row_stride = window_width * 4;	/* JSAMPLEs per row in image_buffer */
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		image_buffer_row = (unsigned char *)malloc(row_stride*sizeof(unsigned char));
		if (image_buffer_row == 0) 
		{ 
			return false;
		}
		orig_ibr = image_buffer_row;
		
		for (int x=0 ; x < window_width ; x++)
		{ 
			*orig_ibr = *image_buffer_bad;
			orig_ibr++; 
			image_buffer_bad++;
			*orig_ibr = *image_buffer_bad;
			orig_ibr++; 
			image_buffer_bad++;
			*orig_ibr = *image_buffer_bad;
			orig_ibr++; 
			image_buffer_bad++;
			image_buffer_bad++;			//skip low order byte.
		}
		
		(void) jpeg_write_scanlines(&cinfo, &image_buffer_row, 1);
		free(image_buffer_row);
		
	}
#endif
	
	
	//Finally, call finish 
	jpeg_finish_compress(&cinfo);
	*jpegLength = jpeg_mem_size(&cinfo);
	std::cout << "jpegLength is " << (long unsigned int)(*jpegLength) << std::endl;
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	return true; 
}

/*********************************************************************************************************
 **		
 ** Function: 	jpeg_mem_dest
 ** Returns:	void
 ** Parameters:	j_compress_ptr cinfo: compression information
 ** 		JOCTET* buf:output buffer
 ** 		size_t bufsize: output buffer size
 ** Notes: This function allocates inits the memory pointer for in memory compression.
 *********************************************************************************************************/
void CCJpeg::jpeg_mem_dest(j_compress_ptr cinfo, JOCTET* buf, size_t bufsize)
{
        

        if (cinfo->dest == NULL) {
                cinfo->dest = (struct jpeg_destination_mgr *)
                    (*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
                    sizeof(mem_destination_mgr));
        }

        dest = (mem_dest_ptr) cinfo->dest;

        dest->pub.init_destination    = init_destination;
        dest->pub.empty_output_buffer = empty_output_buffer;
        dest->pub.term_destination    = term_destination;

        dest->buf      = buf;
        dest->bufsize  = bufsize;
        dest->jpegsize = 0;
}

/*********************************************************************************************************
 **		
 ** Function: 	jpeg_mem_size
 ** Returns:	int, size of the compressed memory
 ** Parameters:	j_compress_ptr cinfo: compression information
 ** Notes: This function returns the size of compressed memory
 *********************************************************************************************************/
int CCJpeg::jpeg_mem_size(j_compress_ptr cinfo)
{
         dest = (mem_dest_ptr) cinfo->dest;
        return dest->jpegsize;
}
};
