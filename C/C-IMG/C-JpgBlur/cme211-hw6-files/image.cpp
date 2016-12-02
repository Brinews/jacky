/*
 * Author: xxx
 * Date: 2016-12-1
 * Brief: jpeg image blur
 */
#include "image.hpp"
#include "hw6.hpp"

#include <fstream>
#include <iostream>

/*
 * constructor:
 *  read Jpeg file
 */
Image::Image(string name)
{
    filename = name;

    ReadGrayscaleJPEG(filename, img);

    height = img.shape()[0];
    width = img.shape()[1];

    out.resize(boost::extents[height][width]);
    out1.resize(boost::extents[height][width]);

}

Image::~Image()
{
}

/*
 * save jpeg file after blur
 */
void Image::Save(string savename)
{
    if (savename == "") savename = filename;
    WriteGrayscaleJPEG(savename, out);
}

void Image::BoxBlur(int kernelsize)
{
    boost::multi_array<float,2> kernel;
    kernel.resize(boost::extents[kernelsize][kernelsize]);

    for (int i = 0; i < kernelsize; i++) {
        for (int j = 0; j < kernelsize; j++) {
            kernel[i][j] = 1.0;
        }
    }

    Convolution(img, out, kernel); // blur
}

unsigned int Image::Sharpness()
{
    int kernelsize = 3;
    boost::multi_array<float, 2> kernel;
    kernel.resize(boost::extents[kernelsize][kernelsize]);
    
	//printf("sharpness\n");
    for (int i = 0; i < kernelsize; i++) {
        for (int j = 0; j < kernelsize; j++) {
            kernel[i][j] = 0.0;
            if (i == 1 || j == 1) {
                kernel[i][j] = 1.0;
            }
            if (i == 1 && j == 1) kernel[i][j] = -4.0;

			//printf("%f ", kernel[i][j]);
        }
		//printf("\n");
    }

    Convolution(out, out1, kernel);

    unsigned int ret = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (ret < out1[i][j]) {
                ret = out1[i][j];
            }
        }
    }

    return ret;
}

/*
 * convolution
 */
void Convolution(boost::multi_array<unsigned char,2>& input,
        boost::multi_array<unsigned char,2>& output,
        boost::multi_array<float,2>& kernel)
{
	//printf("--------------In Convo------------------\n");

    int kernelsize = kernel.shape()[0];
	int height = input.shape()[0];
	int width = input.shape()[1];

    if (input.shape()[0] != output.shape()[0] ||
            input.shape()[1] != output.shape()[1]) {
        std::cout << "input and output should have the same size\n";
        exit(0);
    }

    if ( kernel.shape()[0] != kernel.shape()[1] ||
            kernelsize < 3 || kernelsize%2 == 0) {
        std::cout << "kernel shape should has more than 3 odd square size\n";
        exit(0);
    }

	int count = 0;// kernelsize*kernelsize;
	for (int i = 0; i < kernelsize; i++) {
		for (int j = 0; j < kernelsize; j++) {
			if (kernel[i][j] == 1) count++;
		}
	}
	//printf("count=%d\n", count);


    unsigned char *src = input.data();
    unsigned char *dst = output.data();

	//printf("H=%d,W=%d\n", height, width);
	//printf("ksize=%d\n", kernelsize);

    unsigned char *from = (unsigned char *) malloc(kernelsize*kernelsize);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            int r = 0, c = 0;
            for (int row = -kernelsize/2; row <= kernelsize/2; row++) {
                c = 0;
                for (int col = -kernelsize/2; col <= kernelsize/2; col++) {
                    int pr = row + i;
                    int pc = col + j;
                    if (pr < 0 && pc < 0) { // top left
                        from[r*kernelsize+c] = src[0];
                    } else if (pr < 0 && pc >= width) { // top right
                        from[r*kernelsize+c] = src[width-1];
                    } else if (pr >= height && pc < 0) { // bot left
                        from[r*kernelsize+c] = src[(height-1)*width];
                    } else if (pr >= height && pc >= width) { // bot right
                        from[r*kernelsize+c] = src[height*width-1];
                    } else if (pr < 0) { // top
                        from[r*kernelsize+c] = src[pc];
                    } else if (pr >= 0 && pr < height && pc < 0) { // left
                        from[r*kernelsize+c] = src[pr*width];
                    } else if (pr >= 0 && pr < height && pc >= width) { // right
                        from[r*kernelsize+c] = src[pr*width-1];
                    } else if (pr >= height) { // bot
                        from[r*kernelsize+c] = src[width*(height-1)+pc];
                    } else {
						from[r*kernelsize+c] = src[pr*width+pc];
					}

					/*
					if (i == 0 && j == 0) {
						printf("<%d,%d><%d,%d>=%d\n", pr, pc, r, c, from[r*kernelsize+c]);
					}
					*/

                    c++;
                }
                r++;
            }

			/* for debug */
			/*
			if (i == 0 && j == 0) {
				printf("from\n");
				for(int m = 0; m < kernelsize; m++) {
					for (int n = 0; n < kernelsize; n++) {
						printf("%d, ", from[m*kernelsize+n]);
					}
					printf("\n");
				}
			}
			*/

            float val = 0.0;
            unsigned char *pDst = dst + width*i + j;

            for (int row = 0; row < kernelsize; row++) {
                for (int col = 0; col < kernelsize; col++) {
                    val += from[row*kernelsize+col]*kernel[row][col];
                }
            }

            if (val < 0) val = 0; // omit negative value

			*pDst = (unsigned char)(val/count+0.5);

			/*
			if (i == 0 && j == 0) {
				printf("val=%f\n", val);
				printf("dst=%d, %d\n", *pDst, output[i][j]);
			}
			*/
        }
    }
    
    free(from);
}
