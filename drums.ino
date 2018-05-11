#include <memcpy_audio.h>
#include <AudioControl.h>
#include <output_dac.h>
#include <play_memory.h>
#include <Audio.h>
#include "AudioSample__deller24__chicosnarewavpffm.h"

// GUItool: begin automatically generated code

AudioPlayMemory          playMem1;       //xy=199,259
AudioOutputAnalog        dac1;           //xy=627,242
AudioConnection          patchCord1(playMem1, dac1);
// GUItool: end automatically generated code


elapsedMillis attract;
elapsedMillis playInterval;
bool hit;


void setup() {
    AudioMemory(25);
    pinMode(0, INPUT_PULLUP);
    pinMode(10, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(6, OUTPUT);
    analogWriteFrequency(9, 600);
    attachInterrupt(0, drum, FALLING);
}


void drum() {

    detachInterrupt(0);
    attract = 0;
    playInterval = 0;
    if(playMem1.isPlaying() == false ) {
        playMem1.play(AudioSample__deller24__chicosnarewavpffm);
    }
    hit = true; 
    attachInterrupt(0, drum, FALLING);

}

void  upFadeDown() {
    elapsedMillis fadeTime;

    for(int i=255; i>0; i--) {
        while (fadeTime < 1) {}
        analogWrite(10, i);
        analogWrite(9, i);
        analogWrite(6, i);
        fadeTime = 0;
    }
}

void loop() {
    if(hit == true) {
	upFadeDown();
        hit = false;
    }
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




