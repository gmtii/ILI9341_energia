/*  Demo of draw circle's APP
    drawCircle(int poX, int poY, int r,INT16U color);
    fillCircle(int poX, int poY, int r,INT16U color);
*/

#include <TFTv2.h>
#include <SPI.h>
void setup()
{
    Serial.begin(115200);

    Tft.begin(PE_1,PE_2,PE_3,PE_4);                // CS,DC,BL,RESET pin
    Tft.TFTinit();                                 // init TFT library
    Tft.backlight_on();                                      //init TFT library             

    Tft.drawCircle(100, 100, 30,YELLOW);                //center: (100, 100), r = 30 ,color : YELLOW              
    
    Tft.drawCircle(100, 200, 40,CYAN);                  // center: (100, 200), r = 10 ,color : CYAN  
    
    Tft.fillCircle(200, 100, 30,RED);                   //center: (200, 100), r = 30 ,color : RED    
    
    Tft.fillCircle(200, 200, 30,BLUE);                  //center: (200, 200), r = 30 ,color : BLUE                 
}

void loop()
{

}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
