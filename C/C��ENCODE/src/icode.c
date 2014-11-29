#include "icode.h"
#include "sutable.h"

#include <stdio.h>
#include <string.h>

u8 get_utf8_bytes(u8 leading)
{
	u8 mask = 0x80;
	u8 num = 0;

	while (leading & mask) {
		num++;
		mask >>= 1;
	}

	debug_printf("Leading 1 num : %d\n", num);

	return num;
}

u16 get_utf8_chn_word(const char *utf8_chn)
{
	u16 utf8_word = 0;

	/* little-endian */
	u8 *utf8_high = (u8 *)&utf8_word;
	u8 *utf8_low  = utf8_high + 1;

	if (get_utf8_bytes((u8)utf8_chn[0]) == 3) {
		*utf8_low = ((utf8_chn[0]&0x0F)<< 4)|((utf8_chn[1] >> 2) & 0x0F);
		*utf8_high= ((utf8_chn[1]&0x03) << 6) + (utf8_chn[2] & 0x3F);

		debug_printf("unicode:%04x\n", utf8_word);
	}

	return utf8_word;
}

u16 get_gbk_chn_word(const u16 utf8_chn, encode_t code_table[], int size)
{
	int begin = 0;
	int end	  = size - 1;
	int mid	  = 0;

	debug_printf("SIZE=%d\n", end);

	/* do hash first */

	/* binary search */
	while (begin <= end) {

		mid = (begin + end) / 2;

		if (code_table[mid].unicode == utf8_chn) 
			return code_table[mid].gbk;
		else if (code_table[mid].unicode > utf8_chn)
			end = mid - 1;
		else
			begin = mid + 1;
	}

	return 0;
}

void build_utf8_gbk_maptable(const char *utf8_file, const char *gbk_file)
{
	int cnt = 0;
	FILE *srcfp, *dstfp;
	FILE *outfp;

	/* save table to file */
	outfp = fopen("table.h", "w");
	//outfp = stdout;

	srcfp = fopen(utf8_file, "r");
	if (NULL == srcfp) {
		printf("error: open %s failed.\n", utf8_file);
		return;
	}

	dstfp = fopen(gbk_file, "r");
	if (NULL == dstfp) {
		printf("error: open %s failed.\n", gbk_file);
		return;
	}

	/* read buffer */
	char gbuf[GSIZ], ubuf[USIZ];
	char g2buf[GSIZ+2];

	memset(gbuf, 0, GSIZ);
	memset(ubuf, 0, USIZ);

	while (fread(gbuf, GSIZ, 1, dstfp) > 0) {

		/* skip \n\r _WIN32 platform */
		if (!(gbuf[0] & 0x80)) {
			/* skip utf-8 file too */
			debug_printf("skip 0x%02x.\n", gbuf[0]);

			fread(gbuf, GSIZ, 1, srcfp);
			continue;
		}

		g2buf[0] = gbuf[0];

		/* gbk-code 2bytes a chinese word */
		fread(gbuf, GSIZ, 1, dstfp);
		g2buf[1] = gbuf[0];
		g2buf[2] = 0;

		/* output GBK encode */
		debug_printf("\n%04d: gbk-code: 0x%02x%02x\n", 
				cnt++, (unsigned char)g2buf[0], (unsigned char)g2buf[1]);

		/* read utf-8 file, output UTF encode */
		if (fread(ubuf, USIZ, 1, srcfp) <= 0) {
			printf("error: utf-8 file words leak.\n");
			return;
		}

		debug_printf("utf-code: 0x%02x%02x%02x ",
				(unsigned char)ubuf[0], (unsigned char )ubuf[1], 
				(unsigned char)ubuf[2]);

		u16 unicode = get_utf8_chn_word(ubuf);
		if (unicode == 0) {
			printf("error: unicode converter failed.\n");
			return;
		}

		debug_printf("unicode: 0x%04X ", unicode);

		u16 gbk_byte;
		//memcpy(&gbk_byte, g2buf, 2);
		u8 *gbk_low  = (u8*)&gbk_byte;
		u8 *gbk_high = gbk_low + 1;

		*gbk_low  = g2buf[0];
		*gbk_high = g2buf[1];

		debug_printf("GBK-code: 0x%04X\n", gbk_byte);

		/* output to table.h for use */
		fprintf(outfp, "{0x%04X, 0x%04X},/*%s*/\n", 
				unicode, gbk_byte, g2buf);

		memset(gbuf, 0, GSIZ);
		memset(g2buf, 0, GSIZ+1);
		memset(ubuf, 0, USIZ);

		/* test high-low byte order for u16 
		memcpy(g2buf, &gbk_byte, 2);
		fprintf(outfp, "test-gbk:0x%02x%02x\n", (unsigned char) g2buf[0],
				(unsigned char)g2buf[1]);
		 */
	}

	fclose(srcfp);
	fclose(dstfp);
	fclose(outfp);
}

void utf8_to_gb2312(const char *utf8, int len, char *gbk)
{
	int bytecount = 0;
	int i = 0, j = 0;

	u16 unicode, gbkcode;

	while (i < len) {
		switch(get_utf8_bytes((u8)utf8[i])) {
			case 0:
				gbk[j] = utf8[i]; /* anscii code */
				bytecount = 1;
			break;

			case 2:
				gbk[j] = utf8[i];
				gbk[j+1] = utf8[i+1];
				bytecount = 2;
			break;

			case 3:
				unicode = get_utf8_chn_word(utf8+i);
				gbkcode = get_gbk_chn_word(unicode, code_table, TSIZ);
				debug_printf("gbk:0x%04X\n", gbkcode);

				if (gbkcode != 0) {
					memcpy(gbk+j, &gbkcode, 2);
				}

				bytecount = 3;
			break;

			case 4:
				bytecount = 4;
			break;

			case 5:
				bytecount = 5;
			break;

			case 6:
				bytecount = 6;
			break;

			default:
				debug_printf("Len more than 6\n");
			break;
		}

		i += bytecount;

		if (bytecount == 1) j++;
		else j += 2;
	}
}
