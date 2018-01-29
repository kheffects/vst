//Connor K Harrington
//github.com/kheffects

#include "Tremolo.h"
#include "stdlib.h"

Tremolo::Tremolo(){
	speed = .0001;
	depth = 0.0;
	i = 0.0;
}

Tremolo::Tremolo(double inSpeed, double inDepth) {
	this->speed = inSpeed;
	this->depth = inDepth;
	this->i = 0.0;
}

Tremolo::~Tremolo(){}

void Tremolo::SetSpeed(double inSpeed) {
	this->speed = inSpeed;
}

void Tremolo::SetDepth(double inDepth) {
	this->depth = inDepth;
}

void Tremolo::Iter() {
	this->i += this->speed;
}