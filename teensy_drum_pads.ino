#include <memcpy_audio.h>
#include <AudioControl.h>
#include <output_dac.h>
#include <play_memory.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Adafruit_LIS3DH lis = Adafruit_LIS3DH();
#define CLICKTHRESHHOLD 80

/*
  #include "clap.h"
  #include "hat.h"
  #include "kick.h"
  #include "ride.h"
  #include "snare.h"
  #include "tomhigh.h"
  #include "tomlow.h"
*/
/*
  // GUItool: begin automatically generated code

  AudioPlayMemory          playMem1;       //xy=199,259
  AudioOutputAnalog        dac1;           //xy=627,242
  AudioConnection          patchCord1(playMem1, dac1);
  // GUItool: end automatically generated code
*/
AudioPlaySdWav           playWav1;
// Use one of these 3 output types: Digital I2S, Digital S/PDIF, or Analog DAC
AudioOutputI2S           audioOutput;
//AudioOutputSPDIF       audioOutput;
//AudioOutputAnalog      audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

elapsedMillis attract;
elapsedMillis playInterval;
int sound = 0;
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

void setup() {
  Serial.begin(9600);
  AudioMemory(100);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(2, drum, FALLING);

  sgtl5000_1.enable();
  sgtl5000_1.volume(.8);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");
  lis.setClick(1, CLICKTHRESHHOLD);

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
  delay(100);
  pwm.setPin(0, 0);
  pwm.setPin(1, 0);
  pwm.setPin(2, 0);
  pwm.setPin(3, 0);
}

void drum() {
  if (playInterval > 10) {
    if (sound != 1) {
      //playWav1.stop();
      delay(5);
      sound = 1;
      attract = 0;
      Serial.println("DRUM");
    }
  }


}


void loop() {
  if (sound == 1) {
    pwm.setPin(2, 0);
    //Serial.println("Sound");
    playWav1.play("CLAP.WAV");
    delay(25);

    elapsedMillis fadeTime;
    sound = 0;
    attract = 0;
    playInterval = 0;
    for (uint16_t i = 4096; i > 0; i -= 64) {
      pwm.setPin(1, i);
      delay(1);

    }
    pwm.setPin(1, 0);

  }



  if (attract > 3000) {
    Serial.println("Attract");
    for (int i = 64; i < 4096; i += 32) {
      elapsedMillis fadeTime;
      while (fadeTime < 10) {}
      if (attract < 3000) {
        break;
      }
      //analogWrite(10, i);
      //analogWrite(9, i/2);
      pwm.setPin(2, i);
      fadeTime = 0;

    }
    for (int i = 4096; i > 64; i -= 32) {
      elapsedMillis fadeTime;
      while (fadeTime < 10) {}
      if (attract < 3000) {
        break;
      }
      //analogWrite(10, i);
      //analogWrite(9, i/2);
      pwm.setPin(2, i);
      fadeTime = 0;

    }

  }
}
