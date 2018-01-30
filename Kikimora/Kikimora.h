#ifndef __KIKIMORA__
#define __KIKIMORA__

#include "IPlug_include_in_plug_hdr.h"

class Kikimora : public IPlug{
public:
  Kikimora(IPlugInstanceInfo instanceInfo);
  ~Kikimora();
  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** input, double** output, int nFrames);
private:
	double SUPER_SPEED, SUPER_PITCH1, SUPER_PITCH2, SUPER_PITCH3, SUPER_PITCH4, SUPER_PITCH5, SUPER_PITCH6, SUPER_PITCH7, SUPER_PITCH8, SUPER_VOLUME;
	void IterateSynth();
	void ChangeSynthPitch();
};

#endif
