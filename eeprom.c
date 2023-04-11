#include "eeprom.h"
#include "uart.h"

#define EEARH  *((volatile char*) 0x42)
#define EEARL  *((volatile char*) 0x41)
#define EEDR  *((volatile char*) 0x40)
#define EECR  *((volatile char*) 0x3F)
#define SREG    *((volatile char*) 0x5F)
#define SPMCSR *((volatile char*) 0x57)

#define I_SREG 7
#define EEAR8 7
#define EEPM1 5
#define EEPM0 4
#define EERE 0
#define EEPE 1          //EEPROM write enable
#define EERIE 3         //EEPROM ready interrupt enable
#define EEMPE 2         //EEPROM master write enable
#define SELFPRGEN 0

void EEPROM_set_write_mode();

int eeprom_isbusy(){
    return write_busy==1;
}

void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char size){
    writeaddr = addr;
    write_busy = 1;
    bufidx = 0;

    //Copying
    for (int i=0;i<size;i++)
        writebuf[i] = buf[i];

    writesize = size;

    //init EEPROM
    EECR |= (1<<EEPM1);
    EEARH &= ~(1<<EEAR8);

    //TODO: Enable EEPROM ready interrupts
    SREG |= (1<<I_SREG);        //Global interrupt enable
    EECR |= (1<<EERIE);         //Enabling EEPROM ready interrupt
}

void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char size)
{
    unsigned int read_addr=addr;;
    for(int i=0;i<size;i++)
    {
        //Read from EEPROM and store in buf[0:size-1]
        while(EECR & (1<<EEPE));     //waiting until EEPE becomes 0)
        EEARL = read_addr;
        EECR |= (1<<EERE);
        //Wait for 4 cycles
        //uart_writestr("Here before");
        __builtin_avr_delay_cycles(4);
        //uart_writestr("Here after");

        buf[i] = EEDR;
        read_addr++;
    }
}

void __vector_22(){
    //EEPROM ready
    if(bufidx<writesize)
    {
        //Write writebuf[bufidx] to EEPROM writeaddr

        while (EECR & (1<<EEPE));     //waiting until EEPE becomes 0

        //while (SPMCSR & (1<<SELFPRGEN));     //waiting until SELFPRGEN becomes 0

        EEARL = writeaddr;                      //Writes address to EEARL register
        EEDR = writebuf[bufidx];                //Writes data to EEDR register

        EECR |= (1<<EEMPE);                     //enable master write
        EECR |= (1<<EEPE);                      //enable write

        writeaddr++;
        bufidx++;
    }
    else if (bufidx>=writesize)
    {
        //Disable EEPROM ready interrupts
        EECR &= ~(1<<EERIE);
        write_busy=0;
    }
}

void EEPROM_set_write_mode(){
    EECR |= (1<<EEPM1);
}


