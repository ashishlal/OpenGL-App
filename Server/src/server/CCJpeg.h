#include <stdio.h>
#include "Interfaces.h"

#ifdef __cplusplus
extern "C" {
#include <jpeglib.h>
#include <jerror.h>
}
#endif
namespace Jpeg {

#ifdef __cplusplus
	extern "C" {
		typedef struct {
			struct jpeg_destination_mgr pub;
			JOCTET *buf;
			size_t bufsize;
			size_t jpegsize;
		} mem_destination_mgr;
		typedef mem_destination_mgr *mem_dest_ptr;
	}
#endif


	class CCJpeg: public ICCJpeg {
	public:
		CCJpeg() {}
		~CCJpeg() {}
		void JpegCompressPixelData(void *arg);
	private:
		boolean jpeg_compress(unsigned char *buffer, unsigned char *outbuffer, unsigned long outlen, int *jpegLength,
				      unsigned int *ow, unsigned int *oh, unsigned int buf_win_width, unsigned int buf_win_height);
		boolean jpeg_compress_in_file(unsigned char *buffer, unsigned char *outbuffer, unsigned long outlen, int *jpegLength,
					      unsigned int buf_win_width, unsigned int buf_win_height);
		static void jpeg_mem_dest(j_compress_ptr cinfo, JOCTET* buf, size_t bufsize);
		static int jpeg_mem_size(j_compress_ptr cinfo);
		int myargc;
		char *myargv[32];
	};

}