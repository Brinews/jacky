
/*************************************************************************
* Constants used for LZ77 coding
*************************************************************************/

/* Maximum offset (can be any size < 2^31). Lower values give faster
   compression, while higher values gives better compression. The default
   value of 100000 is quite high. Experiment to see what works best for
   you. */
#define LZ_MAX_OFFSET 100000



/*************************************************************************
*                           INTERNAL FUNCTIONS                           *
*************************************************************************/


/*************************************************************************
* _LZ_StringCompare() - Return maximum length string match.
*************************************************************************/

static unsigned int _LZ_StringCompare( unsigned char * str1,
  unsigned char * str2, unsigned int minlen, unsigned int maxlen )
{
    unsigned int len;

    for( len = minlen; (len < maxlen) && (str1[len] == str2[len]); ++ len );

    return len;
}


/*************************************************************************
* _LZ_WriteVarSize() - Write unsigned integer with variable number of
* bytes depending on value.
*************************************************************************/

static int _LZ_WriteVarSize( unsigned int x, unsigned char * buf )
{
    unsigned int y;
    int num_bytes, i, b;

    /* Determine number of bytes needed to store the number x */
    y = x >> 3;
    for( num_bytes = 5; num_bytes >= 2; -- num_bytes )
    {
        if( y & 0xfe000000 ) break;
        y <<= 7;
    }

    /* Write all bytes, seven bits in each, with 8:th bit set for all */
    /* but the last byte. */
    for( i = num_bytes-1; i >= 0; -- i )
    {
        b = (x >> (i*7)) & 0x0000007f;
        if( i > 0 )
        {
            b |= 0x00000080;
        }
        *buf ++ = (unsigned char) b;
    }

    /* Return number of bytes written */
    return num_bytes;
}


/*************************************************************************
* _LZ_ReadVarSize() - Read unsigned integer with variable number of
* bytes depending on value.
*************************************************************************/

static int _LZ_ReadVarSize( unsigned int * x, unsigned char * buf )
{
    unsigned int y, b, num_bytes;

    /* Read complete value (stop when byte contains zero in 8:th bit) */
    y = 0;
    num_bytes = 0;
    do
    {
        b = (unsigned int) (*buf ++);
        y = (y << 7) | (b & 0x0000007f);
        ++ num_bytes;
    }
    while( b & 0x00000080 );

    /* Store value in x */
    *x = y;

    /* Return number of bytes read */
    return num_bytes;
}



/*************************************************************************
*                            PUBLIC FUNCTIONS                            *
*************************************************************************/


/*************************************************************************
* LZ_Compress() - Compress a block of data using an LZ77 coder.
*  in     - Input (uncompressed) buffer.
*  out    - Output (compressed) buffer. This buffer must be 0.4% larger
*           than the input buffer, plus one byte.
*  insize - Number of input bytes.
* The function returns the size of the compressed data.
*************************************************************************/

