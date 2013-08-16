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
 Foundation, Inc.,51 Franklin St,Fifth Floor, Boston, MA 02110-1301 USA

*/
// Board check
#if defined(__LM4F120H5QR__)
#else
#error Board not supported.
#endif

#include <TFTv2.h>
#include "Energia.h"
#include "SPI.h"
#include "FastDigitalWrite.h"

#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8

#define portOutputRegister(x) (regtype)portBASERegister(x)
#define cbi_macro(reg, mask) GPIOPinWrite(reg, mask, 0)
#define sbi_macro(reg, mask) GPIOPinWrite(reg, mask, mask)

void TFT::sendCMD(uint8_t index)
{
    digitalWrite(_dcPin,   LOW);
    cbi_macro(_port_cs, _bit_cs);   
    SPI.transfer(index);
    sbi_macro(_port_cs, _bit_cs); 
}

void TFT::WRITE_DATA(uint8_t data)
{
    digitalWrite(_dcPin,   HIGH);
    cbi_macro(_port_cs, _bit_cs); 
    SPI.transfer(data);
    sbi_macro(_port_cs, _bit_cs); 
}

void TFT::sendData(uint16_t data)
{
    uint8_t data1 = data>>8;
    uint8_t data2 = data&0xff;
    digitalWrite(_dcPin,   HIGH);
    cbi_macro(_port_cs, _bit_cs); 
    SPI.transfer(data1);
    SPI.transfer(data2);
    sbi_macro(_port_cs, _bit_cs); 
}

void TFT::WRITE_Package(uint16_t *data, uint8_t howmany)
{
    uint16_t  data1 = 0;
    uint8_t   data2 = 0;

    digitalWrite(_dcPin,   HIGH);
    cbi_macro(_port_cs, _bit_cs); 
    uint8_t count=0;
    for(count=0;count<howmany;count++)
    {
        data1 = data[count]>>8;
        data2 = data[count]&0xff;
        SPI.transfer(data1);
        SPI.transfer(data2);
    }
    sbi_macro(_port_cs, _bit_cs); 
}

uint8_t TFT::Read_Register(uint8_t Addr, uint8_t xParameter)
{
    uint8_t data=0;
    sendCMD(0xd9);                                                      /* ext command                  */
    WRITE_DATA(0x10+xParameter);                                        /* 0x11 is the first Parameter  */
    digitalWrite(_dcPin,   LOW);
    cbi_macro(_port_cs, _bit_cs); 
    SPI.transfer(Addr);
    digitalWrite(_dcPin,   HIGH);
    data = SPI.transfer(0);
    sbi_macro(_port_cs, _bit_cs); 
    return data;
}

void TFT::begin(uint8_t csPin, uint8_t dcPin, uint8_t blPin, uint8_t rstPin)
{
	_csPin=csPin;
	_dcPin=dcPin;
	_blPin=blPin;
	_rstPin=rstPin;


	_port_cs   = portOutputRegister(digitalPinToPort(_csPin));
	_bit_cs    = digitalPinToBitMask(_csPin);


}

void TFT::backlight_on(void)
{
	digitalWrite(_blPin,   HIGH);
}

void TFT::backlight_off(void)
{
	digitalWrite(_blPin,   LOW);
}


