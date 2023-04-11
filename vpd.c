#include "vpd.h"
#include "eeprom.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

vpd_struct vpd_defaults = {"SER", "Manish", "Mani", "abcd1234", 0, "sbsdjb", "USA", 0};

void vpd_init()
{
    while(eeprom_isbusy());  //waits for EEPROM to get out of busy state

    //Read 43 bytes from 0x000
    unsigned int base_addr=0x000;
    int size = sizeof(vpd_struct);
    unsigned char buf[size];
    eeprom_readbuf(base_addr, (unsigned char*)buf, size);

    memcpy((unsigned char*)&vpd, (unsigned char*)buf, size);

    if(!vpd_is_data_valid())
    {
        vpd_write_defaults();

        //Initialize data by reading default values
        eeprom_readbuf(base_addr, (unsigned char*)buf, size);
        memcpy((unsigned char*)&vpd, buf, size);

    }
}

int vpd_is_data_valid()
{
    return (vpd.token == "SER") && is_checksum_valid((unsigned char*)&vpd, sizeof(vpd_struct));
}

void vpd_write_defaults()
{
    update_checksum((unsigned char*)&vpd, sizeof(vpd));

    //Writing defaults to eeprom
    unsigned int base_addr = 0x000;
    int size = sizeof(vpd_struct);
    unsigned char buf[size];
    memcpy((unsigned char*)buf, (unsigned char*)&vpd_defaults, size);

    eeprom_writebuf(base_addr, (unsigned char*)buf, size);
}



