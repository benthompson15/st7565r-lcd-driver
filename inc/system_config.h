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
#ifndef SYSTEM_CONFIG_H
#define	SYSTEM_CONFIG_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#define F_CPU 8000000UL  // 8 MHz

// LCD PAGES and COLUMNS
#define LCD_TOTAL_PAGES 8
#define LCD_TOTAL_COLUMNS 128

//Port Configuration Section

// Define the ports for various functions
#define DATA_CMD_PORT     PORTA   // Port for Data/Command pin (A0)
#define CS_PORT           PORTC   // Port for Chip Select pin
#define RST_PORT          PORTA   // Port for Reset pin
#define SPI_PORT          PORTB   // Port for SPI

// Define the pin mappings
#define DATA_CMD_PIN      PINA0   // Pin for A0 (Data/Command selector)
#define CS_PIN            PINC0   // Pin for Chip Select
#define RST_PIN           PINA1   // Pin for Reset
#define LCD_SS            PINB0   // SS (SPI Slave Select input)
#define LCD_SCK           PINB1   // SCK (SPI Bus Serial Clock)
#define LCD_MOSI          PINB2   // MOSI (SPI Bus Master Output/Slave Input)
#define LCD_MISO          PINB3   // MISO (SPI Bus Master Input/Slave Output)

#endif	/* SYSTEM_CONFIG_H */

