// MODIFS pour Kitco:
// 1: remove unused putstring function
// 2: -DLED_START_FLASHES=0 in the makefile
// 3: change LOADER name to CFG much shorter
// 4: make mmc_fat.c simpler so it will not look for EEP extension or something!   


#define OPTIBOOT_MAJVER 4
#define OPTIBOOT_MINVER 4

#define MAKESTR(a) #a
#define MAKEVER(a, b) MAKESTR(a*256+b)

#define PC3 3

//FJA : vraiment?
#define PD7 7

asm("  .section .version\n"
    "optiboot_version:  .word " MAKEVER(OPTIBOOT_MAJVER, OPTIBOOT_MINVER) "\n"
    "  .section .text\n");

asm(	".section .jumps,\"ax\",@progbits\n"
		".global _jumptable\n"
		"_jumptable:\n"
		"rjmp write_flash_page\n"
		"rjmp load_game\n"
		".section .text\n");

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

// <avr/boot.h> uses sts instructions, but this version uses out instructions
// This saves cycles and program memory.
#include "boot.h"
#include <string.h>
#include "prog_flash.h"


// We don't use <avr/wdt.h> as those routines have interrupt overhead we don't need.

#include "pin_defs.h"
#include "stk500.h"
#include "mmc_fat.h"

#ifndef LED_START_FLASHES
#define LED_START_FLASHES 3
#endif

#ifdef LUDICROUS_SPEED
#define BAUD_RATE 230400L
#endif

/* set the UART baud rate defaults */
#ifndef BAUD_RATE
#if F_CPU >= 8000000L
#define BAUD_RATE   19200L // Highest rate Avrdude win32 will support
#elsif F_CPU >= 1000000L
#define BAUD_RATE   9600L   // 19200 also supported, but with significant error
#elsif F_CPU >= 128000L
#define BAUD_RATE   4800L   // Good for 128kHz internal RC
#else
#define BAUD_RATE 1200L     // Good even at 32768Hz
#endif
#endif



/* Switch in soft UART for hard baud rates */
#if (F_CPU/BAUD_RATE) > 280 // > 57600 for 16MHz
#ifndef SOFT_UART
#define SOFT_UART
#endif
#endif

/* Watchdog settings */
#define WATCHDOG_OFF    (0)
#define WATCHDOG_16MS   (_BV(WDE))
#define WATCHDOG_32MS   (_BV(WDP0) | _BV(WDE))
#define WATCHDOG_64MS   (_BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_250MS  (_BV(WDP2) | _BV(WDE))
#define WATCHDOG_500MS  (_BV(WDP2) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_2S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE))
#ifndef __AVR_ATmega8__
#define WATCHDOG_4S     (_BV(WDP3) | _BV(WDE))
#define WATCHDOG_8S     (_BV(WDP3) | _BV(WDP0) | _BV(WDE))
#endif

/* Function Prototypes */
/* The main function is in init9, which removes the interrupt vector table */
/* we don't need. It is also 'naked', which means the compiler does not    */
/* generate any entry or exit code itself. */
int main(void) __attribute__ ((naked)) __attribute__ ((section (".init9")));
void putch(char);
void mydelay(unsigned long);
//void putstring(char * str);
uint8_t getch(void);
static inline void getNch(uint8_t); /* "static inline" is a compiler hint to reduce code size */
void verifySpace();
static inline void flash_led(uint8_t);
uint8_t getLen();
static inline void watchdogReset();
void watchdogConfig(uint8_t x);
static inline void reboot();
void load_loader();
void load_game(const char * filename);

#ifdef SOFT_UART
void uartDelay() __attribute__ ((naked));
#endif
void appStart() __attribute__ ((naked));

#if defined(__AVR_ATmega168__)
#define RAMSTART (0x100)
#define NRWWSTART (0x3800)
#elif defined(__AVR_ATmega328P__)
#define RAMSTART (0x100)
#define NRWWSTART (0x7000)
#elif defined (__AVR_ATmega644P__)
#define RAMSTART (0x100)
#define NRWWSTART (0xE000)
#elif defined(__AVR_ATtiny84__)
#define RAMSTART (0x100)
#define NRWWSTART (0x0000)
#elif defined(__AVR_ATmega1280__)
#define RAMSTART (0x200)
#define NRWWSTART (0xE000)
#elif defined(__AVR_ATmega8__) || defined(__AVR_ATmega88__)
#define RAMSTART (0x100)
#define NRWWSTART (0x1800)
#endif