void TFT::TFTinit (void)
{

	//pinMode(_csPin, OUTPUT);
	//digitalWrite(_csPin, HIGH);

	pinMode(_csPin, OUTPUT);
        sbi_macro(_port_cs, _bit_cs); 

	pinMode(_dcPin, OUTPUT);
	digitalWrite(_dcPin, LOW);

	pinMode(_blPin, OUTPUT);
	digitalWrite(_blPin, LOW);

	pinMode(_rstPin, OUTPUT);
	digitalWrite(_rstPin, LOW);

	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	//SPI.setClockDivider(SPI_CLOCK_DIV4);
	SPI.setBitOrder(MSBFIRST); 

	SPI.transfer(0);  // Strawman transfer, fixes USCI issue on G2553

    sbi_macro(_port_cs, _bit_cs); 
    digitalWrite(_dcPin,   HIGH);
    uint8_t i=0, TFTDriver=0;

	digitalWrite(_rstPin,   LOW);
	delay(10);
	digitalWrite(_rstPin,   HIGH);

    for(i=0;i<3;i++)
    {
        TFTDriver = readID();
    }

	sendCMD(0xCB);  
	WRITE_DATA(0x39); 
	WRITE_DATA(0x2C); 
	WRITE_DATA(0x00); 
	WRITE_DATA(0x34); 
	WRITE_DATA(0x02); 

	sendCMD(0xCF);  
	WRITE_DATA(0x00); 
	WRITE_DATA(0XC1); 
	WRITE_DATA(0X30); 

	sendCMD(0xE8);  
	WRITE_DATA(0x85); 
	WRITE_DATA(0x00); 
	WRITE_DATA(0x78); 

	sendCMD(0xEA);  
	WRITE_DATA(0x00); 
	WRITE_DATA(0x00); 

	sendCMD(0xED);  
	WRITE_DATA(0x64); 
	WRITE_DATA(0x03); 
	WRITE_DATA(0X12); 
	WRITE_DATA(0X81); 

	sendCMD(0xF7);  
	WRITE_DATA(0x20); 

	sendCMD(0xC0);    	//Power control 
	WRITE_DATA(0x23);   	//VRH[5:0] 

	sendCMD(0xC1);    	//Power control 
	WRITE_DATA(0x10);   	//SAP[2:0];BT[3:0] 

	sendCMD(0xC5);    	//VCM control 
	WRITE_DATA(0x3e);   	//Contrast
	WRITE_DATA(0x28); 

	sendCMD(0xC7);    	//VCM control2 
	WRITE_DATA(0x86);  	 //--

	sendCMD(0x36);    	// Memory Access Control 
	WRITE_DATA(0x48);  	//C8	   //48 68绔栧睆//28 E8 妯睆

	sendCMD(0x3A);    
	WRITE_DATA(0x55); 

	sendCMD(0xB1);    
	WRITE_DATA(0x00);  
	WRITE_DATA(0x18); 

	sendCMD(0xB6);    	// Display Function Control 
	WRITE_DATA(0x08); 
	WRITE_DATA(0x82);
	WRITE_DATA(0x27);  
 
	sendCMD(0xF2);    	// 3Gamma Function Disable 
	WRITE_DATA(0x00); 

	sendCMD(0x26);    	//Gamma curve selected 
	WRITE_DATA(0x01); 

	sendCMD(0xE0);    	//Set Gamma 
	WRITE_DATA(0x0F); 
	WRITE_DATA(0x31); 
	WRITE_DATA(0x2B); 
	WRITE_DATA(0x0C); 
	WRITE_DATA(0x0E); 
	WRITE_DATA(0x08); 
	WRITE_DATA(0x4E); 
	WRITE_DATA(0xF1); 
	WRITE_DATA(0x37); 
	WRITE_DATA(0x07); 
	WRITE_DATA(0x10); 
	WRITE_DATA(0x03); 
	WRITE_DATA(0x0E); 
	WRITE_DATA(0x09); 
	WRITE_DATA(0x00); 

	sendCMD(0XE1);    	//Set Gamma 
	WRITE_DATA(0x00); 
	WRITE_DATA(0x0E); 
	WRITE_DATA(0x14); 
	WRITE_DATA(0x03); 
	WRITE_DATA(0x11); 
	WRITE_DATA(0x07); 
	WRITE_DATA(0x31); 
	WRITE_DATA(0xC1); 
	WRITE_DATA(0x48); 
	WRITE_DATA(0x08); 
	WRITE_DATA(0x0F); 
	WRITE_DATA(0x0C); 
	WRITE_DATA(0x31); 
	WRITE_DATA(0x36); 
	WRITE_DATA(0x0F); 

	sendCMD(0x11);    	//Exit Sleep 
	delay(120); 

	sendCMD(0x29);    //Display on 
	sendCMD(0x2c);   
	fillScreen();
}

uint8_t TFT::readID(void)
{
    uint8_t i=0;
    uint8_t data[3] ;
    uint8_t ID[3] = {0x00, 0x93, 0x41};
    uint8_t ToF=1;
    for(i=0;i<3;i++)
    {
        data[i]=Read_Register(0xd3,i+1);
        if(data[i] != ID[i])
        {
            ToF=0;
        }
    }
    if(!ToF)                                                            /* data!=ID                     */
    {
        Serial.print("Read TFT ID failed, ID should be 0x09341, but read ID = 0x");
        for(i=0;i<3;i++)
        {
            Serial.print(data[i],HEX);
        }
        Serial.println();
    }
    return ToF;
}

