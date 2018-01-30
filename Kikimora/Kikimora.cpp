#include "Kikimora.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include "Osc.h"
#include "Filter.h"

Filter* LowPass;
Osc* Synth;
static double IteratorSynthCounter;
static int SynthPitchIterator, pitchMin, pitchMax;

enum EParams{
  speed = 0,
  pitch1,
  pitch2,
  pitch3,
  pitch4,
  volume,
  pitch5,
  pitch6,
  pitch7,
  pitch8,
  vibrato,
  waveform,
  NumParams
};

enum ELayout{
  width = GUI_WIDTH,
  height = GUI_HEIGHT,
  speedX = 50,
  speedY = 50,
  pitch1X = 200,
  pitch1Y = 50,
  pitch2X = 350,
  pitch2Y = 50,
  pitch3X = 500,
  pitch3Y = 50,
  pitch4X = 650,
  pitch4Y = 50,
  volumeX = 50,
  volumeY = 200,
  pitch5X = 200,
  pitch5Y = 200,
  pitch6X = 350,
  pitch6Y = 200,
  pitch7X = 500,
  pitch7Y = 200,
  pitch8X = 650,
  pitch8Y = 200,
  vibratoX = 125,
  vibratoY = 300,
  waveformX = 575,
  waveformY = 300
};

Kikimora::Kikimora(IPlugInstanceInfo instanceInfo)
:	IPLUG_CTOR(NumParams, 1, instanceInfo), SUPER_SPEED(1.0), SUPER_PITCH1(1.0), SUPER_PITCH2(1.0), SUPER_PITCH3(1.0), SUPER_PITCH4(1.0), SUPER_PITCH5(1.0), SUPER_PITCH6(1.0), SUPER_PITCH7(1.0), SUPER_PITCH8(1.0), SUPER_VOLUME(1.0){
  TRACE;

  pitchMin = 18;
  pitchMax = 1152;

  GetParam(vibrato)->InitDouble("", 1, 1, 10, 0.1,"");
  GetParam(vibrato)->SetShape(1.0);
  GetParam(waveform)->InitInt("", 2, 1, 2, "");
  GetParam(waveform)->SetShape(1.0);
  GetParam(speed)->InitInt("", 1, 1, 30, "0 BPM");
  GetParam(speed)->SetShape(1.0);
  GetParam(volume)->InitDouble("", 4, 0, 10, 0.1, "VOLUME");
  GetParam(volume)->SetShape(1.0);
  GetParam(pitch1)->InitInt("", 100, pitchMin, pitchMax, "");
  GetParam(pitch1)->SetShape(1.0);
  GetParam(pitch2)->InitInt("", 50, pitchMin, pitchMax, "");
  GetParam(pitch2)->SetShape(1.0);
  GetParam(pitch3)->InitInt("", 150, pitchMin, pitchMax, "");
  GetParam(pitch3)->SetShape(1.0);
  GetParam(pitch4)->InitInt("", 200, pitchMin, pitchMax, "");
  GetParam(pitch4)->SetShape(1.0);
  GetParam(pitch5)->InitInt("", 100, pitchMin, pitchMax, "");
  GetParam(pitch5)->SetShape(1.0);
  GetParam(pitch6)->InitInt("", 200, pitchMin, pitchMax, "");
  GetParam(pitch6)->SetShape(1.0);
  GetParam(pitch7)->InitInt("", 120, pitchMin, pitchMax, "");
  GetParam(pitch7)->SetShape(1.0);
  GetParam(pitch8)->InitInt("", 200, pitchMin, pitchMax, "");
  GetParam(pitch8)->SetShape(1.0);

  SUPER_PITCH1 = (int)GetParam(pitch1)->Value();
  SUPER_PITCH2 = (int)GetParam(pitch2)->Value();
  SUPER_PITCH3 = (int)GetParam(pitch3)->Value();
  SUPER_PITCH4 = (int)GetParam(pitch4)->Value();
  SUPER_PITCH5 = (int)GetParam(pitch5)->Value();
  SUPER_PITCH6 = (int)GetParam(pitch6)->Value();
  SUPER_PITCH7 = (int)GetParam(pitch7)->Value();
  SUPER_PITCH8 = (int)GetParam(pitch8)->Value();
  SUPER_VOLUME = GetParam(volume)->Value() / 50.0;

  Synth = new Osc(2, 100.0, 44100.0);
  Synth->SetDepth(.035);
  Synth->SetPitch(100.0);
  IteratorSynthCounter = 0.0;
  SynthPitchIterator = -1;

  IGraphics* pGraphics = MakeGraphics(this, width, height);
  pGraphics->AttachBackground(BACK_ID, BACK_FN);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 128);
  IBitmap knob2 = pGraphics->LoadIBitmap(KNOB_IDP, KNOB_FNP, 2);
  pGraphics->AttachControl(new IKnobMultiControl(this, speedX, speedY, speed, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch1X, pitch1Y, pitch1, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch2X, pitch2Y, pitch2, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch3X, pitch3Y, pitch3, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch4X, pitch4Y, pitch4, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch5X, pitch5Y, pitch5, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch6X, pitch6Y, pitch6, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch7X, pitch7Y, pitch7, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, pitch8X, pitch8Y, pitch8, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, volumeX, volumeY, volume, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, vibratoX, vibratoY, vibrato, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, waveformX, waveformY, waveform, &knob2));

  AttachGraphics(pGraphics);
}