/* C zero initialises all global variables. However, that requires */
/* These definitions are NOT zero initialised, but that doesn't matter */
/* This allows us to drop the zero init code, saving us memory */
#define buff    ((uint8_t*)(RAMSTART))
#ifdef VIRTUAL_BOOT_PARTITION
#define rstVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+4))
#define wdtVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+6))
#endif

// FJA: LED VERTE ON
void greenLedON() {
    DDRC |= (1 <<PC3);
    PORTC |= (1 <<PC3);
   // mydelay(250000); // for debouncing

}


/* main program starts here */
int main(void) {
  uint8_t ch;

  /*
   * Making these local and in registers prevents the need for initializing
   * them, and also saves space because code no longer stores to memory.
   * (initializing address keeps the compiler happy, but isn't really
   *  necessary, and uses 4 bytes of flash.)
   */
  register uint16_t address = 0;
  register uint8_t  length;

  // After the zero init loop, this is the first code to run.
  //
  // This code makes the following assumptions:
  //  No interrupts will execute
  //  SP points to RAMEND
  //  r1 contains zero
  //
  // If not, uncomment the following instructions:
  // cli();
  asm volatile ("clr __zero_reg__");
#ifdef __AVR_ATmega8__
  SP=RAMEND;  // This is done by hardware reset
#endif

    
  // Adaboot no-wait mod, part 1
  // I split this up to use the same functionality to prevent
  // loader app being reflashed unnecessarily
  // Jonne Valola 2014
  ch = MCUSR;
  MCUSR = 0;

//#if 0
  // if the down button is currently pressed then try to run the loader app
  DDRD &= ~(1 << PD7);
  PORTD |= (1 << PD7);
  /* This is original Gamebuino code
  if (!(PINC & (1<<PC3))) 
  	  load_loader(); */ 
  if (!(PIND & (1<<PD7)) && !(ch & _BV(EXTRF)) ) {
	  // Use MCUSR to determine whether its a hard or soft boot.
	  // Only boot to loader if it is a hard boot - JV2014
	  watchdogConfig(WATCHDOG_OFF); // turn off watchdog so we do not reset
	  mydelay(25000); // for debouncing
	   if (!(PIND & (1<<PD7))) {
		   mydelay(25000); // for debouncing
           if (!(PIND & (1<<PD7)))     load_loader();//greenLedON();
//load_loader(); // ok, now sure it is wanted
	  } 
	  // perform normal boot otherwise
	  watchdogConfig(WATCHDOG_250MS); 
  }
//#endif

    
  // Adaboot no-wait mod, part 2
  if (!(ch & _BV(EXTRF))) appStart();

  
    
#if LED_START_FLASHES > 0
  // Set up Timer 1 for timeout counter
  TCCR1B = _BV(CS12) | _BV(CS10); // div 1024
#endif
#ifndef SOFT_UART
#ifdef __AVR_ATmega8__
  UCSRA = _BV(U2X); //Double speed mode USART
  UCSRB = _BV(RXEN) | _BV(TXEN);  // enable Rx & Tx
  UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);  // config USART; 8N1
  UBRRL = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );
#else
  UCSR0A = _BV(U2X0); //Double speed mode USART0
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
  UBRR0L = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );
#endif
#endif

  // Set up watchdog to trigger after 500ms
  //watchdogConfig(WATCHDOG_OFF); // FJA DEBUG: turn off watchdog so we do not reset
  watchdogConfig(WATCHDOG_1S); // ORIGINAL CODE (BEFORE FJA)

  /* Set LED pin as output */
  //LED_DDR |= _BV(LED);

#ifdef SOFT_UART
  /* Set TX pin as output */
  //UART_DDR |= _BV(UART_TX_BIT);
#endif

#if LED_START_FLASHES > 0
  /* Flash onboard LED to signal entering of bootloader */
  flash_led(LED_START_FLASHES * 2);
