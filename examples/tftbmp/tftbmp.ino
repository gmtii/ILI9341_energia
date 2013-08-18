#include "SPI.h" 
#include "TFTv2.h"
#include "fatfs.h"

int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

#define cs_pin      PA_5             		        // chip select pin on the Stellaris
#define file        "FLOWER.BMP"                // BMP filename on SD
#define file2       "hibiscus.bmp"              // BMP filename on SD

int rc;
uint32_t offset_counter=0;                      // Global offset when reading from SD


void setup()
{

  Serial.begin(115200);			   	 // Don't remove!

  Tft.begin(PE_1,PE_2,PE_3,PE_4);                // CS,DC,BL,RESET pin
  Tft.TFTinit();                                 // init TFT library
  Tft.backlight_on();                            // turn on the background light

  Tft.drawString("LCD init OK",0,0,2,WHITE,BLACK);  // Init ok message on LCD

  delay(1000);

  Tft.setCol(0,239);
  Tft.setPage(0,319);
  Tft.sendCMD(0x2c);                             // LCD init

  FatFs.begin(cs_pin,2,0);             		 // initialize FatFS library calls on SPI2, SS2=pin 12=PA_5


}

void die (int pff_err) 		/* Stop with dying message */
/* FatFs return value */
{
  Serial.println();
  Serial.print("Failed with rc=");
  Serial.print(pff_err,DEC);
  for (;;) ;
}

void dir_list (void) {
  FILINFO file_info;            // information on file size and type
  unsigned int files_found = 0; // count of files found in directory

  FatFs.opendir();
  do {
    FatFs.readdir(&file_info);

    if ((file_info.fattrib & AM_DIR) != AM_DIR) {
      Serial.println(file_info.fname);
      files_found++;
      delay(100);
    }
  }
  while (file_info.fname[0] != 0);

  Serial.print(files_found); 
  Serial.println(" files found.");
}

// Returns byte located at offset_counter variable and does offset_counter++ for next time

char file_sequential_read() {

  unsigned int bytes_read;                      // number of bytes read each call
  char out[1];

  FatFs.lseek(offset_counter);                  // set file r/w pointer back to 0

  FatFs.read(out, 1, &bytes_read);
  offset_counter++;

  return out[0];
}

void loop()
{
  dir_list();                                  // debug files on sd to serial
  rc= FatFs.open(file, FA_READ );              // open file read mode
  if (rc) die(rc);

  bmpReadHeader();                             // debug bmp header to serial

  bmpdraw(0, 0);                               // draws bmp on screen
  
  FatFs.close();                               // closes file
  
  offset_counter=0;                            // restarts offset counter for next image
  
  delay(5000);
  
  rc= FatFs.open(file2, FA_READ );              // open file read mode
  if (rc) die(rc);

  bmpReadHeader();                             // debug bmp header to serial

  bmpdraw(0, 0);                               // draws bmp on screen
  
  FatFs.close();                               // closes file

  for (;;) ;                                   // end
    
}

void bmpReadHeader() {

  uint32_t tmp;

  if (read16() != 0x4D42) {
    // magic bytes missing
    Serial.print("\nBad header ");
    return;
  }
  else
    Serial.print("\nBMP file detected ");

  Serial.print("\nSize "); 
  Serial.print(read32(), DEC); 
  Serial.println(" bytes.");
  // read and ignore creator bytes
  read32();

  bmpImageoffset = read32();  
  Serial.print("offset "); 
  Serial.println(bmpImageoffset, DEC);

  // read DIB header
  tmp = read32();
  Serial.print("header size "); 
  Serial.println(tmp, DEC);
  bmpWidth = read32();
  bmpHeight = read32();


  if (read16() != 1)
    return ;

  bmpDepth = read16();
  Serial.print("bitdepth "); 
  Serial.println(bmpDepth, DEC);

  if (read32() != 0) {
    // compression not supported!
    return;
  }

  Serial.print("compression "); 
  Serial.println(tmp, DEC);

}

// LITTLE ENDIAN!

uint16_t read16() 
{
  uint16_t d;
  uint8_t b;
  b = file_sequential_read();
  d = file_sequential_read();
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
uint32_t read32() 
{
  uint32_t d;
  uint16_t b;

  b = read16();
  d = read16();
  d <<= 16;
  d |= b;
  return d;
}


/*********************************************/
// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. increading the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place


#define BUFFPIXEL 20

void bmpdraw(int x, int y) 
{

  unsigned int bytes_read;         // number of bytes read each call

  FatFs.lseek(bmpImageoffset);

  uint32_t time = millis();
  uint16_t p;
  uint8_t g, b;
  int i, j;

  uint8_t sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
  uint8_t buffidx = 3*BUFFPIXEL;


  for (i=0; i< bmpHeight; i++) 
  {

    for (j=bmpWidth; j>0; j--) 
    {
      // read more pixels
      if (buffidx >= 3*BUFFPIXEL) 
      {
        FatFs.read(sdbuffer, 3*BUFFPIXEL, &bytes_read);
        buffidx = 0;
      }

      // convert pixel from 888 to 565
      b = sdbuffer[buffidx++];     // blue
      g = sdbuffer[buffidx++];     // green
      p = sdbuffer[buffidx++];     // red

      p >>= 3;
      p <<= 6;

      g >>= 2;
      p |= g;
      p <<= 5;

      b >>= 3;
      p |= b;

      // write out the 16 bits of color
      Tft.setPixel(j,i,p);
    }
  }
  Serial.print(millis() - time, DEC);
  Serial.println(" ms");
}


