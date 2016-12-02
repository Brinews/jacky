#include <stdio.h>

//#include <boost/regex.hpp>

extern "C" {  
    #include "jpeglib.h"  
}  
  
//#pragma comment(lib, "libjpeg.lib")  

//#include "jpeglib.h"
//#pragma comment(lib,"libjpeg.lib")
//#pragma warning(disable:4996)

typedef unsigned char BYTE;

int main(void)
{
	//boost::regex reg("\\d{3}([a-zA-Z]+).(\\d{2}|N/A)\\s\\1");   

	printf("Input jpeg file path:");
	char szFileName[256] = { 0 };
	scanf("%s", szFileName);

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	// STEP 1: StdError
	cinfo.err = jpeg_std_error(&jerr);

	// STEP 2: Create
	jpeg_create_decompress(&cinfo);

	FILE* pf = fopen(szFileName, "rb");
	if (pf != NULL)
	{
		// STEP 3: IO
		jpeg_stdio_src(&cinfo, pf);

		// STEP 4: Header
		jpeg_read_header(&cinfo, TRUE);
		
		long bytes = cinfo.image_width*cinfo.image_height*cinfo.num_components;
		printf("Allocate %d bytes memory:",bytes);
		BYTE* data = new BYTE[bytes];
		if (data != NULL)
		{
			printf("OK.\nPrepare to decompress the image...\n");
			
			// STEP 5: Start
			jpeg_start_decompress(&cinfo);
			JSAMPROW row_pointer[1];

			// STEP 6: ReadScan
			while (cinfo.output_scanline < cinfo.output_height)
			{
				row_pointer[0] = &data[(cinfo.output_height - cinfo.output_scanline - 1)*cinfo.image_width*cinfo.num_components];
				jpeg_read_scanlines(&cinfo, row_pointer, 1);
			}

			// STEP 7: Finish
			jpeg_finish_decompress(&cinfo);
			// Do something with
			// BYTE data[] here
			// and then release it
			delete[] data;
		}
		else
		{
			printf("FAILED.\n");
		}

		// STEP 8: Destroy
		jpeg_destroy_decompress(&cinfo);
		fclose(pf);

		printf("JPEG decompression finished.\n");
	}
	else
	{
		printf("Failed to open \'%s\'\n", szFileName);
	}

	printf("Press ENTER to continue...");
	getchar();
	getchar();
	return 0;
}