void TFT::setCol(uint16_t StartCol,uint16_t EndCol)
{
    sendCMD(0x2A);                                                      /* Column Command address       */
    sendData(StartCol);
    sendData(EndCol);
}

void TFT::setPage(uint16_t StartPage,uint16_t EndPage)
{
    sendCMD(0x2B);                                                      /* Column Command address       */
    sendData(StartPage);
    sendData(EndPage);
}

void TFT::fillScreen(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color)
{
    unsigned long  XY=0;
    unsigned long i=0;

    if(XL > XR)
    {
        XL = XL^XR;
        XR = XL^XR;
        XL = XL^XR;
    }
    if(YU > YD)
    {
        YU = YU^YD;
        YD = YU^YD;
        YU = YU^YD;
    }
    XL = constrain(XL, MIN_X,MAX_X);
    XR = constrain(XR, MIN_X,MAX_X);
    YU = constrain(YU, MIN_Y,MAX_Y);
    YD = constrain(YD, MIN_Y,MAX_Y);

    XY = (XR-XL+1);
    XY = XY*(YD-YU+1);

    Tft.setCol(XL,XR);
    Tft.setPage(YU, YD);
    Tft.sendCMD(0x2c);                                                  /* start to write to display ra */
                                                                        /* m                            */

    digitalWrite(_dcPin,   HIGH);
    cbi_macro(_port_cs, _bit_cs); 

    uint8_t Hcolor = color>>8;
    uint8_t Lcolor = color&0xff;
    for(i=0; i < XY; i++)
    {
        SPI.transfer(Hcolor);
        SPI.transfer(Lcolor);
    }

    sbi_macro(_port_cs, _bit_cs); 
}

void TFT::fillScreen(void)
{
    Tft.setCol(0, 239);
    Tft.setPage(0, 319);
    Tft.sendCMD(0x2c);                                                  /* start to write to display ra */
                                                                        /* m                            */

    digitalWrite(_dcPin,   HIGH);
    cbi_macro(_port_cs, _bit_cs); 
    for(uint16_t i=0; i<38400; i++)
    {
        SPI.transfer(0);
        SPI.transfer(0);
        SPI.transfer(0);
        SPI.transfer(0);
    }
    sbi_macro(_port_cs, _bit_cs); 
}


void TFT::setXY(uint16_t poX, uint16_t poY)
{
    setCol(poX, poX);
    setPage(poY, poY);
    sendCMD(0x2c);
}

void TFT::setPixel(uint16_t poX, uint16_t poY,uint16_t color)
{
    setXY(poX, poY);
    sendData(color);
}

void TFT::drawChar( uint8_t ascii, uint16_t poX, uint16_t poY,uint16_t size, uint16_t fgcolor, uint16_t bgcolor)
{

    //fillRectangle(poX, poY, poX+FONT_X*size, poY+FONT_Y*size, BLACK);


    if((ascii>=32)&&(ascii<=127))
    {
        ;
    }
    else
    {
        ascii = '?'-32;
    }
    for (int i =0; i<FONT_X; i++ ) {
        uint8_t temp = simpleFont[ascii-0x20][i];
        for(uint8_t f=0;f<8;f++)
        {
            if((temp>>f)&0x01)
            {
                fillRectangle(poX+i*size, poY+f*size, size, size, fgcolor);
            }
	    else
		fillRectangle(poX+i*size, poY+f*size, size, size, bgcolor);

        }

    }
}

void TFT::drawString(char *string,uint16_t poX, uint16_t poY, uint16_t size,uint16_t fgcolor, uint16_t bgcolor)
{
    while(*string)
    {
        drawChar(*string, poX, poY, size, fgcolor, bgcolor);
        *string++;

        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                     /* Move cursor right            */
        }
    }
}

//fillRectangle(poX+i*size, poY+f*size, size, size, fgcolor);
void TFT::fillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color)
{
    fillScreen(poX, poX+length, poY, poY+width, color);
}

void  TFT::drawHorizontalLine( uint16_t poX, uint16_t poY,
uint16_t length,uint16_t color)
{
    setCol(poX,poX + length);
    setPage(poY,poY);
    sendCMD(0x2c);
    for(int i=0; i<length; i++)
    sendData(color);
}