#endif


  /* Forever loop */
  for (;;) {
    /* get character from UART */
    ch = getch();

    if(ch == STK_GET_PARAMETER) {
      unsigned char which = getch();
      verifySpace();
      if (which == 0x82) {
	/*
	 * Send optiboot version as "minor SW version"
	 */
	putch(OPTIBOOT_MINVER);
      } else if (which == 0x81) {
	  putch(OPTIBOOT_MAJVER);
      } else {
	/*
	 * GET PARAMETER returns a generic 0x03 reply for
         * other parameters - enough to keep Avrdude happy
	 */
	putch(0x03);
      }
    }
    else if(ch == STK_SET_DEVICE) {
      // SET DEVICE is ignored
      getNch(20);
    }
    else if(ch == STK_SET_DEVICE_EXT) {
      // SET DEVICE EXT is ignored
      getNch(5);
    }
    else if(ch == STK_LOAD_ADDRESS) {
      // LOAD ADDRESS
      uint16_t newAddress;
      newAddress = getch();
      newAddress = (newAddress & 0xff) | (getch() << 8);
#ifdef RAMPZ
      // Transfer top bit to RAMPZ
      RAMPZ = (newAddress & 0x8000) ? 1 : 0;
#endif
      newAddress += newAddress; // Convert from word address to byte address
      address = newAddress;
      verifySpace();
    }
    else if(ch == STK_UNIVERSAL) {
      // UNIVERSAL command is ignored
      getNch(4);
      putch(0x00);
    }
    /* Write memory, length is big endian and is in bytes */
    else if(ch == STK_PROG_PAGE) {
      // PROGRAM PAGE - we support flash programming only, not EEPROM
      uint8_t *bufPtr;
      //uint16_t addrPtr;

      getch();			/* getlen() */
      length = getch();
      getch();

	  // the original optiboot code erases memory before reading in the contents. this is faster but takes up more memory so I've taken it out.

      // read in page contents
      bufPtr = pagebuffer;
      do *bufPtr++ = getch();
      while (--length);

      // Read command terminator, start reply
      verifySpace();

	  // flash the chip
	  write_pagebuffer((uint16_t)(void*)address);
    }

    /* Read memory block mode, length is big endian.  */
    else if(ch == STK_READ_PAGE) {
      // READ PAGE - we only read flash
      getch();			/* getlen() */
      length = getch();
      getch();

      verifySpace();
#ifdef VIRTUAL_BOOT_PARTITION
      do {
        // Undo vector patch in bottom page so verify passes
        if (address == 0)       ch=rstVect & 0xff;
        else if (address == 1)  ch=rstVect >> 8;
        else if (address == 8)  ch=wdtVect & 0xff;
        else if (address == 9) ch=wdtVect >> 8;
        else ch = pgm_read_byte_near(address);
        address++;
        putch(ch);
      } while (--length);
#else
#ifdef __AVR_ATmega1280__
//      do putch(pgm_read_byte_near(address++));
//      while (--length);
      do {
        uint8_t result;
        __asm__ ("elpm %0,Z\n":"=r"(result):"z"(address));
        putch(result);
        address++;
      }
      while (--length);
#else
      do putch(pgm_read_byte_near(address++));
      while (--length);
#endif
#endif
    }

    /* Get device signature bytes  */
    else if(ch == STK_READ_SIGN) {
      // READ SIGN - return what Avrdude wants to hear
      verifySpace();
      putch(SIGNATURE_0);
      putch(SIGNATURE_1);
      putch(SIGNATURE_2);
    }
    else if (ch == 'Q') {
      // Adaboot no-wait mod
      watchdogConfig(WATCHDOG_16MS);
      verifySpace();
    }
    else {
    
      // This covers the response to commands like STK_ENTER_PROGMODE
      verifySpace();
    }

    putch(STK_OK);
  }
}

void mydelay(unsigned long delay) {
  // added by Jonne Valola 2014
  volatile unsigned long i = 0;
  for (i = 0; i < delay; i++) {
      __asm__ __volatile__ ("nop");
  }
}

void putch(char ch) {
#ifndef SOFT_UART
  while (!(UCSR0A & _BV(UDRE0)));
  UDR0 = ch;
#else
  __asm__ __volatile__ (
    "   com %[ch]\n" // ones complement, carry set
    "   sec\n"
    "1: brcc 2f\n"
    "   cbi %[uartPort],%[uartBit]\n"
    "   rjmp 3f\n"
    "2: sbi %[uartPort],%[uartBit]\n"
    "   nop\n"
    "3: rcall uartDelay\n"
    "   rcall uartDelay\n"
    "   lsr %[ch]\n"
    "   dec %[bitcnt]\n"
    "   brne 1b\n"
    :
    :
      [bitcnt] "d" (10),
      [ch] "r" (ch),
      [uartPort] "I" (_SFR_IO_ADDR(UART_PORT)),
      [uartBit] "I" (UART_TX_BIT)
    :
      "r25"
  );
#endif
}

/*void putstring(char * str)
{
	while (*str)
		putch(*str++);
} */


