#include "util.h"
#include "string.h"

#define SREG    *((volatile char*) 0x5F)

#define C_SREG 0

unsigned char calc_checksum(unsigned char *data, unsigned int dsize)
{
    //Storing checksum result in unsigned byte
    unsigned char checksum_result;

    for(int i=0;i<dsize;i++)
    {
        checksum_result += data[i];
        if(SREG & (1<<C_SREG))
        {
            //carry wraparound
            checksum_result+=1;
        }

    }

    return ~checksum_result;

}

void update_checksum(unsigned char *data, unsigned int dsize)
{
    unsigned char checksum_val = calc_checksum(data, dsize);
    unsigned char last_byte = 0b11111111 + (~checksum_val + 0b00000001);
    data[dsize-1]=last_byte;
}


int is_checksum_valid(unsigned char* data, unsigned int dsize)
{
    unsigned char checksum_val = calc_checksum(data, dsize);

    if(checksum_val==0)
        return 1;
    else
        return 0;
}
