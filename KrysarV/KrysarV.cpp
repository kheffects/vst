//Connor K Harrington
//github.com/kheffects

//This program is run via the iPlug SDK. Oli Larken has a wonderful tutorial on getting started.
//Link to tutorial: http://www.martin-finke.de/blog/tags/making_audio_plugins.html

//Attached is the DLL file for using this as a VST2.

#include "KrysarV.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include "OctaveDown.h"
#include "Filter.h"
#include "Delay.h"
#include "Tremolo.h"

//Globals
Filter* InputFilter;
Filter* AmpTreble;
Filter* AmpMids;
Tremolo* Trem;
Delay* Echo;
Filter* DelayFilter;
static std::vector<double> DelayVector;
static std::vector<double> DelayVector2;
static int i;
static int i2;
static int b;
double a0Lo;
double b1Lo;
double delayLo;
Filter* AmpPower;
Filter* AmpOutput;
Filter* MCab1;
Filter* MCab2;
Filter* MCab3;
Filter* MCab4;
Filter* MCab5;
Filter* MCab6;

enum EParams{
  kPre = 0,
  kSpeed,
  kDepth,
  kFeedback,
  kTime,
  kMix,
  kPost,
  kSymmetry,
  kCut,
  kMaster,
  NumParams
};

enum ELayout{
  width = GUI_WIDTH,
  height = GUI_HEIGHT,
};

KrysarV::KrysarV(IPlugInstanceInfo instanceInfo)
:	IPLUG_CTOR(NumParams, 1, instanceInfo), superPre(1.0), superPost(1.0), superSymmetry(1.0), superMaster(1.0), superSpeed(1.0), superDepth(1.0), superCut(1.0){


  //Don't mess with this.
  TRACE;

  //Knob setup
  
  //preamp gain
  GetParam(kPre)->InitDouble("", .1, .0, 10.0, 0.001, "");
  GetParam(kPre)->SetShape(2.);
  //speed of tremolo
  GetParam(kSpeed)->InitDouble("", .000005, .0000001, .0008, .00000001, "");
  GetParam(kSpeed)->SetShape(1.);
  //depth of tremolo
  GetParam(kDepth)->InitDouble("", 0.00, 0.00, 1.00, 0.005, "");
  GetParam(kDepth)->SetShape(1.);
  //delay feedback control
  GetParam(kFeedback)->InitDouble("", 0.0, 0.0, 1.0,0.001, "");
  GetParam(kFeedback)->SetShape(1.);
  //delay mix control
  GetParam(kMix)->InitDouble("", 0.0, 0.0, 1.0,0.001, "");
  GetParam(kMix)->SetShape(1.);
  //delay time control
  GetParam(kTime)->InitDouble("", 0.42, 0.01, .65, 0.01, "");
  GetParam(kTime)->SetShape(1.);
  //poweramp gain
  GetParam(kPost)->InitDouble("", 0.40, 0.0, 0.6, 0.001, "");
  GetParam(kPost)->SetShape(1.);
  //clipping symmetry
  GetParam(kSymmetry)->InitDouble("", 0.7, 0.3, .9, 0.01, "");
  GetParam(kSymmetry)->SetShape(1.);
  //lowpass cut control
  GetParam(kCut)->InitDouble("", 10000.0, 1000.0, 10000.0, 0.1, "");
  GetParam(kCut)->SetShape(2.0);
  //master volume
  GetParam(kMaster)->InitDouble("", 0.5, 0.0, 1.0, 0.001, "");
  GetParam(kMaster)->SetShape(1.);

  //All filters calculated using this tool: http://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/

  //AMP EQ Filters
  InputFilter = new Filter(0.4290831145442384, 0.8581662290884768, 0.4290831145442384, 0.5001855420961149, 0.21614691608083877);
  AmpTreble = new Filter(2.2376543756752447, -2.9073392743062265, 1.0912058231299944, -0.8955830732461522, 0.3171039977451646);
  AmpMids = new Filter(1.0607944578271804, -1.4487165312624912, 0.6444713604256814, -1.4487165312624912, 0.7052658182528619);
  AmpPower = new Filter(1.0768830206095634, -0.2685560020468106, 0.768618975289945, -0.2685560020468106, 0.8455019958995084);
  AmpOutput = new Filter(0.9899759179893742, -1.9799518359787485, 0.9899759179893742, -1.979851353142371, 0.9800523188151258);

  //Cab sim filters
  MCab1 = new Filter(0.9659325150862459,-1.915786164529356,0.9547251062057764,-1.915786164529356,0.9206576212920224);
  MCab2 = new Filter(1.65039052611176, -2.5570842859995473, 1.0458379096547408,-1.407505343915143, 0.5466494936820961);
  MCab3 = new Filter(0.047764392200327466,0.09552878440065493,0.047764392200327466,-1.2936770035370844,0.48473457233839423);
  MCab4 = new Filter(0.05719798682284768,0.11439597364569536,0.05719798682284768,-1.218828499663378, 0.44762044695476855);
  MCab5 = new Filter(2.4968142559030286, -4.09339771041471, 1.7357276042786345, -1.407505343915143, 0.5466494936820961);
  MCab6 = new Filter(0.2569828095848547, 0.5139656191697094, 0.2569828095848547, -0.14748484487229319, 0.175416083211712);

  Trem = new Tremolo(kSpeed, kDepth);
  Echo = new Delay();
  
  //For CUT
  a0Lo = b1Lo = delayLo = 0.0;
  
  IGraphics* pGraphics = MakeGraphics(this, width, height);
  pGraphics->AttachBackground(BACK_ID, BACK_FN);
  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 128);

  //Load controls onto GUI knobs
  pGraphics->AttachControl(new IKnobMultiControl(this, 25, 225, kPre, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 125, 225, kCut, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 225, 225, kSymmetry, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 325, 225, kPost, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 425, 225, kMaster, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 25, 325, kSpeed, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 125, 325, kDepth, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 225, 325, kMix, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 325, 325, kTime, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 425, 325, kFeedback, &knob));

  AttachGraphics(pGraphics);
}