Kikimora::~Kikimora() {}

void Kikimora::ProcessDoubleReplacing(double** input, double** output, int nFrames){
     for(int j = 0; j < nFrames; j++){
			 IterateSynth();
			 output[0][j] = Synth->OscGenerateSample();
			 output[0][j] *= SUPER_VOLUME;
		 output[1][j] = output[0][j];
  }
}

void Kikimora::IterateSynth() {
	if (IteratorSynthCounter > 1.0) {
		IteratorSynthCounter = 0.0;
		ChangeSynthPitch();
	}
	IteratorSynthCounter += SUPER_SPEED;
}

void Kikimora::ChangeSynthPitch() {
	SynthPitchIterator++;
	switch (SynthPitchIterator) {
		case 0:
			Synth->SetPitch(SUPER_PITCH1);
			break;
		case 1:
			Synth->SetPitch(SUPER_PITCH2);
			break;
		case 2:
			Synth->SetPitch(SUPER_PITCH3);
			break;
		case 3:
			Synth->SetPitch(SUPER_PITCH4);
			break;
		case 4:
			Synth->SetPitch(SUPER_PITCH5);
			break;
		case 5:
			Synth->SetPitch(SUPER_PITCH6);
			break;
		case 6:
			Synth->SetPitch(SUPER_PITCH7);
			break;
		case 7:
			Synth->SetPitch(SUPER_PITCH8);
			break;
	}
	if (SynthPitchIterator >= 7) {
		SynthPitchIterator = -1;
	}
}

void Kikimora::Reset(){
  TRACE;
  IMutexLock lock(this);
}

void Kikimora::OnParamChange(int paramChange){
  IMutexLock lock(this);
  switch (paramChange){
    case vibrato:
		Synth->SetVibrato(GetParam(vibrato)->Value());
		break;
	case waveform:
		Synth->SetWaveform((int)GetParam(waveform)->Value());
		break;
    case speed:
        SUPER_SPEED = ((20*GetParam(speed)->Value())/60) / 44100.0;
        break;
	case volume:
		SUPER_VOLUME = GetParam(volume)->Value() / 50.0;
		break;
	case pitch1:
		SUPER_PITCH1 = (int)GetParam(pitch1)->Value();
		break;
	case pitch2:
		SUPER_PITCH2 = (int)GetParam(pitch2)->Value();
		break;
	case pitch3:
		SUPER_PITCH3 = (int)GetParam(pitch3)->Value();
		break;
	case pitch4:
		SUPER_PITCH4 = (int)GetParam(pitch4)->Value();
		break;
	case pitch5:
		SUPER_PITCH5 = (int)GetParam(pitch5)->Value();
		break;
	case pitch6:
		SUPER_PITCH6 = (int)GetParam(pitch6)->Value();
		break;
	case pitch7:
		SUPER_PITCH7 = (int)GetParam(pitch7)->Value();
		break;
	case pitch8:
		SUPER_PITCH8 = (int)GetParam(pitch8)->Value();
		break;
    default:
		break;
  }
}

