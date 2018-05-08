#include <memcpy_audio.h>
#include <AudioControl.h>
#include <output_dac.h>
#include <play_memory.h>
#include <Audio.h>
#include "AudioSampleBassdrumw.h"

// GUItool: begin automatically generated code

AudioPlayMemory          playMem1;       //xy=199,259
AudioOutputAnalog        dac1;           //xy=627,242
AudioConnection          patchCord1(playMem1, dac1);
// GUItool: end automatically generated code

elapsedMillis attract;
elapsedMillis playInterval;


void setup() {
  AudioMemory(15);
  pinMode(0, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  attachInterrupt(0, drum, FALLING);
  Serial.begin(9600);
}

void drum() {
  
    detachInterrupt(0);
    if(playInterval > 100){
     attract = 0;
     playInterval = 0;
     analogWrite(10, 0);
     analogWrite(9, 0);
     analogWrite(6, 0);
     playMem1.play(AudioSampleBassdrumw);
     upFadeDown();
    }
    attachInterrupt(0, drum, FALLING);

}

void upFadeDown() {
      elapsedMillis fadeTime;

   for(int i=255;i>0;i--){
    while (fadeTime < 1) {}
    analogWrite(10, i);
    analogWrite(9, i);
    analogWrite(6, i);
    fadeTime = 0;
    }  
}

void loop() {
  //Serial.println(AudioMemoryUsageMax());
  Serial.println(AudioProcessorUsageMax());

  if(attract > 10000) {
    for(int i = 0; i<255; i++) {
      analogWrite(10, i);
      elapsedMillis fadeTime; 
      while(fadeTime < 5) {}
      if(attract < 10000) {
        break;
      }
      analogWrite(10, i);
      fadeTime = 0;

    }
    for(int i=255; i>0; i--) {
      elapsedMillis fadeTime; 
      while(fadeTime < 5) {}
      if(attract < 10000) {
        break;
      }
      analogWrite(10, i);
      fadeTime = 0;

     }

    }
  }
      



