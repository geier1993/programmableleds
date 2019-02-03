/* 
##############################
SnipSign programmable LED sign
by Niklas Köhn
##############################

Uses FastLED to animate Neopixels. Receives MIDI.

Contains parts from Marc Merlin's work Neopixel-IR.
MIDI part adapted from Teensy MIDI demo.
*/

#include "constMatrix.h"
#include <FastLED.h>
#include "fragmentColumnMovingRight.h"
#include "fragmentRowMovingDown.h"
#include "fragmentProperties.h"
#include "ledregion.h"
#include "vector"

using namespace std;

LedUtils ledUtils;
ColumnMovingRight cmr1(ledUtils, millis());
ColumnMovingRight cmr2(ledUtils, millis());
ColumnMovingRight cmr3(ledUtils, millis());
RowMovingDown rmd1(ledUtils, millis());
RowMovingDown rmd2(ledUtils, millis());
RowMovingDown rmd3(ledUtils, millis());
LedRegion reg00(ledUtils, 0);
LedRegion reg01(ledUtils, 1);
LedRegion reg02(ledUtils, 2);
LedRegion reg03(ledUtils, 3);
LedRegion reg04(ledUtils, 4);
LedRegion reg05(ledUtils, 5);
LedRegion reg06(ledUtils, 6);
LedRegion reg07(ledUtils, 7);
LedRegion reg08(ledUtils, 8);
LedRegion reg09(ledUtils, 9);
LedRegion reg10(ledUtils, 10);
LedRegion reg11(ledUtils, 11);
LedRegion reg12(ledUtils, 12);
LedRegion reg13(ledUtils, 13);
LedRegion reg14(ledUtils, 14);
LedRegion reg15(ledUtils, 15);
LedRegion reg16(ledUtils, 16);
LedRegion reg17(ledUtils, 17);
LedRegion reg18(ledUtils, 18);
LedRegion reg19(ledUtils, 19);
LedRegion reg20(ledUtils, 20);
LedRegion reg21(ledUtils, 21);
LedRegion reg22(ledUtils, 22);

// 32 bit flag for running regions
uint32_t regionsRunning = 0;

#define DATA_PIN 6
#define NUM_LEDS 479
#define DEBUG false
#define SOUND2LIGHT false
#define FRAGMENTS true

// Define the array of leds
CRGB crgbledstrip[NUM_LEDS];
// The fade values of the last 2 LEDs were always messed up. By increasing the array size by 2 the problem disappears... don't know why.
int fadestrip[NUM_LEDS + 2];

// AudioIn
byte incomingAudio;
boolean clipping = 0;

void setup()
{
  Serial.begin(115200);

  // init fadestrip: set all values to 0
  for (int i = 0; i < NUM_LEDS; i++)
  {
    fadestrip[i] = 0;
  }

  Serial.println("LED Setup");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(crgbledstrip, NUM_LEDS);
  LEDS.setBrightness(84);
  eraseAll();

  // setupAudioIn();
  Serial.println("Setup done");
}