//Default deconstructor
KrysarV::~KrysarV() {}

//Where the main DSP stems from for this amp sim
void KrysarV::ProcessDoubleReplacing(double** input, double** output, int nFrames){

	double cutTemp = 0.0;

     for(int j = 0; j < nFrames; j++){

		 //Cuts off some highs
		 output[0][j] = InputFilter->FilterSample(input[0][j]);

		 //Pre-Amp Gain I (PREAMP)
		 output[0][j] = (1.0 / 6.0 * atan(output[0][j] * 15.0))*superPre + (5.0 / 12.0 * atan(output[0][j] * 4.0))*superPre;

		 //Pre-Amp Tone I
		 output[0][j] = AmpTreble->FilterSample(output[0][j]);

		 //Pre-Amp Gain II
		 output[0][j] = (1 / 12.0 * atan(output[0][j] * 8.0));

		 //Pre-Amp Tone II
		 output[0][j] = AmpMids->FilterSample(output[0][j]);

		 //Power-Amp Gain I (POSTAMP)
		 if (output[0][j] >= 0) {
			 //Upper Amplitube Symmetry
			 output[0][j] = fmin(output[0][j], superPost - superPost*(output[0][j] * (superPre*0.25)));
		 }
		 else {
			 //Lower Amplitude Symmetry (SYMMETRY)
			 output[0][j] = fmax(output[0][j], (-superPost + superPost*(1.0 - superSymmetry)));
		 }

		 //Brings volume for symmetry changes
		 output[0][j] /= (superPost - (superPost*(1.0 - superSymmetry)) / 12.4);

		 //TREMOLO
		 output[0][j] = Trem->TremoloSample(output[0][j]);

		 //Power-Amp EQ I
		 output[0][j] = AmpPower->FilterSample(output[0][j]);

		 //Power-Amp Gain II
		 output[0][j] = (1 / 16.0 * atan(output[0][j] * 16.0));
		 output[0][j] /= .1;

		 //DELAY
		 output[0][j] = Echo->DelaySample(output[0][j]);

		 //CUT
		 cutTemp = output[0][j];
		 output[0][j] = a0Lo*cutTemp - b1Lo*delayLo;
		 delayLo = output[0][j];
		 cutTemp = output[0][j];
		 output[0][j] = a0Lo*cutTemp - b1Lo*delayLo;
		 delayLo = output[0][j];
		 cutTemp = output[0][j];
		 output[0][j] = a0Lo*cutTemp - b1Lo*delayLo;
		 delayLo = output[0][j];
		 cutTemp = output[0][j];
		 output[0][j] = a0Lo*cutTemp - b1Lo*delayLo;
		 delayLo = output[0][j];

		 //Power-Amp EQ II
		 output[0][j] = AmpOutput->FilterSample(output[0][j]);

		 //Cab sim
		 output[0][j] = MCab6->FilterSample(MCab5->FilterSample(MCab4->FilterSample(MCab3->FilterSample(MCab2->FilterSample(MCab1->FilterSample(output[0][j]))))));

		 //Master volume
		 output[0][j] = output[0][j] * superMaster;

		 //Swap other side to make mono
		 output[1][j] = output[0][j];
  }
}


void KrysarV::Reset(){

  TRACE;

  IMutexLock lock(this);
}

//When a parameter or knob is changed, it goes here, where we can update our private master variables
void KrysarV::OnParamChange(int paramChange){

  IMutexLock lock(this);

  double tmp;

  //Depending on what parameter was changed, our private variable will be altered.
  switch (paramChange){

	case kPre:
		superPre = GetParam(kPre)->Value();
		break;
	case kSpeed:
		Trem->SetSpeed(GetParam(kSpeed)->Value());
		break;
	case kDepth:
		Trem->SetDepth(GetParam(kDepth)->Value());
		break;
	case kTime:
		Echo->SetTime(GetParam(kTime)->Value());
		break;
	case kMix:
		Echo->SetMix(GetParam(kMix)->Value());
		break;
	case kFeedback:
		Echo->SetFeedback(GetParam(kFeedback)->Value());
		break;
	case kPost:
		superPost = 1.0 - GetParam(kPost)->Value();
		break;
	case kSymmetry:
		superSymmetry = GetParam(kSymmetry)->Value();
		break;
	case kCut:
		//Recalculates a lowpass inline
		tmp = exp(-2.0*PI*GetParam(kCut)->Value() / 44100.0);
		a0Lo = 1.0 - tmp;
		b1Lo = -tmp;
		break;
	case kMaster:
		superMaster = GetParam(kMaster)->Value();
		break;
    default:
		break;
  }
}