void TFT::drawLine( uint16_t x0,uint16_t y0,uint16_t x1, uint16_t y1,uint16_t color)
{

    int x = x1-x0;
    int y = y1-y0;
    int dx = abs(x), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;                                                /* error value e_xy             */
    for (;;){                                                           /* loop                         */
        setPixel(x0,y0,color);
        e2 = 2*err;
        if (e2 >= dy) {                                                 /* e_xy+e_x > 0                 */
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) {                                                 /* e_xy+e_y < 0                 */
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }

}

void TFT::drawVerticalLine( uint16_t poX, uint16_t poY, uint16_t length,uint16_t color)
{
    setCol(poX,poX);
    setPage(poY,poY+length);
    sendCMD(0x2c);
    for(int i=0; i<length; i++)
    sendData(color);
}

void TFT::drawRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width,uint16_t color)
{
    drawHorizontalLine(poX, poY, length, color);
    drawHorizontalLine(poX, poY+width, length, color);
    drawVerticalLine(poX, poY, width,color);
    drawVerticalLine(poX + length, poY, width,color);

}

void TFT::drawCircle(int poX, int poY, int r,uint16_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        setPixel(poX-x, poY+y,color);
        setPixel(poX+x, poY+y,color);
        setPixel(poX+x, poY-y,color);
        setPixel(poX-x, poY-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}

void TFT::fillCircle(int poX, int poY, int r,uint16_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {

        drawVerticalLine(poX-x, poY-y, 2*y, color);
        drawVerticalLine(poX+x, poY-y, 2*y, color);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);

}

void TFT::drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color)
{
    drawLine(poX1, poY1, poX2, poY2,color);
    drawLine(poX1, poY1, poX3, poY3,color);
    drawLine(poX2, poY2, poX3, poY3,color);
}

uint8_t TFT::drawNumber(long long_num,uint16_t poX, uint16_t poY,uint16_t size,uint16_t fgcolor, uint16_t bgcolor)
{
    uint8_t char_buffer[10] = "";
    uint8_t i = 0;
    uint8_t f = 0;


    if (long_num < 0)
    {
        f=1;
        drawChar('-',poX, poY, size, fgcolor, bgcolor);
        long_num = -long_num;
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                     /* Move cursor right            */
        }
    }
    else if (long_num == 0)
    {
        f=1;
        drawChar('0',poX, poY, size, fgcolor, bgcolor);
        return f;
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                     /* Move cursor right            */
        }
    }


    while (long_num > 0)
    {
        char_buffer[i++] = long_num % 10;
        long_num /= 10;
    }

    f = f+i;
    for(; i > 0; i--)
    {
        drawChar('0'+ char_buffer[i - 1],poX, poY, size, fgcolor, bgcolor);
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
        }
    }
    return f;
}

uint8_t TFT::drawFloat(float floatNumber,uint8_t decimal,uint16_t poX, uint16_t poY,uint16_t size,uint16_t fgcolor, uint16_t bgcolor)
{
    uint16_t temp=0;
    float decy=0.0;
    float rounding = 0.5;
    uint8_t f=0;
    if(floatNumber<0.0)
    {
        drawChar('-',poX, poY, size, fgcolor, bgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
        }
        f =1;
    }
    for (uint8_t i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = (uint16_t)floatNumber;
    uint8_t howlong=drawNumber(temp,poX, poY, size, fgcolor, bgcolor);
    f += howlong;
    if((poX+8*size*howlong) < MAX_X)
    {
        poX+=FONT_SPACE*size*howlong;                                   /* Move cursor right            */
    }

    if(decimal>0)
    {
        drawChar('.',poX, poY, size, fgcolor, bgcolor);
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
        }
        f +=1;
    }
    decy = floatNumber-temp;                                            /* decimal part,  4             */
    for(uint8_t i=0;i<decimal;i++)                                      
    {
        decy *=10;                                                      /* for the next decimal         */
        temp = decy;                                                    /* get the decimal              */
        drawNumber(temp,poX, poY, size, fgcolor, bgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
        }
        decy -= temp;
    }
    f +=decimal;
    return f;
}


TFT Tft=TFT();
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
