/*
 * MIT License
 *
 * Copyright (c) 2025
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */
#include <xc.h>
#include "system_config.h"
#include "lcd.h"

// ----------------------------------------------------------------------------
// Fuse Configuration for ATmega128A
// ----------------------------------------------------------------------------
//
// LOW FUSE BYTE = 0xA4
//  Bit 7: BODLEVEL = 1 ? BOD trigger level = 2.7V (programmed by EXTENDED fuse below)
//  Bit 6: BODEN    = 0 ? Brown-out detector enabled
//  Bit 5: SUT1     = 1
//  Bit 4: SUT0     = 0 ? Start-up time: 64 ms (6 CK + 64 ms)
//  Bit 3: CKSEL3   = 0
//  Bit 2: CKSEL2   = 1
//  Bit 1: CKSEL1   = 0
//  Bit 0: CKSEL0   = 0 ? CKSEL = 0100 ? Internal RC Oscillator @ 8 MHz

// HIGH FUSE BYTE = 0xBF
//  Bit 7: OCDEN    = 1 ? On-Chip Debug disabled (unprogrammed)
//  Bit 6: JTAGEN   = 0 ? JTAG enabled (programmed; disable if not used to reclaim pins)
//  Bit 5: SPIEN    = 0 ? SPI programming enabled (must stay programmed for ISP)
//  Bit 4: CKOPT    = 1 ? No effect for internal RC
//  Bit 3: EESAVE   = 1 ? EEPROM preserved during chip erase (unprogrammed = not preserved)
//  Bit 2: BOOTSZ1  = 1
//  Bit 1: BOOTSZ0  = 1 ? Boot size = 512 words (start at 0xFE00)
//  Bit 0: BOOTRST  = 1 ? Reset vector points to application section

// EXTENDED FUSE BYTE = 0xFF
//  Bit 7: BODLEVEL2 = 1
//  Bit 6: BODLEVEL1 = 1
//  Bit 5: BODLEVEL0 = 1 ? BODLEVEL = 111 ? Brown-out level = 2.7V
//  Bit 4-1: Reserved = 1
//  Bit 0: WDTON     = 1 ? Watchdog Timer NOT always on (software controlled)
// ----------------------------------------------------------------------------
FUSES = {
	.low = 0xA4,
	.high = 0xBF, 
	.extended = 0xFF, 
};
// ----------------------------------------------------------------------------
// Lock Bits Configuration for ATmega128A
// ----------------------------------------------------------------------------
// LOCKBITS = 0xFF
//  Bit 7: Reserved   = 1 (unprogrammed)
//  Bit 6: Reserved   = 1 (unprogrammed)
//  Bit 5: BLB12      = 1
//  Bit 4: BLB11      = 1 ? Boot loader section: no restrictions (SPM and LPM allowed)
//  Bit 3: BLB02      = 1
//  Bit 2: BLB01      = 1 ? Application section: no restrictions (SPM and LPM allowed)
//  Bit 1: LB2        = 1
//  Bit 0: LB1        = 1 ? No memory lock features enabled
// ----------------------------------------------------------------------------
LOCKBITS = 0xFF;

// define strings
const char label[]     PROGMEM = "LCD TEST v1";
const char special1[]  PROGMEM = " !\"#$%&'()*+,-./";
const char digits[]    PROGMEM = "0123456789";
const char special2[]  PROGMEM = ":;<=>?@[\\]^_`";
const char alphabet1[] PROGMEM = "ABCDEFGHIJKLMNOPQRSTU";
const char alphabet2[] PROGMEM = "VWXYZabcdefghijklmnop";
const char alphabet3[] PROGMEM = "qrstuvwxyz";
const char special3[]  PROGMEM = "{|}~"; //~ is ->
const char lastSpecial = 0x7F; // 0x7f "DEL" is <-

void init(void) {
    //Clear Interrupt / Disable Interrupts
    cli();

    // SPI INIT
    lcd_spi_init();
    // Initialize the LCD
    lcd_init();
    
    //Enabled interrupts
    sei();
}

int main(void) {
    
    // Initialize hardware
    init();

    // Clear the LCD
    lcd_clear();

    // Print example strings to each line of the display
    lcd_print_str(0,0,label);
    lcd_print_str(1,0,special1);
    lcd_print_str(2,0,digits);
    lcd_print_str(3,0,special2);
    lcd_print_str(4,0,alphabet1);
    lcd_print_str(5,0,alphabet2);
    lcd_print_str(6,0,alphabet3);
    lcd_print_str(7,0,special3);
    //lcd_print_char will print at the end of last lcd_print_str 
    lcd_print_char(lastSpecial); // Final character in font table
    // Main loop
    while (1) {
        // Run tests here
        _delay_ms(1000);  // Delay to let you observe the screen
    }
    
    return 0;
}