int LZ_Compress( unsigned char *in, unsigned char *out,
    unsigned int insize )
{
    unsigned char marker, symbol;
    unsigned int  inpos, outpos, bytesleft, i;
    unsigned int  maxoffset, offset, bestoffset;
    unsigned int  maxlength, length, bestlength;
    unsigned int  histogram[ 256 ];
    unsigned char *ptr1, *ptr2;

    /* Do we have anything to compress? */
    if( insize < 1 )
    {
        return 0;
    }

    /* Create histogram */
    for( i = 0; i < 256; ++ i )
    {
        histogram[ i ] = 0;
    }
    for( i = 0; i < insize; ++ i )
    {
        ++ histogram[ in[ i ] ];
    }

    /* Find the least common byte, and use it as the marker symbol */
    marker = 0;
    for( i = 1; i < 256; ++ i )
    {
        if( histogram[ i ] < histogram[ marker ] )
        {
            marker = i;
        }
    }

    /* Remember the marker symbol for the decoder */
    out[ 0 ] = marker;

    /* Start of compression */
    inpos = 0;
    outpos = 1;

    /* Main compression loop */
    bytesleft = insize;
    do
    {
        /* Determine most distant position */
        if( inpos > LZ_MAX_OFFSET ) maxoffset = LZ_MAX_OFFSET;
        else                        maxoffset = inpos;

        /* Get pointer to current position */
        ptr1 = &in[ inpos ];

        /* Search history window for maximum length string match */
        bestlength = 3;
        bestoffset = 0;
        for( offset = 3; offset <= maxoffset; ++ offset )
        {
            /* Get pointer to candidate string */
            ptr2 = &ptr1[ -(int)offset ];

            /* Quickly determine if this is a candidate (for speed) */
            if( (ptr1[ 0 ] == ptr2[ 0 ]) &&
                (ptr1[ bestlength ] == ptr2[ bestlength ]) )
            {
                /* Determine maximum length for this offset */
                maxlength = (bytesleft < offset ? bytesleft : offset);

                /* Count maximum length match at this offset */
                length = _LZ_StringCompare( ptr1, ptr2, 0, maxlength );

                /* Better match than any previous match? */
                if( length > bestlength )
                {
                    bestlength = length;
                    bestoffset = offset;
                }
            }
        }

        /* Was there a good enough match? */
        if( (bestlength >= 8) ||
            ((bestlength == 4) && (bestoffset <= 0x0000007f)) ||
            ((bestlength == 5) && (bestoffset <= 0x00003fff)) ||
            ((bestlength == 6) && (bestoffset <= 0x001fffff)) ||
            ((bestlength == 7) && (bestoffset <= 0x0fffffff)) )
        {
            out[ outpos ++ ] = (unsigned char) marker;
            outpos += _LZ_WriteVarSize( bestlength, &out[ outpos ] );
            outpos += _LZ_WriteVarSize( bestoffset, &out[ outpos ] );
            inpos += bestlength;
            bytesleft -= bestlength;
        }
        else
        {
            /* Output single byte (or two bytes if marker byte) */
            symbol = in[ inpos ++ ];
            out[ outpos ++ ] = symbol;
            if( symbol == marker )
            {
                out[ outpos ++ ] = 0;
            }
            -- bytesleft;
        }
    }
    while( bytesleft > 3 );

    /* Dump remaining bytes, if any */
    while( inpos < insize )
    {
        if( in[ inpos ] == marker )
        {
            out[ outpos ++ ] = marker;
            out[ outpos ++ ] = 0;
        }
        else
        {
            out[ outpos ++ ] = in[ inpos ];
        }
        ++ inpos;
    }

    return outpos;
}


/*************************************************************************
* LZ_CompressFast() - Compress a block of data using an LZ77 coder.
*  in     - Input (uncompressed) buffer.
*  out    - Output (compressed) buffer. This buffer must be 0.4% larger
*           than the input buffer, plus one byte.
*  insize - Number of input bytes.
*  work   - Pointer to a temporary buffer (internal working buffer), which
*           must be able to hold (insize+65536) unsigned integers.
* The function returns the size of the compressed data.
*************************************************************************/

