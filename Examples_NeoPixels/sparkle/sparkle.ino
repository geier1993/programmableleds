// Simple NeoPixel test.  Lights just a few pixels at a time so a
// long strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip -- look for the data
// direction arrows printed on the strip.

#include <Adafruit_NeoPixel.h>

#define PIN    6
#define N_LEDS 60 


const uint8_t BitMap[5] = {   // store in program memory to save RAM
    B1100011,
    B0010100,
    B0001000,
    B0010100,
    B1100011
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
}

void loop() {
  bitmap(0x010101);//, 0b1100101);
}


int updateColor(int c) {
  if (c<0xFFFFFF) {
    c+=0x010101;
  }
  else
  {
    c=0x010101;
  }
}


static void bitmap(uint32_t c) {
  uint8_t n = 30;
  uint32_t sub = 0x001000;
  int8_t increment = 1;
  uint16_t p = 0;  

DisplayBitMap(c);

/*
  for (int d=bitmap; d>0; 1<<d) { //1152921504606846975; d+=1000) {
    DisplayBitMap(d, c);
    //c+=0x010101;
    //delay(10);
    
    // erase all
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i  , 0);
      strip.show();
    }
    //delay(1);
  }
  */
}


void DisplayBitMap(uint8_t c)
{
    for (byte x=0; x<5; ++x) {
      for (byte y=0; y<7; ++y) {
        byte data = bitRead((int)&BitMap[x], y);
        if (data) {
            strip.setPixelColor(y, c);
        } else {
            strip.setPixelColor(y, c);
        }
      }
      strip.show();
    }
}

static void DisplayBitMap2(int data, uint32_t c)
{
  //int data = 1; //pgm_read_byte (&BitMap[0]);   // fetch data from program memory
    for (int y=0; y<N_LEDS; ++y) {
        strip.setPixelColor(data, c);
        if (data & (1<<y)) {
            strip.setPixelColor(y, c);
    /*
        } else {
            strip.setPixelColor(y, 0);*/
        }
    }
}




static void sparkle(uint32_t c) {
  uint8_t n = 30;
  uint32_t sub = 0x000000;

  // draw
  uint16_t iopp = strip.numPixels()+16;
  uint16_t p = -18;
  uint16_t popp = iopp;
  for(uint16_t i=0; i<strip.numPixels()+18; i++) {

      //from front
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p+=2;
      strip.setPixelColor(p  , c); // Draw new pixel
      c-=sub;
      p-=17;
      
      strip.show();
      delay(10);
    
      // erase all
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i  , 0);
      }
      strip.show();
      delay(30);
  }
}

