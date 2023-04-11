/********************************************************
 * eeprom.h
 *
 * this file provides function declarations for SER486
 * electronically erasable programmable rom (EEPROM) library functions.
 *
 */

unsigned char writebuf[64];   //A 64 byte write buffer used by writebuf() and the interrupt service routine.
unsigned char bufidx;           //The index of the next writebuf character the interrupt service routine should write.
unsigned char writesize;        //The size of the data (in bytes) within the write buffer that must be written.
unsigned int writeaddr;         //Used by the interrupt service routine, specifies the next EEPROM address with which to write the data to.
volatile unsigned char write_busy;      //Set to 1 if there is data in the write buffer that needs to be written. Cleared to 0 by the interrupt service routine when the last byte of the data has been written.


/*
places the data (specified by buf and size) into the write buffer for later writing
to the EEPROM. The addr parameter specifies the location to write the data to. This
function should not be called when another write is in progress.
*/

void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char size);

/*
reads a specified amount of data (size) from the EEPROM starting at a
specified address (addr) and places it in the specified buffer (buf).
*/

void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char size);


/*
returns 0 if write_busy is 0, otheriwise, returns 1.
*/

int eeprom_isbusy();

/*
Enabled when writebuf() places new data in the write buffer, this ISR
sends one byte at a time to the EEPROM. When the last byte is sent, it disables further
EEPROM interrupts.
*/

void __vector_22();
