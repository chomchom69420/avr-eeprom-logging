#include "config.h"
#include "eeprom.h"
#include "util.h"
#include <string.h>

config_struct config_defaults = {
    .token="ASU",
    .hi_alarm=0x3FF,
    .hi_warn = 0x3FE,
    .lo_alarm = 0x0000,
    .lo_warn=0x0001,
    .use_static_ip=0,
    .static_ip={192, 168, 1, 100},
    .checksum=0};

void config_init()
{
    while(eeprom_isbusy());  //waits for EEPROM to get out of busy state

    //initialize config
    unsigned int base_addr=0x040;
    int size = sizeof(config_struct);
    unsigned char buf[size];
    eeprom_readbuf(base_addr, buf, size);

    memcpy((unsigned char*)&config, (unsigned char*)buf, size);

    if(!config_is_data_valid())
    {

        //Write default values to EEPROM location 0x040
        config_write_defaults();

        //Initialize config by reading location 0x040
        eeprom_readbuf(base_addr, (unsigned char*)buf, size);
        memcpy((unsigned char*)&config, (unsigned char*)buf, size);

        //Clearing modified flag
        modified=0;

    }

    //uart_writedec32(write_busy);
    //get_writebusy();
}

int config_is_data_valid()
{
    return (config.token=="ASU") && is_checksum_valid((unsigned char*)&config, sizeof(config_struct));
}

void config_set_modified()
{
    modified=1;
}

void config_update()
{
    if(!eeprom_isbusy() && modified)
    {
        //Update the checksum
        update_checksum((unsigned char*)&config, sizeof(config));

        //Write config to eeprom
        unsigned int base_addr = 0x040;
        int size = sizeof(config_struct);
        unsigned char buf[size];
        eeprom_writebuf(base_addr, (unsigned char*)buf, size);

        //Clearing modified flag
        modified=0;

    }

}

void config_write_defaults()
{
    update_checksum((unsigned char*)&config, sizeof(config));

    //Writing defaults to eeprom
    unsigned int base_addr = 0x040;
    int size = sizeof(config_struct);
    unsigned char buf[size];
    memcpy((unsigned char*)buf, (unsigned char*)&config_defaults, size);

    eeprom_writebuf(base_addr, (unsigned char*)buf, size);
}



