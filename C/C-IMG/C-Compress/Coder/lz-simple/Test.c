#include <stdio.h>
#include <stdlib.h>

#include "lz.h"

int main(int argc, char **argv)
{
    FILE *fp = fopen("test.jpg", "rb");
    unsigned char *image_data;
    int total;

    if (fp == NULL) {
        printf("Open file failed.\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    total = ftell(fp);

    rewind(fp);
    printf("filesize=%d\n", total);

    image_data = (unsigned char *)malloc (total *sizeof(unsigned char));
    if (image_data == NULL) {
        printf("Malloc failed.\n");
        return -1;
    }

    fread(image_data, sizeof(unsigned char), total, fp);

    unsigned char *outbuf = (unsigned char *)malloc(total*2*sizeof(unsigned char));
    if (outbuf == NULL) {
        printf("Malloc failed.\n");
        return -1;
    }

    int outlen = LZ_Compress(image_data, outbuf, total);
    printf("outlen=%d\n", outlen);

    FILE *fout = fopen("mid.jpg", "wb+");
    if (fout == NULL) {
        printf("open file failed.\n");
        return -1;
    }

    int i = 0;
    while (i < outlen) {
        fputc(outbuf[i], fout);
        i++;
    }

    /* decompress
     */
    FILE *fdep = fopen("out-lz.jpg", "wb+");
    if (fdep == NULL) {
        printf("open failed.\n");
        return -1;
    }

    unsigned char *imgbuf = (unsigned char *) malloc(total*2*sizeof(unsigned char));
    if (imgbuf == NULL) {
        printf("malloc error\n");
        return -1;
    }

    LZ_Uncompress(outbuf, imgbuf, outlen);

    i = 0;
    while (i < total) {
        fputc(imgbuf[i], fdep);
        i++;
    }

    free(image_data);
    free(outbuf);
    free(imgbuf);

    fclose(fp);
    fclose(fout);
    fclose(fdep);

    return 0;
}
