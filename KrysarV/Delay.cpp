//Connor K Harrington
//github.com/kheffects

#include "Delay.h"
#include "stdlib.h"
#include "Filter.h"

Delay::Delay() {
	this->DelayVector.resize(18522, 0.0);
	this->DelayVector2.resize(18522, 0.0);
	this->i = 0;
	this->i2 = 0;
	this->b = 0;
	this->feedback = 0.0;
	this->mix = 0.0;
	//Used to emulate a dark analog delay
	this->DelayFilter = new Filter(0.3571472555781642, -0.09407186380687081, 0.28930939067212436, -0.09407186380687081, -0.3535433537497115);
}

Delay::Delay(int time) {
	SetTime(time);
}

Delay::~Delay(){}

void Delay::SetTime(double time) {
	this->DelayVector.resize((int)(time*44100), 0.0);
	this->DelayVector2.resize((int)(time*44100), 0.0);
}

void Delay::SetMix(double inMix) {
	this->mix = inMix;
}

void Delay::SetFeedback(double inFeedback) {
	this->feedback = inFeedback;
}