/********************************************************
 * vpd.h
 *
 * this file provides function declarations for SER486
 * electronically erasable programmable rom (EEPROM) library functions.
 *
 */

 typedef struct
{
    char token[4];
    char model[12];
    char manufacturer[12];
    char serial_number[12];
    unsigned long manufacture_date;
    unsigned char mac_address[6];
    char country_of_origin[4];
    unsigned char checksum;
} vpd_struct;

vpd_struct vpd;


 /*
 initializes vpd member data from the EEPROM. If vpd data is
invalid after initialization, the EEPROM is written to “factory defaults”, and
the vpd data is reinitialized from the new EEPROM values.
 */
void vpd_init();

/*
returns 1 (true) if the vpd token is “SER” and the
checksum is valid. Otherwise, returns 0.
*/
int vpd_is_data_valid();

/*
called by init () to write the “factory defaults” to the
EEPROM.
*/

void vpd_write_defaults();
