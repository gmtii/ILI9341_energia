/*  Draw Boxes - Demonstrate drawRectangle and fillRectangle
    fillScreen(INT16U XL,INT16U XR,INT16U YU,INT16U YD,INT16U color);
    fillRectangle(INT16U poX, INT16U poY, INT16U length, INT16U width, INT16U color);
    drawRectangle(INT16U poX, INT16U poY, INT16U length,INT16U width,INT16U color);
*/
#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

#include <TFTv2.h>
#include <SPI.h>
void setup()
{
    Serial.begin(115200);

    Tft.begin(PE_1,PE_2,PE_3,PE_4);                // CS,DC,BL,RESET pin
    Tft.TFTinit();                                 // init TFT library
    Tft.backlight_on(); 

    Tft.fillScreen(80, 160, 50, 150,RED);
    Tft.fillRectangle(30, 120, 100,65,YELLOW);
    Tft.drawRectangle(100, 170, 120,60,BLUE);
}

void loop()
{
  
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
