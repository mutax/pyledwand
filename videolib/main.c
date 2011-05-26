#include <stdio.h>
#include <stdint.h>

#define LINES 20
#define LINELEN 56
#define MODWIDTH 8
#define MODHEIGHT 10
#define BIAS 5

int Image_to_Ledbuffer(const char* imagebuf, const uint32_t len, char* ledbuffer)
{
    if(len != LINES*LINELEN*MODWIDTH*MODWIDTH)
    {
        printf("size of imagebuf hould be %d but is %d\n", LINELEN*LINES*MODWIDTH*MODWIDTH, len);
        return 1;
    }
    uint32_t i = 0, pos = 0, step = LINELEN*MODWIDTH*(MODWIDTH-1);
    for(i = 0; i<LINELEN*MODWIDTH*LINES; i++)
    {
        pos = ((uint32_t)(i/(LINELEN*MODWIDTH)))*step;
        ledbuffer[i] =  (imagebuf[i+pos] & 0x80) |
                        (imagebuf[i+pos+LINELEN*MODWIDTH] & 0x40) |
                        (imagebuf[i+pos+2*LINELEN*MODWIDTH] & 0x20) |
                        (imagebuf[i+pos+3*LINELEN*MODWIDTH] & 0x10) |
                        (imagebuf[i+pos+4*LINELEN*MODWIDTH] & 0x8) |
                        (imagebuf[i+pos+5*LINELEN*MODWIDTH] & 0x4) |
                        (imagebuf[i+pos+6*LINELEN*MODWIDTH] & 0x2) |
                        (imagebuf[i+pos+7*LINELEN*MODWIDTH] & 0x1);
    }
    return 0;
}

int Regular_to_Ledbuffer(const char* imagebuf, const uint32_t len, char* ledbuffer)
{
    if(len != LINES*LINELEN*MODWIDTH*MODHEIGHT)
    {
        printf("size of imagebuf should be %d but is %d\n", LINELEN*LINES*MODWIDTH*MODHEIGHT, len);
        return 1;
    }
    uint32_t i = 0, pos = 0;
    for(i = 0; i<LINELEN*MODWIDTH*LINES; i++)
    {
        pos = (i%(LINELEN*MODWIDTH))+(((uint32_t)(i/(MODWIDTH*LINELEN)))*MODWIDTH*LINELEN*MODHEIGHT);
        ledbuffer[i] =  (imagebuf[pos] & 0x80) |
                        ((imagebuf[pos+LINELEN*MODWIDTH]>>1) & 0x40) |
                        ((imagebuf[pos+2*LINELEN*MODWIDTH]>>2) & 0x20) |
                        ((imagebuf[pos+3*LINELEN*MODWIDTH]>>3) & 0x10) |
                        ((imagebuf[pos+4*LINELEN*MODWIDTH]>>4) & 0x8) |
                        ((imagebuf[pos+5*LINELEN*MODWIDTH]>>5) & 0x4) |
                        ((imagebuf[pos+6*LINELEN*MODWIDTH]>>6) & 0x2) |
                        ((imagebuf[pos+7*LINELEN*MODWIDTH]>>7) & 0x1);
        }
    return 0;
}

int Regular_to_Ledbuffer2(const char* imagebuf, uint32_t len, char* ledbuffer)
{
    if(len != LINES*LINELEN*MODWIDTH*MODWIDTH)
    {
        printf("size of imagebuf should be %d but is %d\n", LINELEN*LINES*MODWIDTH*MODWIDTH, len);
        return 1;
    }
    uint32_t i = 0, pos = 0, middle = 0;
    for(i = 0; i<LINELEN*MODWIDTH*LINES; i++)
    {
        pos = (i%(LINELEN*MODWIDTH))+(((uint32_t)(i/(MODWIDTH*LINELEN)))*MODWIDTH*LINELEN*MODWIDTH);
        middle = (imagebuf[pos]+imagebuf[pos+LINELEN*MODWIDTH] + imagebuf[pos+2*LINELEN*MODWIDTH] +
                  imagebuf[pos+3*LINELEN*MODWIDTH] + imagebuf[pos+4*LINELEN*MODWIDTH] +
                  imagebuf[pos+5*LINELEN*MODWIDTH] + imagebuf[pos+6*LINELEN*MODWIDTH] +
                  imagebuf[pos+7*LINELEN*MODWIDTH])/8;

        if(imagebuf[pos]>middle && imagebuf[pos]>BIAS)
            ledbuffer[i] |= 1<<7;
        else
            ledbuffer[i] &= ~(1<<7);

        if(imagebuf[pos+LINELEN*MODWIDTH]>middle && imagebuf[pos+LINELEN*MODWIDTH]>BIAS)
            ledbuffer[i] |= 1<<6;
        else
            ledbuffer[i] &= ~(1<<6);

        if(imagebuf[pos+2*LINELEN*MODWIDTH]>middle && imagebuf[pos+2*LINELEN*MODWIDTH]>BIAS)
            ledbuffer[i] |= 1<<5;
        else
            ledbuffer[i] &= ~(1<<5);

        if(imagebuf[pos+3*LINELEN*MODWIDTH]>middle && imagebuf[pos+3*LINELEN*MODWIDTH]>BIAS)
            ledbuffer[i] |= 1<<4;
        else
            ledbuffer[i] &= ~(1<<4);

        if(imagebuf[pos+4*LINELEN*MODWIDTH]>middle && imagebuf[pos+4*LINELEN*MODWIDTH]>BIAS)
            ledbuffer[i] |= 1<<3;
        else
            ledbuffer[i] &= ~(1<<3);

        if(imagebuf[pos+5*LINELEN*MODWIDTH]>middle && imagebuf[pos+5*LINELEN*MODWIDTH]>BIAS)
            ledbuffer[i] |= 1<<2;
        else
            ledbuffer[i] &= ~(1<<2);

        if(imagebuf[pos+6*LINELEN*MODWIDTH]>middle && imagebuf[pos+6*LINELEN*MODWIDTH]>BIAS)
            ledbuffer[i] |= 1<<1;
        else
            ledbuffer[i] &= ~(1<<1);

        if(imagebuf[pos+7*LINELEN*MODWIDTH]>middle && imagebuf[pos+7*LINELEN*MODWIDTH]>BIAS)
            ledbuffer[i] |= 0x1;
        else
            ledbuffer[i] &= ~(0x1);

        }
    return 0;
}


int compare_Buffs(const char* buf1, const uint32_t buf1_len, const char* buf2, const uint32_t buf2_len, char* diffbuf, const uint32_t diffbuf_len)
{
    if(buf1_len != buf2_len && buf1_len != diffbuf_len)
    {
        printf("size of buffers are not equal buf1: %d buf2: %d diffbuf: %d\n", buf1_len, buf2_len, diffbuf_len);
        return 1;
    }

    size_t i = 0;

    size_t *p_buf1 = (uint32_t*)buf1;
    size_t *p_buf2 = (uint32_t*)buf2;
    size_t *p_diffbuf = (uint32_t*)diffbuf;

    for(i=0; i<(buf1_len/sizeof(size_t)); i++)
        p_diffbuf[i] = p_buf1[i]^p_buf2[i];

    return 0;
}
