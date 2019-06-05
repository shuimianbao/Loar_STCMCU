#ifndef _CRC_H_
#define _CRC_H_

typedef union {

    struct {
        uint8_t Low;
        uint8_t High;
    } crcchar;
    uint16_t allbyte;
} CrcSum;


uint16_t CalculatePacketCrc(uint16_t usCRCIn);
#endif /*_CRC_H_*/
