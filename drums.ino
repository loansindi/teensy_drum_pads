#include <memcpy_audio.h>
#include <AudioControl.h>
#include <output_dac.h>
#include <play_memory.h>
#include <Audio.h>
#include "AudioSampleEmuproteusfxwackysnarewav1z.h"

// GUItool: begin automatically generated code

AudioPlayMemory          playMem1;       //xy=199,259
AudioOutputAnalog        dac1;           //xy=627,242
AudioConnection          patchCord1(playMem1, dac1);
// GUItool: end automatically generated code


void setup() {
  AudioMemory(200);
  pinMode(0, INPUT_PULLUP);
  pinMode(12, OUTPUT);

}

void loop() {

  
  if(digitalRead(0) == LOW) {
    digitalWrite(12, HIGH);
    playMem1.play(AudioSampleEmuproteusfxwackysnarewav1z);
    delay(100);
    digitalWrite(12, LOW);  
  }
      


}
