//Connor K Harrington
//github.com/kheffects

#ifndef Filter_h
#define Filter_h

class Filter {
public:
	Filter();
	~Filter();
	Filter(double inA0, double inA1, double inA2, double inB1, double inB2);
	double FilterSample(double input);
private:
	double delay1, delay2, output, a0, a1, a2, b1, b2;
};

//Biquad filter calculation. Manually selected values for biquad coefficients. See Biquad calculator URL in Filter.cpp
inline double Filter::FilterSample(double input) {
	output = input * a0 + delay1;
	delay1 = input * a1 + delay2 - b1 * output;
	delay2 = input * a2 - b2 * output;
	return output;
}

#endif