uint8_t getch(void) {
  uint8_t ch;

#ifdef LED_DATA_FLASH
#ifdef __AVR_ATmega8__
  //DEBUG FJA LED_PORT ^= _BV(LED);
#else
  //DEBUG FJA   LED_PIN |= _BV(LED);
#endif
#endif

#ifdef SOFT_UART
    __asm__ __volatile__ (
    "1: sbic  %[uartPin],%[uartBit]\n"  // Wait for start edge
    "   rjmp  1b\n"
    "   rcall uartDelay\n"          // Get to middle of start bit
    "2: rcall uartDelay\n"              // Wait 1 bit period
    "   rcall uartDelay\n"              // Wait 1 bit period
    "   clc\n"
    "   sbic  %[uartPin],%[uartBit]\n"
    "   sec\n"
    "   dec   %[bitCnt]\n"
    "   breq  3f\n"
    "   ror   %[ch]\n"
    "   rjmp  2b\n"
    "3:\n"
    :
      [ch] "=r" (ch)
    :
      [bitCnt] "d" (9),
      [uartPin] "I" (_SFR_IO_ADDR(UART_PIN)),
      [uartBit] "I" (UART_RX_BIT)
    :
      "r25"
);
#else
  while(!(UCSR0A & _BV(RXC0)))
    ;
  if (!(UCSR0A & _BV(FE0))) {
      /*
       * A Framing Error indicates (probably) that something is talking
       * to us at the wrong bit rate.  Assume that this is because it
       * expects to be talking to the application, and DON'T reset the
       * watchdog.  This should cause the bootloader to abort and run
       * the application "soon", if it keeps happening.  (Note that we
       * don't care that an invalid char is returned...)
       */
    watchdogReset();
  }
  
  ch = UDR0;
#endif

#ifdef LED_DATA_FLASH
#ifdef __AVR_ATmega8__
  //DEBUG FJA   LED_PORT ^= _BV(LED);
#else
  //DEBUG FJA   LED_PIN |= _BV(LED);
#endif
#endif

  return ch;
}

#ifdef SOFT_UART
// AVR350 equation: #define UART_B_VALUE (((F_CPU/BAUD_RATE)-23)/6)
// Adding 3 to numerator simulates nearest rounding for more accurate baud rates
#define UART_B_VALUE (((F_CPU/BAUD_RATE)-20)/6)
#if UART_B_VALUE > 255
#error Baud rate too slow for soft UART
#endif

void uartDelay() {
  __asm__ __volatile__ (
    "ldi r25,%[count]\n"
    "1:dec r25\n"
    "brne 1b\n"
    "ret\n"
    ::[count] "M" (UART_B_VALUE)
  );
}
#endif

void getNch(uint8_t count) {
  do getch(); while (--count);
  verifySpace();
}

void verifySpace() {
  if (getch() != CRC_EOP) {
    reboot();
  }
  putch(STK_INSYNC);
}

#if LED_START_FLASHES > 0
void flash_led(uint8_t count) {
  do {
    TCNT1 = -(F_CPU/(1024*16));
    TIFR1 = _BV(TOV1);
    while(!(TIFR1 & _BV(TOV1)));
#ifdef __AVR_ATmega8__
    //LED_PORT ^= _BV(LED);
#else
    //LED_PIN |= _BV(LED);
#endif
    watchdogReset();
  } while (--count);
}
#endif

// Watchdog functions. These are only safe with interrupts turned off.
void watchdogReset() {
  __asm__ __volatile__ (
    "wdr\n"
  );
}

void watchdogConfig(uint8_t x) {
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = x;
}

void appStart() {
  watchdogConfig(WATCHDOG_OFF);
  
  __asm__ __volatile__ (
#ifdef VIRTUAL_BOOT_PARTITION
    // Jump to WDT vector
    "ldi r30,4\n"
    "clr r31\n"
#else
    // Jump to RST vector
    "clr r30\n"
    "clr r31\n"
#endif
    "ijmp\n"
  );
}

void reboot()
{
	watchdogConfig(WATCHDOG_16MS);
	asm("sei");
	while (1);
}

#include <avr/pgmspace.h>

void load_loader()
{
	watchdogConfig(WATCHDOG_OFF);

	// silly, but it works
	static char filename[3];
	filename[0] = 'C';
	filename[1] = 'F';
	filename[2] = 'G';
    filename[3] = 0;
	load_game(filename);
}

// loads a game and reboots
void load_game(const char * filename)
{
	// don't want any interrupts corrupting our memory
	asm("cli");
	asm volatile ("clr __zero_reg__");

	// if we're being called by the application then the filename could be anywhere in SRAM.
	// push it to the stack so that we don't accidentally overwrite it
	memcpy(FILENAME_LOCATION, filename, 9);

	// reset the stack back to the top of ram, not including space for the filename.
	SP=RAMEND-9;

	load_file();
	reboot();
}
