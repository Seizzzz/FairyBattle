#include "crc32.h"

crc32::crc32(unsigned char* buf, unsigned nLength)
{
    val = calCRC(buf, nLength);
}

uint32_t crc32::calCRC(unsigned char* buf, unsigned nLength)
{
    if (nLength < 1)
        return 0xffffffff;

    uint32_t crc = 0;

    for (unsigned i = 0; i != nLength; ++i)
    {
        crc = table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    }

    crc = crc ^ 0xffffffff;

    return crc;
}

uint32_t crc32::getCRC()
{
    return val;
}
