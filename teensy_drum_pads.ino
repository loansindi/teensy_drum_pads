#include <IniFile.h>

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


Adafruit_LIS3DH lis = Adafruit_LIS3DH();
#define CLICKTHRESHHOLD 80

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

AudioPlaySdWav           playWav1;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

elapsedMillis attract;
elapsedMillis playInterval;
int sound = 0;
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
#define WHITE 0
#define BLUE 1
#define GREEN 2
#define RED 3



void printErrorMessage(uint8_t e, bool eol = true)
{
  switch (e) {
    case IniFile::errorNoError:
      Serial.print("no error");
      break;
    case IniFile::errorFileNotFound:
      Serial.print("file nocout found");
      break;
    case IniFile::errorFileNotOpen:
      Serial.print("file not open");
      break;
    case IniFile::errorBufferTooSmall:
      Serial.print("buffer too small");
      break;
    case IniFile::errorSeekError:
      Serial.print("seek error");
      break;
    case IniFile::errorSectionNotFound:
      Serial.print("section not found");
      break;
    case IniFile::errorKeyNotFound:
      Serial.print("key not found");
      break;
    case IniFile::errorEndOfFile:
      Serial.print("end of file");
      break;
    case IniFile::errorUnknownError:
      Serial.print("unknown error");
      break;
    default:
      Serial.print("unknown error value");
      break;
  }
  if (eol)
    Serial.println();
}


const size_t bufferLen = 80;
char buffer[bufferLen];

float idleRed;
float idleGreen;
float idleBlue;
float idleWhite;
float hitRed;
float hitGreen;
float hitBlue;
float hitWhite;
long hitSpeed;
long idleSpeed;

void setup() {
  //  const size_t bufferLen = 80;
  //  char buffer[bufferLen];
  AudioMemory(100);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(2, drum, FALLING);
  Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(.8);

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
  pwm.setPin(GREEN, 4096);
  delay(100);
  pwm.setPin(0, 0);
  pwm.setPin(BLUE, 0);
  pwm.setPin(GREEN, 0);
  pwm.setPin(RED, 0);
  pwm.setPin(WHITE, 0);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    Serial.println("No SD card!");
    while (1) {
      sdError();

    }
  }

  const char *config = "config.ini";
  IniFile ini(config);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(config);
    Serial.println(" does not exist");
    // Cannot do anything else
    while (1)
      ;
  }
  /*
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    printErrorMessage(ini.getError());
    // Cannot do anything else
    while (1)
      ;
  }
  */
  if (! ini.getValue("colors", "idleRed", buffer, bufferLen, idleRed)) {
    Serial.print("idleRed");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("colors", "idleGreen", buffer, bufferLen, idleGreen)) {
    Serial.print("idleGreen");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("colors", "idleBlue", buffer, bufferLen, idleBlue)) {
    Serial.print("idleBlue");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("colors", "idleWhite", buffer, bufferLen, idleBlue)) {
    Serial.print("idleWhite");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("colors", "hitRed", buffer, bufferLen, hitRed)) {
    Serial.print("hitRed");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("colors", "hitGreen", buffer, bufferLen, hitGreen)) {
    Serial.print("hitGreen");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("colors", "hitBlue", buffer, bufferLen, hitBlue)) {
    Serial.print("hitBlue");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("colors", "hitWhite", buffer, bufferLen, hitBlue)) {
    Serial.print("hitWhite");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("speeds", "hitSpeed", buffer, bufferLen, hitSpeed)) {
    Serial.print("hitSpeed");

    printErrorMessage(ini.getError());

    sdError();
  }
  if (! ini.getValue("speeds", "idleSpeed", buffer, bufferLen, idleSpeed)) {
    Serial.print("idleSpeed");
    printErrorMessage(ini.getError());

    sdError();
  }

  ini.close();


}


void sdError() {
  pwm.setPin(RED, 4096);
  delay(100);
  pwm.setPin(RED, 0);
  delay(100);
}


void drum() {

  if (playInterval > 5) {
    if (sound != 1) {
      sound = 1;
      attract = 0;
      Serial.println("DRUM");
    }
  }


}

void hit() {
   float vol = analogRead(15);
  vol = vol / 1024;
  sgtl5000_1.volume(vol);

  pwm.setPin(BLUE, 0);
  pwm.setPin(RED, 0);
  pwm.setPin(GREEN, 0);
  pwm.setPin(WHITE, 0);
  //Serial.println("Sound");
  playWav1.play("SOUND.WAV");
  delay(5);

  elapsedMillis fadeTime;
  sound = 0;
  attract = 0;
  playInterval = 0;
  for (uint16_t i = 4096; i > 0; i -= 64) {
    pwm.setPin(RED, i * hitRed);
    pwm.setPin(GREEN, i * hitGreen);
    pwm.setPin(BLUE, i * hitBlue);
    pwm.setPin(WHITE, i * hitWhite);
    delay(hitSpeed);

  }
  pwm.setPin(RED, 0);
  pwm.setPin(GREEN, 0);
  pwm.setPin(BLUE, 0);
  pwm.setPin(WHITE, 0);


}


void loop() {
  if (sound == 1) {
    hit();
  }

  if (attract > 3000) {
    Serial.println("Attract");
    for (int i = 64; i < 4096; i += 32) {
      elapsedMillis fadeTime;
      while (fadeTime < idleSpeed) {}
      if (attract < 3000) {
        break;
      }
      pwm.setPin(RED, i * idleRed);
      pwm.setPin(GREEN, i * idleGreen);
      pwm.setPin(BLUE, i * idleBlue);
      pwm.setPin(WHITE, i * idleWhite);
      fadeTime = 0;

    }
    for (int i = 4096; i > 64; i -= 32) {
      elapsedMillis fadeTime;
      while (fadeTime < idleSpeed) {}
      if (attract < 3000) {
        break;
      }
      pwm.setPin(RED, i * idleRed);
      pwm.setPin(GREEN, i * idleGreen);
      pwm.setPin(BLUE, i * idleBlue);
      pwm.setPin(WHITE, i * idleWhite);
      fadeTime = 0;

    }

  }
}
