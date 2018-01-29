//Connor K Harrington
//github.com/kheffects

#ifndef Tremolo_h
#define Tremolo_h 
#define _USE_MATH_DEFINES

#include "math.h"

class Tremolo {
public:
	Tremolo();
	~Tremolo();
	Tremolo(double inSpeed, double inDepth);
	void SetSpeed(double inSpeed);
	void SetDepth(double inDepth);
	void Iter();
	inline double TremoloSample(double input);
private:
	double speed, depth, i;
};

//A tremolo works by rapidly modulating the volume of a signal. Here the volume is being modulated by a sine wave
inline double Tremolo::TremoloSample(double input) {
	Iter();
	if (i > 2*3.14) {
		i = 0.0;
	}
	return (input*(1.0-fabs(sin(i)*depth)));
}
#endif