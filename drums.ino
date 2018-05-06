#include <output_dacs.h>
#include <effect_delay.h>
#include <synth_simple_drum.h>
#include <filter_biquad.h>
#include <effect_reverb.h>
#include <play_sd_wav.h>
#include <effect_fade.h>
#include <synth_pinknoise.h>
#include <effect_midside.h>
#include <control_sgtl5000.h>
#include <synth_tonesweep.h>
#include <memcpy_audio.h>
#include <analyze_notefreq.h>
#include <synth_waveform.h>
#include <play_queue.h>
#include <control_cs42448.h>
#include <output_i2s.h>
#include <output_i2s_quad.h>
#include <mixer.h>
#include <analyze_rms.h>
#include <analyze_fft256.h>
#include <analyze_tonedetect.h>
#include <synth_sine.h>
#include <record_queue.h>
#include <synth_dc.h>
#include <filter_fir.h>
#include <output_pt8211.h>
#include <filter_variable.h>
#include <input_i2s.h>
#include <input_adcs.h>
#include <play_sd_raw.h>
#include <AudioControl.h>
#include <control_cs4272.h>
#include <effect_multiply.h>
#include <effect_delay_ext.h>
#include <effect_flange.h>
#include <control_ak4558.h>
#include <effect_bitcrusher.h>
#include <output_dac.h>
#include <synth_karplusstrong.h>
#include <effect_chorus.h>
#include <input_i2s_quad.h>
#include <spi_interrupt.h>
#include <play_memory.h>
#include <effect_waveshaper.h>
#include <output_pwm.h>
#include <Audio.h>
#include <synth_pwm.h>
#include <control_wm8731.h>
#include <output_tdm.h>
#include <input_adc.h>
#include <analyze_peak.h>
#include <play_serialflash_raw.h>
#include <input_tdm.h>
#include <synth_whitenoise.h>
#include <output_spdif.h>
#include <effect_envelope.h>
#include <analyze_print.h>
#include <analyze_fft1024.h>
#include <SerialFlash.h>
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
