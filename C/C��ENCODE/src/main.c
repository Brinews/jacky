/* ls --show-control-chars */

#include "icode.h"

#include <stdio.h>
#include <string.h>

void test_utf8_to_gbk(const char *srcfile, const char *dstfile)
{
    char buf[65535]; /* max 64kb file, can optimize to infinite size */
    char code[65535] = {0};

    FILE *srcfp, *dstfp;
    srcfp = fopen(srcfile, "r");
    dstfp = fopen(dstfile, "w");

    int len = fread(buf, 1, sizeof(buf), srcfp);

    debug_printf("file len: %d\n", len);

    utf8_to_gb2312(buf, len, code);

    fwrite(code, strlen(code), 1, dstfp);

    fclose(srcfp);
    fclose(dstfp);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: [-b|-u|-g] \n");
        printf("-b <utf-8 file> <gbk file>  : build encoding table\n");
        printf("-u <utf-8 file>             : change utf-8 to gbk\n");
        printf("-g <gbk file>               : change gbk to utf-8\n");
        return 0;
    }

    if (strcmp(argv[1], "-b") == 0) {
        if (argc < 4) {
            return 0;
        }
        build_utf8_gbk_maptable(argv[2], argv[3]);
    }

    if (strcmp(argv[1], "-u") == 0) {
        test_utf8_to_gbk(argv[2], "gbk_result.txt");
    }

    if (strcmp(argv[1], "-g") == 0) {
    }

    return 0;
}
