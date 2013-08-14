// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <TFTv2.h>
#include <SPI.h>
void setup()
{
    Serial.begin(115200);

    Tft.begin(PE_1,PE_2,PE_3,PE_4);                // CS,DC,BL,RESET pin
    Tft.TFTinit();                                 // init TFT library
    Tft.backlight_on();                                         // init TFT library
}

void loop()
{
    for(int r=0;r<115;r=r+2)                            //set r : 0--115
    {
        Tft.drawCircle(119,160,r,random(0xFFFF));       //draw circle, center:(119, 160), color: random
    }
    delay(10);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
