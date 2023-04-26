/*
*  Copyright (C) 2012 Libelium Comunicaciones Distribuidas S.L.
*  http://www.libelium.com
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Version 1.5
*  Author: Anartz Nuin Jiménez
*/

#include "arduPi.h"
//namespace unistd {
//All functions of unistd.h must be called like this: the_function()
#include <unistd.h>
//} // namespace unistd

struct bcm2835_peripheral gpio = {GPIO_BASE};
struct bcm2835_peripheral bsc_rev1 = {IOBASE + 0X205000};
struct bcm2835_peripheral bsc_rev2 = {IOBASE + 0X804000};
struct bcm2835_peripheral bsc0;
volatile uint32_t *bcm2835_bsc1;

void *spi0 = MAP_FAILED;
static  uint8_t *spi0Mem = NULL;

pthread_t idThread2;
pthread_t idThread3;
pthread_t idThread4;
pthread_t idThread5;
pthread_t idThread6;
pthread_t idThread7;
pthread_t idThread8;
pthread_t idThread9;
pthread_t idThread10;
pthread_t idThread11;
pthread_t idThread12;
pthread_t idThread13;

timeval start_program, end_point;






/*******************************
 *                             *
 * WirePi Class implementation *
 * --------------------------- *
 *******************************/

/******************
 * Public methods *
 ******************/