void loop()
{
  vector<PixelUpdate> matrixUpdate;

  // usbMIDI.read() needs to be called rapidly from loop().  When
  // each MIDI messages arrives, it return true.  The message must
  // be fully processed before usbMIDI.read() is called again.
  if (usbMIDI.read())
  {
    processMIDI();
  }

  if (cmr1Running > 0)
  {
    cmr1.nextFrame(millis(), matrixUpdate, cmr1Running);
  }

  if (cmr2Running > 0)
  {
    cmr2.nextFrame(millis(), matrixUpdate, cmr2Running);
  }

  if (rmd1Running > 0)
  {
    rmd1.nextFrame(millis(), matrixUpdate, rmd1Running);
  }

  if (rmd2Running > 0)
  {
    rmd2.nextFrame(millis(), matrixUpdate, rmd2Running);
  }

  // Check for enabled regions
  if (regionsRunning & (1 << 0))
  {
    reg00.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 1))
  {
    reg01.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 2))
  {
    reg02.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 3))
  {
    reg03.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 4))
  {
    reg04.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 5))
  {
    reg05.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 6))
  {
    reg06.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 7))
  {
    reg07.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 8))
  {
    reg08.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 9))
  {
    reg09.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 10))
  {
    reg10.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 11))
  {
    reg11.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 12))
  {
    reg12.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 13))
  {
    reg13.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 14))
  {
    reg14.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 15))
  {
    reg15.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 16))
  {
    reg16.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 17))
  {
    reg17.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 18))
  {
    reg18.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 19))
  {
    reg19.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 20))
  {
    reg20.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 21))
  {
    reg21.nextFrame(matrixUpdate);
  }
  if (regionsRunning & (1 << 22))
  {
    reg22.nextFrame(matrixUpdate);
  }

  if (matrixUpdate.size() > 0)
  {
    for (vector<PixelUpdate>::iterator it = matrixUpdate.begin(); it != matrixUpdate.end(); ++it)
    {
      int ledNum = -1;
      if (it->ledNum > -1)
      {
        ledNum = it->ledNum;
      }
      else
      {
        ledNum = snipSignMatrix[it->row][it->col];
      }
      if (ledNum > -1)
      {
        crgbledstrip[ledNum] = CHSV(it->hue, it->sat, it->bri);
        if (DEBUG)
          Serial.println((String) "matrixUpdate at time " + millis() + ": col" + it->col + " / row" + it->row + " ... hue" + it->hue + " ... sat" + it->sat + " ... bri" + it->bri + ": fade" + it->fade + " ... time" + it->time + " ... ledNum" + ledNum + " ... fadestrip[ledNum]" + fadestrip[ledNum]);
      }
      fadestrip[ledNum] = it->fade;
    }
  }
  else {
      crgbledstrip[0] = CHSV(0, 0, 0);
  }
  FastLED.show();
  fadeIndividual();
}

void startCmr1(FragmentProperties fP)
{
  cmr1.setColorProperties(fP.hue, fP.sat, fP.bri, fP.hueIncrement);
  cmr1.setAnimationProperties(fP.wait, fP.fade, fP.reverse, fP.length, fP.start);
}
void startCmr2(FragmentProperties fP)
{
  cmr2.setColorProperties(fP.hue, fP.sat, fP.bri, fP.hueIncrement);
  cmr2.setAnimationProperties(fP.wait, fP.fade, fP.reverse, fP.length, fP.start);
}
void startRmd1(FragmentProperties fP)
{
  rmd1.setColorProperties(fP.hue, fP.sat, fP.bri, fP.hueIncrement);
  rmd1.setAnimationProperties(fP.wait, fP.fade, fP.reverse, fP.length, fP.start);
}
void startRmd2(FragmentProperties fP)
{
  rmd2.setColorProperties(fP.hue, fP.sat, fP.bri, fP.hueIncrement);
  rmd2.setAnimationProperties(fP.wait, fP.fade, fP.reverse, fP.length, fP.start);
}

