/*
 2012 Copyright (c) Seeed Technology Inc.

 Authors: Albert.Miao & Loovee, 
 Visweswara R (with initializtion code from TFT vendor)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/



#ifndef TFTv2_h
#define TFTv2_h

#include "Energia.h"
#include "SPI.h"
#include <stdint.h>

#if defined(__LM4F120H5QR__)
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#else
#error Board not supported.
#endif

//Basic Colors
#define RED		0xf800
#define GREEN		0x07e0
#define BLUE		0x001f
#define BLACK		0x0000
#define YELLOW		0xffe0
#define WHITE		0xffff

//Other Colors
#define CYAN		0x07ff	
#define BRIGHT_RED	0xf810	
#define GRAY1		0x8410  
#define GRAY2		0x4208  

//TFT resolution 240*320
#define MIN_X		0
#define MIN_Y		0
#define MAX_X		239
#define MAX_Y		319


extern uint8_t simpleFont[][8];

class TFT
{
public:
	void begin(uint8_t csPin, uint8_t dcPin, uint8_t blPin, uint8_t rstPin);
	void TFTinit (void);
	void backlight_on (void);
	void backlight_off (void);
	void setCol(uint16_t StartCol,uint16_t EndCol);
	void setPage(uint16_t StartPage,uint16_t EndPage);
	void setXY(uint16_t poX, uint16_t poY);
	void setPixel(uint16_t poX, uint16_t poY,uint16_t color);
	void sendCMD(uint8_t index);
	void WRITE_Package(uint16_t *data,uint8_t howmany);
	void WRITE_DATA(uint8_t data);
	void sendData(uint16_t data);
	uint8_t Read_Register(uint8_t Addr,uint8_t xParameter);
	void fillScreen(uint16_t XL,uint16_t XR,uint16_t YU,uint16_t YD,uint16_t color);
	void fillScreen(void);
	uint8_t readID(void);

	void drawChar(uint8_t ascii,uint16_t poX, uint16_t poY,uint16_t size, uint16_t fgcolor,uint16_t bgcolor);
	void drawString(char *string,uint16_t poX, uint16_t poY,uint16_t size,uint16_t fgcolor, uint16_t bgcolor);
	void fillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);

	void drawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color);
	void drawVerticalLine(uint16_t poX, uint16_t poY,uint16_t length,uint16_t color);
	void drawHorizontalLine(uint16_t poX, uint16_t poY,uint16_t length,uint16_t color);
	void drawRectangle(uint16_t poX, uint16_t poY, uint16_t length,uint16_t width,uint16_t color);

	void drawCircle(int poX, int poY, int r,uint16_t color);
	void fillCircle(int poX, int poY, int r,uint16_t color);

	void drawTraingle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color);
	uint8_t drawNumber(long long_num,uint16_t poX, uint16_t poY,uint16_t size,uint16_t fgcolor, uint16_t bgcolor);
	uint8_t drawFloat(float floatNumber,uint8_t decimal,uint16_t poX, uint16_t poY,uint16_t size,uint16_t fgcolor, uint16_t bgcolor);

  private:

	uint8_t _csPin, _dcPin, _blPin, _rstPin, _port_cs, _bit_cs, _port_dc, _bit_dc, _port_bl, _bit_bl, _port_rst, _bit_rst;

};

extern TFT Tft;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