//Constructor
WirePi::WirePi(){
	REV = getBoardRev();
	if(map_peripheral(&gpio) == -1) {
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
	}
	
	memfd = -1;
	i2c_byte_wait_us = 0;
	
	// Open the master /dev/memory device
    if ((memfd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) 
    {
	fprintf(stderr, "bcm2835_init: Unable to open /dev/mem: %s\n",
		strerror(errno));
	exit(1);
    }
	
	bcm2835_bsc1 = mapmem("bsc1", BLOCK_SIZE, memfd, BCM2835_BSC1_BASE);
    if (bcm2835_bsc1 == MAP_FAILED) exit(1);
	
    // start timer
    gettimeofday(&start_program, NULL);
    
}

//Initiate the Wire library and join the I2C bus.
void WirePi::begin(){

	volatile uint32_t* paddr = bcm2835_bsc1 + BCM2835_BSC_DIV/4;

    // Set the I2C/BSC1 pins to the Alt 0 function to enable I2C access on them
    bcm2835_gpio_fsel(RPI_V2_GPIO_P1_03, BCM2835_GPIO_FSEL_ALT0); // SDA
    bcm2835_gpio_fsel(RPI_V2_GPIO_P1_05, BCM2835_GPIO_FSEL_ALT0); // SCL

    // Read the clock divider register
    uint16_t cdiv = bcm2835_peri_read(paddr);
    // Calculate time for transmitting one byte
    // 1000000 = micros seconds in a second
    // 9 = Clocks per byte : 8 bits + ACK
    i2c_byte_wait_us = ((float)cdiv / BCM2835_CORE_CLK_HZ) * 1000000 * 9;
}

//Begin a transmission to the I2C slave device with the given address
void WirePi::beginTransmission(unsigned char address){
	// Set I2C Device Address
	volatile uint32_t* paddr = bcm2835_bsc1 + BCM2835_BSC_A/4;
	bcm2835_peri_write(paddr, address);
}

//Writes data to the I2C.
void WirePi::write(char data){
	
	char i2cdata[1];
	i2cdata[0] = data;
	
	write(i2cdata,1);
	
}

//Writes data to the I2C.
uint8_t WirePi::write(const char * buf, uint32_t len){
	
	volatile uint32_t* dlen    = bcm2835_bsc1 + BCM2835_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm2835_bsc1 + BCM2835_BSC_FIFO/4;
    volatile uint32_t* status  = bcm2835_bsc1 + BCM2835_BSC_S/4;
    volatile uint32_t* control = bcm2835_bsc1 + BCM2835_BSC_C/4;

    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM2835_I2C_REASON_OK;

    // Clear FIFO
    bcm2835_peri_set_bits(control, BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );
    // Clear Status
	bcm2835_peri_write_nb(status, BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
	// Set Data Length
    bcm2835_peri_write_nb(dlen, len);
    // pre populate FIFO with max buffer
    while( remaining && ( i < BCM2835_BSC_FIFO_SIZE ) )
    {
        bcm2835_peri_write_nb(fifo, buf[i]);
        i++;
        remaining--;
    }
    
    // Enable device and start transfer
    bcm2835_peri_write_nb(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);
    
    // Transfer is over when BCM2835_BSC_S_DONE
    while(!(bcm2835_peri_read_nb(status) & BCM2835_BSC_S_DONE ))
    {
        while ( remaining && (bcm2835_peri_read_nb(status) & BCM2835_BSC_S_TXD ))
    	{
        	// Write to FIFO, no barrier
        	bcm2835_peri_write_nb(fifo, buf[i]);
        	i++;
        	remaining--;
    	}
    }

    // Received a NACK
    if (bcm2835_peri_read(status) & BCM2835_BSC_S_ERR)
    {
		reason = BCM2835_I2C_REASON_ERROR_NACK;
    }

    // Received Clock Stretch Timeout
    else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
    {
		reason = BCM2835_I2C_REASON_ERROR_CLKT;
    }

    // Not all data is sent
    else if (remaining)
    {
		reason = BCM2835_I2C_REASON_ERROR_DATA;
    }

    bcm2835_peri_set_bits(control, BCM2835_BSC_S_DONE , BCM2835_BSC_S_DONE);

    return reason;
}


void WirePi::endTransmission(){
	// Set all the I2C/BSC1 pins back to input
    bcm2835_gpio_fsel(RPI_V2_GPIO_P1_03, BCM2835_GPIO_FSEL_INPT); // SDA
    bcm2835_gpio_fsel(RPI_V2_GPIO_P1_05, BCM2835_GPIO_FSEL_INPT); // SCL
}

//Used by the master to request bytes from a slave device
void WirePi::requestFrom(unsigned char address,int quantity){
	// Set I2C Device Address
	volatile uint32_t* paddr = bcm2835_bsc1 + BCM2835_BSC_A/4;
	bcm2835_peri_write(paddr, address);
	
	i2c_bytes_to_read = quantity;
}

//Reads a byte that was transmitted from a slave device to a master after a call to WirePi::requestFrom()
unsigned char WirePi::read(){
	char buf;
	i2c_bytes_to_read=1;
	read(&buf);
    return static_cast<unsigned char>(buf);
}

uint8_t WirePi::read(char* buf){
    volatile uint32_t* dlen    = bcm2835_bsc1 + BCM2835_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm2835_bsc1 + BCM2835_BSC_FIFO/4;
    volatile uint32_t* status  = bcm2835_bsc1 + BCM2835_BSC_S/4;
    volatile uint32_t* control = bcm2835_bsc1 + BCM2835_BSC_C/4;

    uint32_t remaining = i2c_bytes_to_read;
    uint32_t i = 0;
    uint8_t reason = BCM2835_I2C_REASON_OK;

    // Clear FIFO
    bcm2835_peri_set_bits(control, BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );
    // Clear Status
	bcm2835_peri_write_nb(status, BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
	// Set Data Length
    bcm2835_peri_write_nb(dlen, i2c_bytes_to_read);
    // Start read
    bcm2835_peri_write_nb(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST | BCM2835_BSC_C_READ);
    
    // wait for transfer to complete
    while (!(bcm2835_peri_read_nb(status) & BCM2835_BSC_S_DONE))
    {
        // we must empty the FIFO as it is populated and not use any delay
        while (bcm2835_peri_read_nb(status) & BCM2835_BSC_S_RXD)
    	{
    		// Read from FIFO, no barrier
    		buf[i] = bcm2835_peri_read_nb(fifo);
        	i++;
        	remaining--;
    	}
    }
    
    // transfer has finished - grab any remaining stuff in FIFO
    while (remaining && (bcm2835_peri_read_nb(status) & BCM2835_BSC_S_RXD))
    {
        // Read from FIFO, no barrier
        buf[i] = bcm2835_peri_read_nb(fifo);
        i++;
        remaining--;
    }
    
    // Received a NACK
    if (bcm2835_peri_read(status) & BCM2835_BSC_S_ERR)
    {
		reason = BCM2835_I2C_REASON_ERROR_NACK;
    }

    // Received Clock Stretch Timeout
    else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
    {
		reason = BCM2835_I2C_REASON_ERROR_CLKT;
    }

    // Not all data is received
    else if (remaining)
    {
		reason = BCM2835_I2C_REASON_ERROR_DATA;
    }

    bcm2835_peri_set_bits(control, BCM2835_BSC_S_DONE , BCM2835_BSC_S_DONE);

    return reason;
}


// Read an number of bytes from I2C sending a repeated start after writing
// the required register. Only works if your device supports this mode
uint8_t WirePi::read_rs(char* regaddr, char* buf, uint32_t len){   
    volatile uint32_t* dlen    = bcm2835_bsc1 + BCM2835_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm2835_bsc1 + BCM2835_BSC_FIFO/4;
    volatile uint32_t* status  = bcm2835_bsc1 + BCM2835_BSC_S/4;
    volatile uint32_t* control = bcm2835_bsc1 + BCM2835_BSC_C/4;
    
	uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM2835_I2C_REASON_OK;
    
    // Clear FIFO
    bcm2835_peri_set_bits(control, BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );
    // Clear Status
	bcm2835_peri_write_nb(status, BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
	// Set Data Length
    bcm2835_peri_write_nb(dlen, 1);
    // Enable device and start transfer
    bcm2835_peri_write_nb(control, BCM2835_BSC_C_I2CEN);
    bcm2835_peri_write_nb(fifo, regaddr[0]);
    bcm2835_peri_write_nb(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);
    
    // poll for transfer has started
    while ( !( bcm2835_peri_read_nb(status) & BCM2835_BSC_S_TA ) )
    {
        // Linux may cause us to miss entire transfer stage
        if(bcm2835_peri_read(status) & BCM2835_BSC_S_DONE)
            break;
    }
    
    // Send a repeated start with read bit set in address
    bcm2835_peri_write_nb(dlen, len);
    bcm2835_peri_write_nb(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST  | BCM2835_BSC_C_READ );
    
    // Wait for write to complete and first byte back.	
    m_delayMicroseconds(i2c_byte_wait_us * 3);
    
    // wait for transfer to complete
    while (!(bcm2835_peri_read_nb(status) & BCM2835_BSC_S_DONE))
    {
        // we must empty the FIFO as it is populated and not use any delay
        while (remaining && bcm2835_peri_read_nb(status) & BCM2835_BSC_S_RXD)
    	{
    		// Read from FIFO, no barrier
    		buf[i] = bcm2835_peri_read_nb(fifo);
        	i++;
        	remaining--;
    	}
    }
    
    // transfer has finished - grab any remaining stuff in FIFO
    while (remaining && (bcm2835_peri_read_nb(status) & BCM2835_BSC_S_RXD))
    {
        // Read from FIFO, no barrier
        buf[i] = bcm2835_peri_read_nb(fifo);
        i++;
        remaining--;
    }
    
    // Received a NACK
    if (bcm2835_peri_read(status) & BCM2835_BSC_S_ERR)
    {
		reason = BCM2835_I2C_REASON_ERROR_NACK;
    }

    // Received Clock Stretch Timeout
    else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
    {
		reason = BCM2835_I2C_REASON_ERROR_CLKT;
    }

    // Not all data is sent
    else if (remaining)
    {
		reason = BCM2835_I2C_REASON_ERROR_DATA;
    }

    bcm2835_peri_set_bits(control, BCM2835_BSC_S_DONE , BCM2835_BSC_S_DONE);

    return reason;
}


/*******************
 * Private methods *
 *******************/

// Exposes the physical address defined in the passed structure using mmap on /dev/mem
int WirePi::map_peripheral(struct bcm2835_peripheral *p)
{
   // Open /dev/mem
   if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("Failed to open /dev/mem, try checking permissions.\n");
      return -1;
   }

   p->map = mmap(
      NULL,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      p->mem_fd,  // File descriptor to physical memory virtual file '/dev/mem'
      p->addr_p      // Address in physical map that we want this memory block to expose
   );

   if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
   }

   p->addr = (volatile unsigned int *)p->map;

   return 0;
}

void WirePi::unmap_peripheral(struct bcm2835_peripheral *p) {

    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

void WirePi::wait_i2c_done() {
        //Wait till done, let's use a timeout just in case
        int timeout = 50;
        while((!((BSC0_S) & BSC_S_DONE)) && --timeout) {
        usleep(1000);
        }
        if(timeout == 0)
            printf("wait_i2c_done() timeout. Something went wrong.\n");
}





/*******************************
 *                             *
 * SPIPi Class implementation *
 * --------------------------- *
 *******************************/

/******************
 * Public methods *
 ******************/

 SPIPi::SPIPi(){
	 
	REV = getBoardRev();

    uint8_t *mapaddr;

    if ((spi0Mem = (uint8_t*)malloc(BLOCK_SIZE + (PAGESIZE - 1))) == NULL){
        fprintf(stderr, "bcm2835_init: spi0Mem malloc failed: %s\n", strerror(errno));
        exit(1);
    }
    
    mapaddr = spi0Mem;
//    if (((uint32_t)mapaddr % PAGESIZE) != 0)
//        mapaddr += PAGESIZE - ((uint32_t)mapaddr % PAGESIZE);
    
//    spi0 = (uint32_t *)mmap(mapaddr, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, gpio.mem_fd, BCM2835_SPI0_BASE);
    
//    if ((int32_t)spi0 < 0){
//        fprintf(stderr, "bcm2835_init: mmap failed (spi0): %s\n", strerror(errno));
//        exit(1);
//    }
 }

void SPIPi::begin(){
    // Set the SPI0 pins to the Alt 0 function to enable SPI0 access on them
    bcm2835_gpio_fsel(7, BCM2835_GPIO_FSEL_ALT0); // CE1
    bcm2835_gpio_fsel(8, BCM2835_GPIO_FSEL_ALT0); // CE0
    bcm2835_gpio_fsel(9, BCM2835_GPIO_FSEL_ALT0); // MISO
    bcm2835_gpio_fsel(10, BCM2835_GPIO_FSEL_ALT0); // MOSI
    bcm2835_gpio_fsel(11, BCM2835_GPIO_FSEL_ALT0); // CLK
    
    // Set the SPI CS register to the some sensible defaults
    volatile uint32_t* paddr = (volatile uint32_t*)spi0 + BCM2835_SPI0_CS/4;
    bcm2835_peri_write(paddr, 0); // All 0s
    
    // Clear TX and RX fifos
    bcm2835_peri_write_nb(paddr, BCM2835_SPI0_CS_CLEAR);
}

void SPIPi::end(){  
    // Set all the SPI0 pins back to input
    bcm2835_gpio_fsel(7, BCM2835_GPIO_FSEL_INPT); // CE1
    bcm2835_gpio_fsel(8, BCM2835_GPIO_FSEL_INPT); // CE0
    bcm2835_gpio_fsel(9, BCM2835_GPIO_FSEL_INPT); // MISO
    bcm2835_gpio_fsel(10, BCM2835_GPIO_FSEL_INPT); // MOSI
    bcm2835_gpio_fsel(11, BCM2835_GPIO_FSEL_INPT); // CLK
}

void SPIPi::setBitOrder(uint8_t order){
    // BCM2835_SPI_BIT_ORDER_MSBFIRST is the only one suported by SPI0
}

// defaults to 0, which means a divider of 65536.
// The divisor must be a power of 2. Odd numbers
// rounded down. The maximum SPI clock rate is
// of the APB clock
void SPIPi::setClockDivider(uint16_t divider){
    volatile uint32_t* paddr = (volatile uint32_t*)spi0 + BCM2835_SPI0_CLK/4;
    bcm2835_peri_write(paddr, divider);
}

void SPIPi::setDataMode(uint8_t mode){
    volatile uint32_t* paddr = (volatile uint32_t*)spi0 + BCM2835_SPI0_CS/4;
    // Mask in the CPO and CPHA bits of CS
    bcm2835_peri_set_bits(paddr, mode << 2, BCM2835_SPI0_CS_CPOL | BCM2835_SPI0_CS_CPHA);
}

// Writes (and reads) a single byte to SPI
uint8_t SPIPi::transfer(uint8_t value){
    volatile uint32_t* paddr = (volatile uint32_t*)spi0 + BCM2835_SPI0_CS/4;
    volatile uint32_t* fifo = (volatile uint32_t*)spi0 + BCM2835_SPI0_FIFO/4;

    bcm2835_peri_set_bits(paddr, BCM2835_SPI0_CS_CLEAR, BCM2835_SPI0_CS_CLEAR);

    bcm2835_peri_set_bits(paddr, BCM2835_SPI0_CS_TA, BCM2835_SPI0_CS_TA);

    while (!(bcm2835_peri_read(paddr) & BCM2835_SPI0_CS_TXD))
    m_delayMicroseconds(10);

    bcm2835_peri_write_nb(fifo, value);

    while (!(bcm2835_peri_read_nb(paddr) & BCM2835_SPI0_CS_DONE))
    m_delayMicroseconds(10);

    uint32_t ret = bcm2835_peri_read_nb(fifo);

    bcm2835_peri_set_bits(paddr, 0, BCM2835_SPI0_CS_TA);

    return ret;
}

// Writes (and reads) a number of bytes to SPI
void SPIPi::transfernb(char* tbuf, char* rbuf, uint32_t len){
    volatile uint32_t* paddr = (volatile uint32_t*)spi0 + BCM2835_SPI0_CS/4;
    volatile uint32_t* fifo = (volatile uint32_t*)spi0 + BCM2835_SPI0_FIFO/4;

    // This is Polled transfer as per section 10.6.1
    // BUG ALERT: what happens if we get interupted in this section, and someone else
    // accesses a different peripheral? 

    // Clear TX and RX fifos
    bcm2835_peri_set_bits(paddr, BCM2835_SPI0_CS_CLEAR, BCM2835_SPI0_CS_CLEAR);

    // Set TA = 1
    bcm2835_peri_set_bits(paddr, BCM2835_SPI0_CS_TA, BCM2835_SPI0_CS_TA);

    uint32_t i;
    for (i = 0; i < len; i++)
    {
    // Maybe wait for TXD
    while (!(bcm2835_peri_read(paddr) & BCM2835_SPI0_CS_TXD))
        m_delayMicroseconds(10);

    // Write to FIFO, no barrier
    bcm2835_peri_write_nb(fifo, tbuf[i]);

    // Wait for RXD
    while (!(bcm2835_peri_read(paddr) & BCM2835_SPI0_CS_RXD))
        m_delayMicroseconds(10);

    // then read the data byte
    rbuf[i] = bcm2835_peri_read_nb(fifo);
    }
    // Wait for DONE to be set
    while (!(bcm2835_peri_read_nb(paddr) & BCM2835_SPI0_CS_DONE))
    m_delayMicroseconds(10);

    // Set TA = 0, and also set the barrier
    bcm2835_peri_set_bits(paddr, 0, BCM2835_SPI0_CS_TA);
}

void SPIPi::chipSelect(uint8_t cs){
    volatile uint32_t* paddr = (volatile uint32_t*)spi0 + BCM2835_SPI0_CS/4;
    // Mask in the CS bits of CS
    bcm2835_peri_set_bits(paddr, cs, BCM2835_SPI0_CS_CS);
}

void SPIPi::setChipSelectPolarity(uint8_t cs, uint8_t active){
    volatile uint32_t* paddr = (volatile uint32_t*)spi0 + BCM2835_SPI0_CS/4;
    uint8_t shift = 21 + cs;
    // Mask in the appropriate CSPOLn bit
    bcm2835_peri_set_bits(paddr, active << shift, 1 << shift);
}




/********** FUNCTIONS OUTSIDE CLASSES **********/

// Sleep the specified milliseconds
void m_delay(long millis){
    usleep(millis * 1000);
}

void m_delayMicroseconds(long micros){
	if (micros > 100){
		struct timespec tim, tim2;
		tim.tv_sec = 0;
		tim.tv_nsec = micros * 1000;

		if(nanosleep(&tim , &tim2) < 0 )   {
		  fprintf(stderr,"Nano sleep system call failed \n");
		  exit(1);
		}
	}else{
		struct timeval tNow, tLong, tEnd;
		
		gettimeofday (&tNow, NULL);
		tLong.tv_sec  = micros / 1000000;
		tLong.tv_usec = micros % 1000000;
		timeradd (&tNow, &tLong, &tEnd);

		while (timercmp (&tNow, &tEnd, <))
			gettimeofday (&tNow, NULL);
	}
}

uint8_t shiftIn(uint8_t dPin, uint8_t cPin, bcm2835SPIBitOrder order){
	uint8_t value = 0;
	int8_t  i;

	if (order == MSBFIRST)
		for (i = 7; i >= 0; --i){
            m_digitalWrite (cPin, m_HIGH);
            value |= m_digitalRead (dPin) << i;
            m_digitalWrite (cPin, m_LOW);
		}
	else
		for (i = 0; i < 8; ++i){
          m_digitalWrite (cPin, m_HIGH);
          value |= m_digitalRead (dPin) << i;
          m_digitalWrite (cPin, m_LOW);
		}

	return value;
}

void shiftOut(uint8_t dPin, uint8_t cPin, bcm2835SPIBitOrder order, uint8_t val){
	int8_t i;

	if (order == MSBFIRST)
		for (i = 7; i >= 0; --i){	
            m_digitalWrite (dPin, val & (1 << i));
            m_digitalWrite (cPin, m_HIGH);
            m_digitalWrite (cPin, m_LOW);
		}
	else
		for (i = 0; i < 8; ++i){
            m_digitalWrite (dPin, val & (1 << i));
            m_digitalWrite (cPin, m_HIGH);
            m_digitalWrite (cPin, m_LOW);
		}
}

// Configures the specified pin to behave either as an input or an output
void m_pinMode(int pin, m_Pinmode mode){
	pin = raspberryPinNumber(pin);
    if(mode == m_OUTPUT){
		switch(pin){
			case 4:  GPFSEL0 &= ~(7 << 12); GPFSEL0 |= (1 << 12); break;
			case 8:  GPFSEL0 &= ~(7 << 24); GPFSEL0 |= (1 << 24); break;
			case 9:  GPFSEL0 &= ~(7 << 27); GPFSEL0 |= (1 << 27); break;
			case 10: GPFSEL1 &= ~(7 << 0); 	GPFSEL1 |= (1 << 0);  break;
			case 11: GPFSEL1 &= ~(7 << 3);  GPFSEL1 |= (1 << 3);  break;
			case 17: GPFSEL1 &= ~(7 << 21); GPFSEL1 |= (1 << 21); break;
			case 18: GPFSEL1 &= ~(7 << 24); GPFSEL1 |= (1 << 24); break;
			case 21: GPFSEL2 &= ~(7 << 3);  GPFSEL2 |= (1 << 3);  break;
			case 27: GPFSEL2 &= ~(7 << 21); GPFSEL2 |= (1 << 21); break;
			case 22: GPFSEL2 &= ~(7 << 6);  GPFSEL2 |= (1 << 6);  break;
			case 23: GPFSEL2 &= ~(7 << 9);  GPFSEL2 |= (1 << 9);  break;
			case 24: GPFSEL2 &= ~(7 << 12); GPFSEL2 |= (1 << 12); break;
			case 25: GPFSEL2 &= ~(7 << 15); GPFSEL2 |= (1 << 15); break;
		}

    }else if (mode == m_INPUT){
		switch(pin){
			case 4:  GPFSEL0 &= ~(7 << 12); break;
			case 8:  GPFSEL0 &= ~(7 << 24); break;
			case 9:  GPFSEL0 &= ~(7 << 27); break;
			case 10: GPFSEL1 &= ~(7 << 0);  break;
			case 11: GPFSEL1 &= ~(7 << 3);  break;		
            case 17: GPFSEL1 &= ~(7 << 21); break;
			case 18: GPFSEL1 &= ~(7 << 24); break;
			case 21: GPFSEL2 &= ~(7 << 3);  break;
			case 27: GPFSEL2 &= ~(7 << 3);  break;
			case 22: GPFSEL2 &= ~(7 << 6);  break;
			case 23: GPFSEL2 &= ~(7 << 9);  break;
			case 24: GPFSEL2 &= ~(7 << 12); break;
			case 25: GPFSEL2 &= ~(7 << 15); break;
		}
	}
}

// Write a m_HIGH or a m_LOW value to a digital pin
void m_digitalWrite(int pin, int value){
	pin = raspberryPinNumber(pin);
    if (value == m_HIGH){
		switch(pin){
			case  4:GPSET0 =  BIT_4;break;
			case  8:GPSET0 =  BIT_8;break;
			case  9:GPSET0 =  BIT_9;break;
			case 10:GPSET0 = BIT_10;break;
			case 11:GPSET0 = BIT_11;break;
			case 17:GPSET0 = BIT_17;break;
			case 18:GPSET0 = BIT_18;break;
			case 21:GPSET0 = BIT_21;break;
			case 27:GPSET0 = BIT_27;break;
			case 22:GPSET0 = BIT_22;break;
			case 23:GPSET0 = BIT_23;break;
			case 24:GPSET0 = BIT_24;break;
			case 25:GPSET0 = BIT_25;break;
		}
    }else if(value == m_LOW){
		switch(pin){
			case  4:GPCLR0 =  BIT_4;break;
			case  8:GPCLR0 =  BIT_8;break;
			case  9:GPCLR0 =  BIT_9;break;
			case 10:GPCLR0 = BIT_10;break;
			case 11:GPCLR0 = BIT_11;break;
			case 17:GPCLR0 = BIT_17;break;
			case 18:GPCLR0 = BIT_18;break;
			case 21:GPCLR0 = BIT_21;break;
			case 27:GPCLR0 = BIT_27;break;
			case 22:GPCLR0 = BIT_22;break;
			case 23:GPCLR0 = BIT_23;break;
			case 24:GPCLR0 = BIT_24;break;
			case 25:GPCLR0 = BIT_25;break;
		}
	}
        m_delayMicroseconds(1);    // Delay to alm_LOW any change in state to be reflected in the LEVn, register bit.
}



// Reads the value from a specified digital pin, either m_HIGH or m_LOW.
int m_digitalRead(int pin){
    m_Digivalue value;
	pin = raspberryPinNumber(pin);
	switch(pin){
        case 4: if(GPLEV0 & BIT_4){value = m_HIGH;} else{value = m_LOW;};break;
        case 8: if(GPLEV0 & BIT_8){value = m_HIGH;} else{value = m_LOW;};break;
        case 9: if(GPLEV0 & BIT_9){value = m_HIGH;} else{value = m_LOW;};break;
        case 10:if(GPLEV0 & BIT_10){value = m_HIGH;} else{value = m_LOW;};break;
        case 11:if(GPLEV0 & BIT_11){value = m_HIGH;} else{value = m_LOW;};break;
        case 17:if(GPLEV0 & BIT_17){value = m_HIGH;}else{value = m_LOW;};break;
        case 18:if(GPLEV0 & BIT_18){value = m_HIGH;}else{value = m_LOW;};break;
        case 21:if(GPLEV0 & BIT_21){value = m_HIGH;}else{value = m_LOW;};break;
        case 27:if(GPLEV0 & BIT_27){value = m_HIGH;}else{value = m_LOW;};break;
        case 22:if(GPLEV0 & BIT_22){value = m_HIGH;}else{value = m_LOW;};break;
        case 23:if(GPLEV0 & BIT_23){value = m_HIGH;}else{value = m_LOW;};break;
        case 24:if(GPLEV0 & BIT_24){value = m_HIGH;}else{value = m_LOW;};break;
        case 25:if(GPLEV0 & BIT_25){value = m_HIGH;}else{value = m_LOW;};break;
	}
	return value;
}

int analogRead (int pin){

int value;
int address; 

	if (pin == 0) {
		address = 0xDC;
	} else if (pin == 1){
		address = 0x9C;
	} else if (pin == 2){ 
		address = 0xCC;
	} else if (pin == 3){ 
		address = 0x8C;
	} else if (pin == 4){ 
		address = 0xAC;
	} else if (pin == 5){ 
		address = 0xEC;
	} else if (pin == 6){ 
		address = 0xBC;
    } else /*if (pin == 7)*/{ //RISK
		address = 0xFC;
	}
	
		
	Wire.begin();
	Wire.beginTransmission(8); 
	Wire.write(byte(address));            
  
	byte val_0 = (byte)Wire.read();
	byte val_1 = (byte)Wire.read();
	
	value = int(val_0)*16 + int(val_1>>4);
	value = value * 1023 / 4095;  //mapping the value between 0 and 1023
	return value;
}

void attachInterrupt(int p, void (*f)(), m_Digivalue m){
	int GPIOPin = raspberryPinNumber(p);
	pthread_t *threadId = getThreadIdFromPin(p);
	struct ThreadArg *threadArgs = (ThreadArg *)malloc(sizeof(ThreadArg));
	threadArgs->func = f;
	threadArgs->pin = GPIOPin;
	
	//Export pin for interrupt
	FILE *fp = fopen("/sys/class/gpio/export","w");
	if (fp == NULL){
		fprintf(stderr,"Unable to export pin %d for interrupt\n",p);
		exit(1);
	}else{
		fprintf(fp,"%d",GPIOPin); 
	}
	fclose(fp);
	
	//The system to create the file /sys/class/gpio/gpio<GPIO number>
	//So we wait a bit
     m_delay(1);
	
	char * interruptFile = NULL;
	asprintf(&interruptFile, "/sys/class/gpio/gpio%d/edge",GPIOPin);
	
	//Set detection condition
	fp = fopen(interruptFile,"w");
	if (fp == NULL){
		fprintf(stderr,"Unable to set detection type on pin %d\n",p);
		exit(1);
	}else{
		switch(m){
            case m_RISING: fprintf(fp,"rising");break;
            case m_FALLING: fprintf(fp,"falling");break;
			default: fprintf(fp,"both");break;
		}
		
	}
	fclose(fp);
	
	if(*threadId == 0){
		//Create a thread passing the pin and function
		pthread_create (threadId, NULL, threadFunction, (void *)threadArgs);
	}else{
		//First cancel the existing thread for that pin
		pthread_cancel(*threadId);
		//Create a thread passing the pin, function and mode
		pthread_create (threadId, NULL, threadFunction, (void *)threadArgs);
	}
	
}

void detachInterrupt(int p){
	int GPIOPin = raspberryPinNumber(p);
	
	FILE *fp = fopen("/sys/class/gpio/unexport","w");
	if (fp == NULL){
		fprintf(stderr,"Unable to unexport pin %d for interrupt\n",p);
		exit(1);
	}else{
		fprintf(fp,"%d",GPIOPin); 
	}
	fclose(fp);
	
	pthread_t *threadId = getThreadIdFromPin(p);
	pthread_cancel(*threadId);
}

long millis(){
	long elapsedTime;
	// stop timer
    gettimeofday(&end_point, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (end_point.tv_sec - start_program.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (end_point.tv_usec - start_program.tv_usec) / 1000.0;   // us to ms
    return elapsedTime;
}

/* Some helper functions */

int getBoardRev(){
	
	FILE *cpu_info;
	char line [120];
	char *c,finalChar;
	static int rev = 0;
	
	if (REV != 0) return REV;
	
	if ((cpu_info = fopen("/proc/cpuinfo","r"))==NULL){
		fprintf(stderr,"Unable to open /proc/cpuinfo. Cannot determine board reivision.\n");
		exit(1);
	}
	
	while (fgets (line,120,cpu_info) != NULL){
		if(strncmp(line,"Revision",8) == 0) break;
	}
	
	fclose(cpu_info);
	
    if (line == NULL){
		fprintf (stderr, "Unable to determine board revision from /proc/cpuinfo.\n");
		exit(1);
	}

	for (c = line; *c; ++c)
    if (isdigit (*c))
      break;

	if (!isdigit (*c)){
		fprintf (stderr, "Unable to determine board revision from /proc/cpuinfo\n");
		fprintf (stderr, "  (Info not found in: %s\n", line);
		exit(1);
	}
	
	finalChar = c [strlen (c) - 2];
	
	if ((finalChar == '2') || (finalChar == '3')){
		bsc0 = bsc_rev1;
		return 1;
	}else{
		bsc0 = bsc_rev2;
		return 2;
	}
}

uint32_t* mapmem(const char *msg, size_t size, int fd, off_t off)
{
    uint32_t *map = (uint32_t *)mmap(NULL, size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, off);
    if (MAP_FAILED == map)
	fprintf(stderr, "bcm2835_init: %s mmap failed: %s\n", msg, strerror(errno));
    return map;
}

int raspberryPinNumber(int arduinoPin){
	switch(arduinoPin){
		case 2: return 18; break;
		case 3: return 23; break;
		case 4: return 24; break;
		case 5: return 25; break;
		case 6: return  4; break;
		case 7: return 17; break;
		case 8: if(REV == 1){return 21;}else{return 27;} break;
		case 9: return 22; break;
		case 10: return 8; break;
		case 11: return 10; break;
		case 12: return 9; break;
		case 13: return 11; break;
	}
        return 11;
}

// safe read from peripheral
uint32_t bcm2835_peri_read(volatile uint32_t* paddr){
    uint32_t ret = *paddr;
    return ret;

}

// read from peripheral without the read barrier
uint32_t bcm2835_peri_read_nb(volatile uint32_t* paddr){
    return *paddr;
}

// safe write to peripheral
void bcm2835_peri_write(volatile uint32_t* paddr, uint32_t value){
    *paddr = value;
    *paddr = value;
}

// write to peripheral without the write barrier
void bcm2835_peri_write_nb(volatile uint32_t* paddr, uint32_t value){
    *paddr = value;
}

// Set/clear only the bits in value covered by the mask
void bcm2835_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask){
    uint32_t v = bcm2835_peri_read(paddr);
    v = (v & ~mask) | (value & mask);
    bcm2835_peri_write(paddr, v);
}

//
// m_LOW level convenience functions
//

// Function select
// pin is a BCM2835 GPIO pin number NOT RPi pin number
//      There are 6 control registers, each control the functions of a block
//      of 10 pins.
//      Each control register has 10 sets of 3 bits per GPIO pin:
//
//      000 = GPIO Pin X is an input
//      001 = GPIO Pin X is an output
//      100 = GPIO Pin X takes alternate function 0
//      101 = GPIO Pin X takes alternate function 1
//      110 = GPIO Pin X takes alternate function 2
//      111 = GPIO Pin X takes alternate function 3
//      011 = GPIO Pin X takes alternate function 4
//      010 = GPIO Pin X takes alternate function 5
//
// So the 3 bits for port X are:
//      X / 10 + ((X % 10) * 3)
void bcm2835_gpio_fsel(uint8_t pin, uint8_t mode){
    // Function selects are 10 pins per 32 bit word, 3 bits per pin
    volatile uint32_t* paddr = (volatile uint32_t*)gpio.map + BCM2835_GPFSEL0/4 + (pin/10);
    uint8_t   shift = (pin % 10) * 3;
    uint32_t  mask = BCM2835_GPIO_FSEL_MASK << shift;
    uint32_t  value = mode << shift;
    bcm2835_peri_set_bits(paddr, value, mask);
}

pthread_t *getThreadIdFromPin(int pin){
	switch(pin){
		case 2: return &idThread2; break;
		case 3: return &idThread3; break;
		case 4: return &idThread4; break;
		case 5: return &idThread5; break;
		case 6: return &idThread6; break;
		case 7: return &idThread7; break;
		case 8: return &idThread8; break;
		case 9: return &idThread9; break;
		case 10: return &idThread10; break;
		case 11: return &idThread11; break;
		case 12: return &idThread12; break;
		case 13: return &idThread13; break;
	}
}

/* This is the function that will be running in a thread if
 * attachInterrupt() is called */
void * threadFunction(void *args){
	ThreadArg *arguments = (ThreadArg *)args;
	int pin = arguments->pin;
	
	int GPIO_FN_MAXLEN = 32;
	int RDBUF_LEN = 5;
	
	char fn[GPIO_FN_MAXLEN];
	int fd,ret;
	struct pollfd pfd;
	char rdbuf [RDBUF_LEN];
	
	memset(rdbuf, 0x00, RDBUF_LEN);
	memset(fn,0x00,GPIO_FN_MAXLEN);
	
	snprintf(fn, GPIO_FN_MAXLEN-1, "/sys/class/gpio/gpio%d/value",pin);
	fd=open(fn, O_RDONLY);
	if(fd<0){
		perror(fn);
		exit(1);
	}
	pfd.fd=fd;
	pfd.events=POLLPRI;

    ret = read(fd, rdbuf, RDBUF_LEN - 1);
    if(ret<0){
		perror("Error reading interrupt file\n");
		exit(1);
	}
	
	while(1){
		memset(rdbuf, 0x00, RDBUF_LEN);
        lseek(fd, 0, SEEK_SET);
        ret=poll(&pfd, 1, -1);
		if(ret<0){
			perror("Error waiting for interrupt\n");
            close(fd);
            exit(1);
		}
		if(ret==0){
			printf("Timeout\n");
			continue;
		}
        ret = read(fd, rdbuf, RDBUF_LEN - 1);
        if(ret<0){
			perror("Error reading interrupt file\n");
			exit(1);
		}
		//Interrupt. We call user function.
		arguments->func();
	}
}

//SerialPi Serial = SerialPi();
WirePi Wire = WirePi();
SPIPi SPI = SPIPi();
