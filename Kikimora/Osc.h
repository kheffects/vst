#ifndef OSC_H
#define OSC_H
#define _USE_MATH_DEFINES
#include "math.h"

enum {
	sine = 0,
	triangle,
	square
};

class Osc {
public:
	Osc();
	~Osc();
	Osc(int inWaveform, double inPitch, double inSampleRate);
	void SetPitch(double inPitch);
	void SetWaveform(int inWaveform);
	inline double OscGenerateSample();
	void SetVibrato(double inVibrato);
	void SetDepth(double inDepth);
private:
	int waveform;
	void Iter();
	void Vibrato();
	double pitch, freq, sampleRate, v, a, n, vibrato, depth, vibratoIterator;
};

//Makes waveforms manually per sample
inline double Osc::OscGenerateSample() {
	Iter();
	Vibrato();
	switch (waveform) {
	case sine:
		return sin(2*M_PI*(v+ sin(2 * M_PI*vibratoIterator)*depth));
	case triangle:
		return 2.0*(fabs(-2*(v + sin(2 * M_PI*vibratoIterator)*depth) +1)-0.5);
	case square:
		if (sin(2 * M_PI*(v + sin(2 * M_PI*vibratoIterator)*depth)) >= 0) {
			return 1;
		}
		else {
			return -1;
		}
	default:
		return 0;
	}
}

#endif // !OSC_H

