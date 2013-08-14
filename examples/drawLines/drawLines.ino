/*  Demo of draw line's APP
    drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);
    drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
*/

#include <TFTv2.h>
#include <SPI.h>
void setup()
{
    Serial.begin(115200);

    Tft.begin(PE_1,PE_2,PE_3,PE_4);                // CS,DC,BL,RESET pin
    Tft.TFTinit();                                 // init TFT library
    Tft.backlight_on();                              //init TFT library

    Tft.drawLine(0,0,239,319,RED);              //start: (0, 0) end: (239, 319), color : RED
   
    Tft.drawVerticalLine(60,100,100,GREEN);     // Draw a vertical line
                                                // start: (60, 100) length: 100 color: green
                                            
    Tft.drawHorizontalLine(30,60,150,BLUE);     //Draw a horizontal line
                                                //start: (30, 60), high: 150, color: blue
}

void loop()
{
  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
