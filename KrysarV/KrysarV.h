//Connor K Harrington
//github.com/kheffects

#ifndef __KRYSARV__
#define __KRYSARV__

#include "IPlug_include_in_plug_hdr.h"

class KrysarV : public IPlug{
public:
  KrysarV(IPlugInstanceInfo instanceInfo);
  ~KrysarV();
  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** input, double** output, int nFrames);
private:
  double superPre;
  double superSpeed;
  double superDepth;
  double superPost;
  double superSymmetry;
  double superMaster;
  double superCut;
};

#endif
