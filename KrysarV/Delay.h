//Connor K Harrington
//github.com/kheffects

#ifndef Delay_h
#define Delay_h

#include "vector"
#include "Filter.h"

class Delay {
public:	
	Delay();
	~Delay();
	Delay(int time);
	void SetTime(double time);
	void SetMix(double inMix);
	void SetFeedback(double inFeedback);
	inline double DelaySample(double input);
private:
	std::vector<double> DelayVector;
	std::vector<double> DelayVector2;
	Filter* DelayFilter;
	int i;
	int i2;
	int b;
	double feedback;
	double temp;
	double mix;
};

//A delay or echo works by recording and playing back a given sample over a specified time period
//I have implemented this by round-robining two vectors filled with live recordings in real time
inline double Delay::DelaySample(double input) {
	if (i < DelayVector.size() && b == 0) {
		temp = DelayVector2[i]; 
		DelayVector[i] = DelayFilter->FilterSample(temp*feedback) + input;
		i++;
	}
	else if (i >= DelayVector.size() && b == 0) {
		i = 0;
		b = 1;
	}
	else if (i2 < DelayVector2.size() && b == 1) {
		temp = DelayVector[i2];
		DelayVector2[i2] = DelayFilter->FilterSample(temp*feedback) + input;
		i2++;
	}
	else if (i2 >= DelayVector2.size() && b == 1) {
		i2 = 0;
		b = 0;
	}
	return temp*mix + input;
}

#endif
