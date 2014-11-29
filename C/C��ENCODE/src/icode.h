#ifndef _I_CODE_H
#define _I_CODE_H

#define debug_printf	//printf
#define USIZ			3
#define GSIZ			1
#define TABLESIZE		5000

/**********************************************************************
  UTF-8 : UNICODE
  ---------------
U-00000000 - U-0000007F: 0xxxxxxx  
U-00000080 - U-000007FF: 110xxxxx 10xxxxxx  
U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx  
U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
**********************************************************************/

typedef unsigned char 	u8;
typedef unsigned short 	u16;
typedef unsigned int	u32;

typedef struct encode {
	u16	unicode;
	u16	gbk;
} encode_t;

/* 1. utf-8, unicode, gb2312 ---> diff code format */

/* utf-8 code (unicode) parser */
u8 get_utf8_bytes(u8 leading);					/* 3 bytes for a chn */
u16	get_utf8_chn_word(const char *utf8_chn);	/* utf-8 to 2 bytes*/
u16	get_gbk_chn_word(const u16 utf8_chn, 
		encode_t t[], int size);				/* 2bytes utf-8 to gbk */

/* code-converter */
void utf8_to_gb2312(const char *utf8, int len, char *gbk);

/* construct encode map-table (utf8-gbk) */
void build_utf8_gbk_maptable(const char *utf8_file, const char *gbk_file);

void test_utf8_to_gbk(const char *srcfile, const char *dstfile);

#endif

/***
 * TODO:
 * 1. UTF-8 convert to GB2312
 * 2. GB2312 convert to UTF-8
 * 3. Build a small Chinese Font Lib
 * 4. Dynamic search the GB2312
 */
