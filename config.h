/********************************************************
 * config.h
 */

 typedef struct
{
    char token[4];
    unsigned int hi_alarm;
    unsigned int hi_warn;
    unsigned int lo_alarm;
    unsigned int lo_warn;
    char use_static_ip;
    char static_ip[4];
    unsigned char checksum;
} config_struct;

config_struct config;

unsigned char modified;

 /*
initializes config member data from the EEPROM. If config data is invalid
after initialization, the EEPROM is written to “factory defaults”, and the config data is
reinitialized from the new EEPROM values.
*/
void config_init();

/*
if config has been modified and eeprom is not busy, write all of the
configuration data to the eeprom write buffer.
*/
void config_update();

/*
set the modified flag. This function should be called any time the
config data is modified.
*/
void config_set_modified();

/*
returns 1 (true) if the token is “ASU” and the checksum is valid.
Otherwise, returns 0.
*/
int config_is_data_valid();

/*
called by init () to write the config “factory defaults” to eeprom .
*/

void config_write_defaults();