int LZ_CompressFast( unsigned char *in, unsigned char *out,
    unsigned int insize, unsigned int *work )
{
    unsigned char marker, symbol;
    unsigned int  inpos, outpos, bytesleft, i, index, symbols;
    unsigned int  offset, bestoffset;
    unsigned int  maxlength, length, bestlength;
    unsigned int  histogram[ 256 ], *lastindex, *jumptable;
    unsigned char *ptr1, *ptr2;

    /* Do we have anything to compress? */
    if( insize < 1 )
    {
        return 0;
    }

    /* Assign arrays to the working area */
    lastindex = work;
    jumptable = &work[ 65536 ];

    /* Build a "jump table". Here is how the jump table works:
       jumptable[i] points to the nearest previous occurrence of the same
       symbol pair as in[i]:in[i+1], so in[i] == in[jumptable[i]] and
       in[i+1] == in[jumptable[i]+1], and so on... Following the jump table
       gives a dramatic boost for the string search'n'match loop compared
       to doing a brute force search. The jump table is built in O(n) time,
       so it is a cheap operation in terms of time, but it is expensice in
       terms of memory consumption. */
    for( i = 0; i < 65536; ++ i )
    {
        lastindex[ i ] = 0xffffffff;
    }
    for( i = 0; i < insize-1; ++ i )
    {
        symbols = (((unsigned int)in[i]) << 8) | ((unsigned int)in[i+1]);
        index = lastindex[ symbols ];
        lastindex[ symbols ] = i;
        jumptable[ i ] = index;
    }
    jumptable[ insize-1 ] = 0xffffffff;

    /* Create histogram */
    for( i = 0; i < 256; ++ i )
    {
        histogram[ i ] = 0;
    }
    for( i = 0; i < insize; ++ i )
    {
        ++ histogram[ in[ i ] ];
    }

    /* Find the least common byte, and use it as the marker symbol */
    marker = 0;
    for( i = 1; i < 256; ++ i )
    {
        if( histogram[ i ] < histogram[ marker ] )
        {
            marker = i;
        }
    }

    /* Remember the marker symbol for the decoder */
    out[ 0 ] = marker;

    /* Start of compression */
    inpos = 0;
    outpos = 1;

    /* Main compression loop */
    bytesleft = insize;
    do
    {
        /* Get pointer to current position */
        ptr1 = &in[ inpos ];

        /* Search history window for maximum length string match */
        bestlength = 3;
        bestoffset = 0;
        index = jumptable[ inpos ];
        while( (index != 0xffffffff) && ((inpos - index) < LZ_MAX_OFFSET) )
        {
            /* Get pointer to candidate string */
            ptr2 = &in[ index ];

            /* Quickly determine if this is a candidate (for speed) */
            if( ptr2[ bestlength ] == ptr1[ bestlength ] )
            {
                /* Determine maximum length for this offset */
                offset = inpos - index;
                maxlength = (bytesleft < offset ? bytesleft : offset);

                /* Count maximum length match at this offset */
                length = _LZ_StringCompare( ptr1, ptr2, 2, maxlength );

                /* Better match than any previous match? */
                if( length > bestlength )
                {
                    bestlength = length;
                    bestoffset = offset;
                }
            }

            /* Get next possible index from jump table */
            index = jumptable[ index ];
        }

        /* Was there a good enough match? */
        if( (bestlength >= 8) ||
            ((bestlength == 4) && (bestoffset <= 0x0000007f)) ||
            ((bestlength == 5) && (bestoffset <= 0x00003fff)) ||
            ((bestlength == 6) && (bestoffset <= 0x001fffff)) ||
            ((bestlength == 7) && (bestoffset <= 0x0fffffff)) )
        {
            out[ outpos ++ ] = (unsigned char) marker;
            outpos += _LZ_WriteVarSize( bestlength, &out[ outpos ] );
            outpos += _LZ_WriteVarSize( bestoffset, &out[ outpos ] );
            inpos += bestlength;
            bytesleft -= bestlength;
        }
        else
        {
            /* Output single byte (or two bytes if marker byte) */
            symbol = in[ inpos ++ ];
            out[ outpos ++ ] = symbol;
            if( symbol == marker )
            {
                out[ outpos ++ ] = 0;
            }
            -- bytesleft;
        }
    }
    while( bytesleft > 3 );

    /* Dump remaining bytes, if any */
    while( inpos < insize )
    {
        if( in[ inpos ] == marker )
        {
            out[ outpos ++ ] = marker;
            out[ outpos ++ ] = 0;
        }
        else
        {
            out[ outpos ++ ] = in[ inpos ];
        }
        ++ inpos;
    }

    return outpos;
}


/*************************************************************************
* LZ_Uncompress() - Uncompress a block of data using an LZ77 decoder.
*  in      - Input (compressed) buffer.
*  out     - Output (uncompressed) buffer. This buffer must be large
*            enough to hold the uncompressed data.
*  insize  - Number of input bytes.
*************************************************************************/

void LZ_Uncompress( unsigned char *in, unsigned char *out,
    unsigned int insize )
{
    unsigned char marker, symbol;
    unsigned int  i, inpos, outpos, length, offset;

    /* Do we have anything to uncompress? */
    if( insize < 1 )
    {
        return;
    }

    /* Get marker symbol from input stream */
    marker = in[ 0 ];
    inpos = 1;

    /* Main decompression loop */
    outpos = 0;
    do
    {
        symbol = in[ inpos ++ ];
        if( symbol == marker )
        {
            /* We had a marker byte */
            if( in[ inpos ] == 0 )
            {
                /* It was a single occurrence of the marker byte */
                out[ outpos ++ ] = marker;
                ++ inpos;
            }
            else
            {
                /* Extract true length and offset */
                inpos += _LZ_ReadVarSize( &length, &in[ inpos ] );
                inpos += _LZ_ReadVarSize( &offset, &in[ inpos ] );

                /* Copy corresponding data from history window */
                for( i = 0; i < length; ++ i )
                {
                    out[ outpos ] = out[ outpos - offset ];
                    ++ outpos;
                }
            }
        }
        else
        {
            /* No marker, plain copy */
            out[ outpos ++ ] = symbol;
        }
    }
    while( inpos < insize );
}
