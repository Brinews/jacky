/*
 * Date: 2015-10-09
 * Author: CoderLi
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include <zlib.h>

/*
 * function prototype
 */
void cli_usage(char *name);
uLong cli_get_crc32(const char *filename);
void cli_file_check(const char *filename, const char *basename, FILE *fp);
void cli_dir_recursive(const char *dirname, const char *basename, FILE *fp);


/*
 * @param name program's name
 */
void cli_usage(char *name)
{
    printf("Usage:%s <dir> <filename>\n", name);
    printf("Compute CRC-32 checksum for each file in the given directory.\n");
}

/*
 * @param filename the file need to calculate CRC32
 * @return CRC32-checksum
 */
uLong cli_get_crc32(const char *filename)
{
    uLong crc = crc32(0L, Z_NULL, 0);
    FILE *fp = fopen(filename, "rb");

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    char *buf = (char *) malloc(size*sizeof(char));

    if (buf == NULL) {
        fprintf(stderr, "malloc for buf failed.");
    } else {
        fseek(fp, 0L, SEEK_SET);

        if (fread(buf, size, 1, fp) != 1) {
            fprintf(stderr, "fread %s failed.\n", filename);
        }

        crc = crc32(crc, (Bytef *)buf, size);

        free(buf);

        return crc;
    }

    return crc;
}

/*
 * @param filename the file need to get checksum
 * @param fp file handle to record the checksum
 */
void cli_file_check(const char *filename, const char *basename, FILE *fp)
{
    const char *ptr = filename;
    int len = strlen(basename);

    while (len--) ptr++;
    ptr++;

    uLong crc32 = cli_get_crc32(filename);



    fprintf(fp, "%s,%lu\n", ptr, crc32);
}

/*
 * @param dirname the given directory need to traverse recursive
 * @param fp file handle to record the checksum
 */
void cli_dir_recursive(const char *dirname, const char *basename, FILE *fp)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    struct stat info;

    char pathname[1024];

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "cannot open %s\n", dirname);
    } else {
        while ((direntp = readdir(dir_ptr)) != NULL) {

            if (strcmp(direntp->d_name, ".") == 0 || 
                    strcmp(direntp->d_name, "..") == 0)
                continue;

            sprintf(pathname, "%s/%s", dirname, direntp->d_name);

            if (stat(pathname, &info) == -1) {
                fprintf(stderr, "stat %s error\n", pathname);
            } else { 

                if (S_ISDIR(info.st_mode)) {
                    /* if dir, recursive dispose */
                    cli_dir_recursive(pathname, basename, fp);
                } else {
                    /* if file, get checksum */
                    cli_file_check(pathname, basename, fp);
                }
            }
        }

        closedir(dir_ptr);
    }
}

int main(int argc, char **argv)
{
    char *dirname, *recordfilename;
    
    if (argc < 2) {
        cli_usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    dirname = argv[1];
    recordfilename = argv[2];

    FILE *fp = fopen(recordfilename, "w+");
    if (fp == NULL) {
        fprintf(stderr, "open file %s failed.", recordfilename);
        return EXIT_FAILURE;
    }

    cli_dir_recursive(dirname, dirname, fp);

    fclose(fp);
    
    return EXIT_SUCCESS;
}
