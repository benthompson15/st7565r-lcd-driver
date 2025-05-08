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
#include "lcd.h"

/**
 * @brief Initialize the SPI interface for communication with the LCD.
 * 
 * Configures the SPI pins and initializes the SPI controller in Master mode
 * with the appropriate settings (clock polarity, phase, etc.).
 */
void lcd_spi_init() {
    // Configure DATA_CMD_PIN () as output for Data/Command selector
    // Configure RST_PIN () as output for Reset
    DDRA |= (1 << DATA_CMD_PIN) | (1 << RST_PIN);
    // Configure CS_PIN () as output for Chip Select
    DDRC |= (1 << CS_PIN);        // Set C0 as output
    // Configure SPI Outputs
    DDRB |= (1 << LCD_SS) | (1 << LCD_SCK) | (1 << LCD_MOSI);
    // Configure SPI as Master, enable SPI
    // Set SPI control register (SPCR) to enable SPI as Master mode, set clock frequency fosc/16
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA);
    // Clear SPI status register (SPSR)
    SPSR = 0x00;
}

/**
 * @brief Write a data byte to the LCD.
 * 
 * This function writes a byte of data to the LCD in data mode.
 * 
 * @param data The byte of data to be sent to the LCD.
 */
void lcd_write_data(uint8_t data) {
    // Set Data/Command pin high to indicate data transfer
    DATA_CMD_PORT |= (1 << DATA_CMD_PIN);  // Data mode (A0)
    // Set CS pin low to enable LCD chip select
    CS_PORT &= ~(1 << CS_PIN);

    // Send data via SPI
    SPI_SEND(data);

    // Set CS pin high to disable LCD chip select
    CS_PORT |= (1 << CS_PIN);
}

/**
 * @brief Write a command byte to the LCD.
 * 
 * This function sends a byte of command data to the LCD in command mode.
 * 
 * @param cmd The command byte to be sent to the LCD.
 */
void lcd_write_cmd(uint8_t cmd) {
    // Clear Data/Command pin to indicate command mode
    DATA_CMD_PORT &= ~(1 << DATA_CMD_PIN);  // Command mode (A0)
    // Set CS pin low to enable LCD chip select
    CS_PORT &= ~(1 << CS_PIN);

    // Send command via SPI
    SPI_SEND(cmd);

    // Set CS pin high to disable LCD chip select
    CS_PORT |= (1 << CS_PIN);

    // Optional delay to ensure command is processed
    _delay_us(25);
}

/**
 * @brief Initialize the LCD.
 * 
 * This function performs the LCD initialization sequence to prepare the display
 * for operation, including setting power control, display configuration, and 
 * enabling the display.
 */
void lcd_init() {
    // Delay for power to stabilize
    _delay_ms(10);
    RST_PORT |= (1 << RST_PIN);  // Reset the LCD
    _delay_ms(10);

    // Initialization sequence
    lcd_write_cmd(0xA0);  // ADC Select
    lcd_write_cmd(0xAE);  // Display OFF
    lcd_write_cmd(0xC8);  // COM Output Normal
    lcd_write_cmd(0xA3);  // 1/7 Bias
    lcd_write_cmd(0x2F);  // Power Control Set
    lcd_write_cmd(0x26);  // Internal Resistor Ratio
    lcd_write_cmd(0x81);  // Electronic Volume Mode
    lcd_write_cmd(0x07);  // Electronic Volume Level

    lcd_write_cmd(0xAF);  // Display ON
    lcd_write_cmd(0xA4);  // Display All Points Normal

    // Set initial display position
    lcd_write_cmd(0x40);
    lcd_write_cmd(0xB0);
    lcd_write_cmd(0x10);
    lcd_write_cmd(0x00);

    // Dummy data to initialize display
    lcd_write_data(0xFF);
    lcd_write_data(0xFF);
    lcd_write_data(0xFF);
    lcd_write_data(0xFF);
    lcd_write_data(0xFF);
}

/**
 * @brief Set the current cursor position for subsequent data writes.
 * 
 *  * @note The LCD supports up to 21 characters per line (128 pixels / 6 pixels per character).
 *  * @note This function does not check if the string fits on the screen; it will 
 *       continue writing past the edge of the display if the string is too long.
 *       Ensure the string length and starting column do not exceed display limits.
 * 
 * @param page   Page address (0-7)
 * @param column Column address (0-121), accounting for 6-pixel wide characters
 *               (5 pixels for the character + 1 for spacing)
 */
void lcd_set_cursor(uint8_t page, uint8_t column) {
    lcd_write_cmd(0xB0 | (page & 0x0F));          // Page address
    lcd_write_cmd(0x10 | ((column >> 4) & 0x0F)); // Column high nibble
    lcd_write_cmd(column & 0x0F);                 // Column low nibble
}

/**
 * @brief Clear the entire LCD display.
 * 
 * This function clears the LCD display by writing zeros to all pages and columns,
 * effectively turning off all pixels.
 */
void lcd_clear() {
    // Loop through all pages (0 to LCD_TOTAL_PAGES)
    // if you dont clear page = 8 you can get random pixels at top of lcd
    for (uint8_t page = 0; page <= LCD_TOTAL_PAGES; page++) {
        lcd_set_cursor(page, 0x00); // Start from the first column (0x00)
        
        // Clear each column in the page
        for (uint8_t i = 0; i < LCD_TOTAL_COLUMNS; i++) {
            lcd_write_data(0x00); // Write zeros to clear the line
        }
    }
}

/**
 * @brief Print a single character to the LCD.
 * 
 * This function writes a single ASCII character to the LCD at the current cursor
 * position, using 6-byte data (one byte per column of the character).
 * 
 *  * @note The LCD supports up to 21 characters per line (128 pixels / 6 pixels per character).
 *  * @note This function does not check if the string fits on the screen; it will 
 *       continue writing past the edge of the display if the string is too long.
 *       Ensure the string length and starting column do not exceed display limits.
 * 
 * @param ascii_char The ASCII value of the character to print.
 */
void lcd_print_char(uint8_t ascii_char) {
    uint8_t index = ascii_char - 0x20;  // Calculate the index (starting from space)
    const uint8_t *char_data = Char_Table[index];  // Get pointer to the character data
    
    // Send 6 bytes for the character to the LCD
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t byte = pgm_read_byte(&(char_data[i]));  // Read character byte from program memory
        lcd_write_data(byte);  // Send the byte to the LCD
    }
}

/**
 * @brief Print a string of characters to the LCD.
 * 
 * This function prints a null-terminated string of characters starting from the 
 * specified page and column, using 6-byte character data for each character.
 * 
 *  * @note The LCD supports up to 21 characters per line (128 pixels / 6 pixels per character).
 *  * @note This function does not check if the string fits on the screen; it will 
 *       continue writing past the edge of the display if the string is too long.
 *       Ensure the string length and starting column do not exceed display limits.
 * 
 * @param page_number The starting page (0-7) for the string to be printed.
 * @param column      The starting column (0-121) for the string to be printed.
 * @param print_str   Pointer to the null-terminated string to be printed.
 */
void lcd_print_str(uint8_t page, uint8_t column, const char* print_str) {
    uint8_t c;
    lcd_set_cursor(page, column);
    // Loop through each character in the string
    while ((c = pgm_read_byte(print_str++)) != '\0') {
        // Initialize the LCD for the current character position
        // Print the character on the LCD
        lcd_print_char(c);
    }
}