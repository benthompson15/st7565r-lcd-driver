# ST7565R LCD Driver

This project provides a driver for controlling an LCD with the **ST7565R** controller using SPI.
This code is written specifically for the AVR architecture and tested on the ATmega128A microcontroller. 
The driver allows for basic operations like writing commands and data, initializing the display, and printing characters and strings from `PROGMEM`.

## Requirements

- AVR toolchain (e.g., `avr-gcc`, `avr-libc`)
- AVR microcontroller (tested on ATmega128A)

## Features

- **SPI Communication**: Uses SPI for data transfer.
- **LCD Initialization**: Includes standard initialization for the ST7565R display.
- **Print Functions**: Print individual characters and strings from `PROGMEM` to the display.

## File Structure

- **`lcd.c`**: Contains the driver code for interacting with the LCD.
- **`lcd.h`**: Header file with function prototypes, constants, and the character lookup table (Char_Table).
- **`config.h`**: Configuration file for defining pin mappings and ports.
- **`main.c`**: Example usage of the driver, prints all possible characters.

## Functions

### `lcd_spi_init()`
Initializes the SPI interface for communication with the LCD.

### `lcd_write_data(uint8_t data)`
Sends a data byte to the LCD.

### `lcd_write_cmd(uint8_t cmd)`
Sends a command byte to the LCD.

### `lcd_init()`
Initializes the LCD with the required settings.

### `lcd_set_cursor(uint8_t page_number, uint8_t column)`
Sets the display to the specified page and column for writing.

### `lcd_clear()`
Clears the entire display.

### `lcd_print_char(uint8_t ascii_char)`
Prints a single character to the LCD.

### `lcd_print_str(uint8_t page_number, uint8_t column, const char* print_str)`
Prints a string of characters to the LCD from `PROGMEM`.

## Example Usage

The following should be printed to the LCD:

```
LCD TEST v1
 !"#$%&'()*+,-./
0123456789
:;<=>?@[\\]^_`
ABCDEFGHIJKLMNOPQRSTU
VWXYZabcdefghijklmnop
qrstuvwxyz
{|}-><-
```

This demonstrates printing all characters defined in the font table (Char_Table) in lcd.h:

