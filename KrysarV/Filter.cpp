//Connor K Harrington
//github.com/kheffects

#include "Filter.h"
#include "stdlib.h"

//Used to calculate coefficents: http://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/
Filter::Filter(){
	delay1 = 0.0;
	delay2 = 0.0;
	output = 0.0;
	a0 = 0.813223141473437;
	a1 = 1.626446282946874;
	a2 = 0.813223141473437;
	b1 = 1.5912598102766813;
	b2 = 0.6616327556170669;
}

Filter::~Filter(){}

Filter::Filter(double inA0, double inA1, double inA2, double inB1, double inB2){
	this->delay1 = 0.0;
	this->delay2 = 0.0;
	this->output = 0.0;
    this->a0 = inA0;
	this->a1 = inA1;
	this->a2 = inA2;
	this->b1 = inB1;
	this->b2 = inB2;
}