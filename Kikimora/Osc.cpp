#include "Osc.h"
#include "stdlib.h"

Osc::Osc() {
	waveform = sine;
	pitch = 440.0;
	sampleRate = 44100.0;
	freq = pitch / sampleRate;
	v = 0.0;
	a = 0.0;
	n = 0.0;
	vibrato = 0.0;
	depth = 0.0;
	vibratoIterator = 0.0;
}

Osc::~Osc(){}

Osc::Osc(int inWaveform, double inPitch, double inSampleRate) {
	this->waveform = inWaveform;
	this->pitch = inPitch;
	this->sampleRate = inSampleRate;
	this->freq = this->pitch / this->sampleRate;
	this->v = 0.0;
	this->a = 0.0;
	this->n = 1.0;
	this->vibrato = 0.0;
	this->depth = 0.0;
	this->vibratoIterator = 0.0;
}

void Osc::SetPitch(double inPitch) {
	this->pitch = inPitch;
	this->freq = this->pitch / this->sampleRate;
}

void Osc::SetVibrato(double inVibrato) {
		this->vibrato = inVibrato / this->sampleRate;
}

void Osc::SetDepth(double inDepth) {
	this->depth = inDepth;
}

void Osc::SetWaveform(int inWaveform) {
	this->waveform = inWaveform;
}

//Goes through one period of each waveform
void Osc::Iter() {
	this->v += this->freq;
	if (this->v >= 1) {
		this->v = 0.0;
	}
}

//Modulates the pitch of the selected step in the step sequencer
void Osc::Vibrato() {
	if (this->vibrato > 1 / this->sampleRate) {
		this->vibratoIterator += this->vibrato;
		if (this->vibratoIterator >= 1) {
			this->vibratoIterator = 0.0;
		}
	}
}
