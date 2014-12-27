#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "FFT.h"

#include "IOdefines.h"
#include "Effect.h"

#include "ArduinoControl.h"
#define ARDUINO_BOARD_NUM 1
#define ARDUINO_SETTING_BOARD_NUM 0

//#define DEBUG_DISPLAY


class testApp : public ofBaseApp{

public:
	static void (*rgbTable[khLib::EffectNum])(float* amplitude, float* fftResult, RGBcolor*  rgbColor);
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void exit();
	static bool isKeyPressed();
	static bool keyPressed;
	ofImage				bgImage;
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	bool setUpisDone;

	khLib::ArduinoControl arduino[ARDUINO_BOARD_NUM];
	bool		bSetupArduino;			// flag variable for setting up arduino once
    // Sound
	float * left;
	float * right;
	float * fft_result;
	float *volume;
	float previousVolume;

	khLib::FFT fft;
	std::vector<float> currentFFTamplitude;

	void setupArduino(const int & version);
	khLib::Effect effect;
	ofSoundPlayer player;

private:
    
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();
    
    string buttonState;
    string potValue;


	ofMutex soundMutex;	/* For Sound Buffer*/
};