void processMIDI(void)
{
  byte type, channel, data1, data2, cable;

  // fetch the MIDI message, defined by these 5 numbers (except SysEX)
  //
  type = usbMIDI.getType();       // which MIDI message, 128-255
  channel = usbMIDI.getChannel(); // which MIDI channel, 1-16
  data1 = usbMIDI.getData1();     // first data byte of message, 0-127
  data2 = usbMIDI.getData2();     // second data byte of message, 0-127
  cable = usbMIDI.getCable();     // which virtual cable with MIDIx8, 0-7

  // uncomment if using multiple virtual cables
  //Serial.print("cable ");
  //Serial.print(cable, DEC);
  //Serial.print(": ");

  // print info about the message
  //
  switch (type)
  {
  case usbMIDI.NoteOff: // 0x80
    Serial.print("Note Off, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(data1, DEC);
    Serial.print(", velocity=");
    Serial.println(data2, DEC);

    if (data1 == 0)
    {
      regionsRunning &= (0 << 0);
      reg00.setAnimationProperties(data1, 250);
    }
    else if (data1 == 1)
    {
      regionsRunning &= (0 << 1);
    }
    else if (data1 == 2)
    {
      regionsRunning &= (0 << 2);
    }
    else if (data1 == 3)
    {
      regionsRunning &= (0 << 3);
    }
    else if (data1 == 4)
    {
      regionsRunning &= (0 << 4);
    }
    else if (data1 == 5)
    {
      regionsRunning &= (0 << 5);
    }
    else if (data1 == 6)
    {
      regionsRunning &= (0 << 6);
    }
    else if (data1 == 7)
    {
      regionsRunning &= (0 << 7);
    }
    else if (data1 == 8)
    {
      regionsRunning &= (0 << 8);
    }
    else if (data1 == 9)
    {
      regionsRunning &= (0 << 9);
    }
    else if (data1 == 10)
    {
      regionsRunning &= (0 << 10);
    }
    else if (data1 == 11)
    {
      regionsRunning &= (0 << 11);
    }
    else if (data1 == 12)
    {
      regionsRunning &= (0 << 12);
    }
    else if (data1 == 13)
    {
      regionsRunning &= (0 << 13);
    }
    else if (data1 == 14)
    {
      regionsRunning &= (0 << 14);
    }
    else if (data1 == 15)
    {
      regionsRunning &= (0 << 15);
    }
    else if (data1 == 16)
    {
      regionsRunning &= (0 << 16);
    }
    else if (data1 == 17)
    {
      regionsRunning &= (0 << 17);
    }
    else if (data1 == 18)
    {
      regionsRunning &= (0 << 18);
    }
    else if (data1 == 19)
    {
      regionsRunning &= (0 << 19);
    }
    else if (data1 == 20)
    {
      regionsRunning &= (0 << 20);
    }
    else if (data1 == 21)
    {
      regionsRunning &= (0 << 21);
    }
    else if (data1 == 22)
    {
      regionsRunning &= (0 << 22);
    }
    break;

  case usbMIDI.NoteOn: // 0x90
    Serial.print("Note On, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(data1, DEC);
    Serial.print(", velocity=");
    Serial.println(data2, DEC);

    // data1 = note
    // data2 = velocity
    if (data1 == 0) // S
    {
      regionsRunning |= (1 << 0);
      reg00.setColorProperties(data2*2, 100);
    }
    else if (data1 == 1)  // n
    {
      regionsRunning |= (1 << 1);
      reg01.setColorProperties(data2*2, 100);
    }
    else if (data1 == 2)  // i
    {
      regionsRunning |= (1 << 2);
      reg02.setColorProperties(data2*2, 100);
    }
     else if (data1 == 3)  // Sp1
     {
       regionsRunning |= (1 << 3);
       reg03.setColorProperties(data2*2, 100);
     }
    else if (data1 == 4)  // Sp2
    {
      regionsRunning |= (1 << 4);
      reg04.setColorProperties(data2*2, 100);
    }
    else if (data1 == 5)  // Se
    {
      regionsRunning |= (1 << 5);
      reg05.setColorProperties(data2*2, 100);
    }
    else if (data1 == 6)  // t
    {
      regionsRunning |= (1 << 6);
      reg06.setColorProperties(data2*2, 100);
    }
    else if (data1 == 7)
    {
      regionsRunning |= (1 << 7);
      reg07.setColorProperties(data2*2, 100);
    }
    else if (data1 == 8)
    {
      regionsRunning |= (1 << 8);
      reg08.setColorProperties(data2*2, 100);
    }
    else if (data1 == 9)
    {
      regionsRunning |= (1 << 9);
      reg09.setColorProperties(data2*2, 100);
    }
    else if (data1 == 10)
    {
      regionsRunning |= (1 << 10);
      reg10.setColorProperties(data2*2, 100);
    }
    else if (data1 == 11)
    {
      regionsRunning |= (1 << 11);
      reg11.setColorProperties(data2*2, 100);
    }
    else if (data1 == 12)
    {
      regionsRunning |= (1 << 12);
      reg12.setColorProperties(data2*2, 100);
    }
    else if (data1 == 13)
    {
      regionsRunning |= (1 << 13);
      reg13.setColorProperties(data2*2, 100);
    }
    else if (data1 == 14)
    {
      regionsRunning |= (1 << 14);
      reg14.setColorProperties(data2*2, 100);
    }
    else if (data1 == 15)
    {
      regionsRunning |= (1 << 15);
      reg15.setColorProperties(data2*2, 100);
    }
    else if (data1 == 16)
    {
      regionsRunning |= (1 << 16);
      reg16.setColorProperties(data2*2, 100);
    }
    else if (data1 == 17)
    {
      regionsRunning |= (1 << 17);
      reg17.setColorProperties(data2*2, 100);
    }
    else if (data1 == 18)
    {
      regionsRunning |= (1 << 18);
      reg18.setColorProperties(data2*2, 100);
    }
    else if (data1 == 19)
    {
      regionsRunning |= (1 << 19);
      reg19.setColorProperties(data2*2, 100);
    }
    else if (data1 == 20)
    {
      regionsRunning |= (1 << 20);
      reg20.setColorProperties(data2*2, 100);
    }
    else if (data1 == 21)
    {
      regionsRunning |= (1 << 21);
      reg21.setColorProperties(data2*2, 100);
    }
    else if (data1 == 22)
    {
      regionsRunning |= (1 << 22);
      reg22.setColorProperties(data2*2, 100);
    }
    else if (data1 == 30)
    {
      rmd2Running = 1;

//      int data2Hundreds = floor(data2/100)*100;
//      double data2TensAndOnes = data2-data2Hundreds;
//      Serial.println((String)"data2 " + data2 + " => " + data2Hundreds + " , " + data2TensAndOnes);

      int bri = 255-((channel-1)*10.625); // channel 1-16 => 16*10.625=170, which is 2/3 of the total brightness. Lower that that doesn't really make sense.
      Serial.println((String)"channel " + channel + " .. bri " + bri);

      FragmentProperties fP;
      fP.hue = data2*2;
      fP.sat = 255; //200;
      fP.bri = bri;
      fP.hueIncrement = 0.0;
      fP.wait = 30;
      fP.fade = 230;
      fP.reverse = true;
      startRmd2(fP);
    }
    else if (data1 == 31)
    {
      rmd1Running = 1;

      FragmentProperties fP;
      fP.hue = data2*2;
      fP.sat = 255; //200;
      fP.bri = 255; //60;
      fP.hueIncrement = 0.9;
      fP.wait = 30;
      fP.fade = 230;
      fP.reverse = true;
      fP.length = 30;
      fP.start = 0;
      startRmd1(fP);
    }
    else if (data1 == 40)
    {
      cmr1Running = 1;

      FragmentProperties fP;
      fP.hue = data2*2;
      fP.sat = 1;
      fP.bri = 150;
      fP.hueIncrement = 1.0;
      fP.wait = 80;
      fP.fade = 230;
      fP.reverse = true;
      fP.length = 30;
      fP.start = 30;
      startCmr1(fP);
    }
    else if (data1 == 41)
    {
      cmr2Running = 1;

      FragmentProperties fP;
      fP.hue = data2*2;
      fP.sat = 200;
      fP.bri = 250;
      fP.hueIncrement = 0.0;
      fP.wait = 30;
      fP.fade = 180;
      fP.reverse = false;
      startCmr2(fP);
    }
    break;

  case usbMIDI.AfterTouchPoly: // 0xA0
    Serial.print("AfterTouch Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(data1, DEC);
    Serial.print(", velocity=");
    Serial.println(data2, DEC);
    break;

  case usbMIDI.ControlChange: // 0xB0
    Serial.print("Control Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(data1, DEC);
    Serial.print(", value=");
    Serial.println(data2, DEC);
    break;

  case usbMIDI.ProgramChange: // 0xC0
    Serial.print("Program Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", program=");
    Serial.println(data1, DEC);
    break;

  case usbMIDI.AfterTouchChannel: // 0xD0
    Serial.print("After Touch, ch=");
    Serial.print(channel, DEC);
    Serial.print(", pressure=");
    Serial.println(data1, DEC);
    break;

  case usbMIDI.PitchBend: // 0xE0
    Serial.print("Pitch Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", pitch=");
    Serial.println(data1 + data2 * 128, DEC);
    break;

  case usbMIDI.SystemExclusive: // 0xF0
    // Messages larger than usbMIDI's internal buffer are truncated.
    // To receive large messages, you *must* use the 3-input function
    // handler.  See InputFunctionsComplete for details.
    Serial.print("SysEx Message: ");
    printBytes(usbMIDI.getSysExArray(), data1 + data2 * 256);
    Serial.println();
    break;

  case usbMIDI.TimeCodeQuarterFrame: // 0xF1
    Serial.print("TimeCode, index=");
    Serial.print(data1 >> 4, DEC);
    Serial.print(", digit=");
    Serial.println(data1 & 15, DEC);
    break;

  case usbMIDI.SongPosition: // 0xF2
    Serial.print("Song Position, beat=");
    Serial.println(data1 + data2 * 128);
    break;

  case usbMIDI.SongSelect: // 0xF3
    Serial.print("Sond Select, song=");
    Serial.println(data1, DEC);
    break;

  case usbMIDI.TuneRequest: // 0xF6
    Serial.println("Tune Request");
    break;

  case usbMIDI.Clock: // 0xF8
    Serial.println("Clock");
    break;

  case usbMIDI.Start: // 0xFA
    Serial.println("Start");
    break;

  case usbMIDI.Continue: // 0xFB
    Serial.println("Continue");
    break;

  case usbMIDI.Stop: // 0xFC
    Serial.println("Stop");
    break;

  case usbMIDI.ActiveSensing: // 0xFE
    Serial.println("Actvice Sensing");
    break;

  case usbMIDI.SystemReset: // 0xFF
    Serial.println("System Reset");
    break;

  default:
    Serial.println("Opps, an unknown MIDI message type!");
  }
}

void eraseAll()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    crgbledstrip[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void fadeall()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    crgbledstrip[i].nscale8(250);
  }
}

void fadeIndividual()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    crgbledstrip[i].nscale8(fadestrip[i]);
  }
}

void printBytes(const byte *data, unsigned int size)
{
  while (size > 0)
  {
    byte b = *data++;
    if (b < 16)
      Serial.print('0');
    Serial.print(b, HEX);
    if (size > 1)
      Serial.print(' ');
    size = size - 1;
  }
}

// void setupAudioIn() {
//   pinMode(13,OUTPUT);//led indicator pin

//   cli();//disable interrupts

//   //set up continuous sampling of analog pin 0

//   //clear ADCSRA and ADCSRB registers
//   ADCSRA = 0;
//   ADCSRB = 0;

//   ADMUX |= (1 << REFS0); //set reference voltage
//   ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only

//   ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 1mHz/32=500kHz
//   ADCSRA |= (1 << ADATE); //enable auto trigger
//   ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
//   ADCSRA |= (1 << ADEN); //enable ADC
//   ADCSRA |= (1 << ADSC); //start ADC measurements

//   sei();//enable interrupts
// }

// ISR(ADC_vect) { //when new ADC value ready
//   incomingAudio = ADCH; //store 8 bit value from analog pin 0
//   incomingAudio = incomingAudio*9;
//   if (incomingAudio == 0 || incomingAudio == 255){
//     digitalWrite(13,HIGH);
//     clipping = 1;
//   }
//   else {
//     if (clipping){
//       clipping = 0;
//       digitalWrite(13,LOW);
//     }
//   }
// }
