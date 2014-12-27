/*
 * This is a simple example use of ofArduino
 *
 * ofArduino currently only supports the standard Arduino boards
 * (UNO, Duemilanove, Diecimila, NG, and other boards based on the
 * ATMega168 or ATMega328 microcontrollers
 * The Arduio FIO and Arduino Mini should also work.
 * The Arduino MEGA and other variants based on microcontrollers
 * other than the ATMega168 and ATMega328 are not currently supported.
 * 
 * To use this example, open Arduino (preferably Arduino 1.0) and 
 * navigate to File -> Examples -> Firmata and open StandardFirmata.
 * Compile and upload StandardFirmata for your board, then close
 * the Arduino application and run this application.
 *
 * If you have a servo attached, press the left arrow key to rotate
 * the servo head counterclockwise and press the right arrow key to
 * rotate the servo head clockwise.
 *
 * Clicking the mouse over any part of the application will turn the
 * on-board LED on and off.
 *
 */

#include "testApp.h"

static float lmax;
static float rmax;
static int freq_r;
static int freq_l;
static unsigned long long lastUpdateTime;
static float previousVolume;

#define LMAX_THRESHOLD 0.001
#define RMAX_THRESHOLD 0.001
#define FREQ_THRESHOLD_WIDTH 50
#define FREQ_THRESHOLD 200



//#define DEBUG_DISPLAY
//--------------------------------------------------------------
RGBcolor rgbColor[LED_NUM];



void testApp::setup(){
	setUpisDone = false;
//	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofBackground(255,0,130);
    ofEnableAlphaBlending();

    buttonState = "digital pin:";
    potValue = "analog pin:"; 

#ifndef DEBUG_DISPLAY
	bgImage.loadImage("background.png");
	font.loadFont("franklinGothic.otf", 20);
    smallFont.loadFont("franklinGothic.otf", 14);
#endif // 
    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
//	ard.connect("/dev/tty.usbmodemfd121", 57600);
	/* rgbport setting*/
	khLib::PWMportNum rgbPort;
	rgbPort.r.portNum	= 3;
	rgbPort.r.value		= 0;

	rgbPort.g.portNum	= 5;
	rgbPort.g.value		= 0;
	
	rgbPort.b.portNum	= 6;
	rgbPort.b.value		= 0;	

	for (int i = 0; i < ARDUINO_BOARD_NUM; ++i)
	{
#if 1
		for (int ledDriverNum = 0; ledDriverNum < LED_NUM; ++ledDriverNum)
		{
			arduino[i].setRGBport(ledDriverNum, rgbPort);
		}
#else
		arduino[i].setRGBport(0, rgbPort);
#endif // 
	}

	rgbPort.r.portNum = 9;
	rgbPort.g.portNum = 10;
	rgbPort.b.portNum = 11;


	for (int i = 0; i < ARDUINO_BOARD_NUM; ++i)
	{
		arduino[i].setRGBport(1, rgbPort);
	}

	arduino[0].setup("COM9", 57600);

#if ARDUINO_BOARD_NUM == 3
	arduino[1].setup("COM10", 57600);
	arduino[2].setup("COM11", 57600);
#endif // 

	/* For Sound Data*/
//	fft.fft->signalNormalized = false;
	left = new float[INPUT_DATA_NUM/2];
	right = new float[INPUT_DATA_NUM/2];

	fft.setup();
//	ofSoundStreamSetup(0, 2, this, FFT_SAMPLE_FREQ, fft.getBufferSize(), 4);
	//	ofSoundStreamSetup(0,2,this, 44100, 256, 4); 

	/* For sound data*/
	fft_result = new float[INPUT_DATA_NUM];
	volume = new float[FFT_VOLUME_NUM];
	for (int i = 0; i < FFT_VOLUME_NUM; ++i)
	{
		volume[i] = 0.0f;
	}
#if 1
//	player.loadSound("come_again.mp3");
//	player.loadSound("aoi.wma");
	player.loadSound("bakarithm.mp3");
	player.setLoop(true);
	player.play();
	currentFFTamplitude.resize(INPUT_DATA_NUM);
#endif // 
//	currentFFTamplitude.resize(fft.getBinSize());

	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
//	ofAddListener(arduino[0].arduino.EInitialized, this, &testApp::setupArduino);
	
}

//--------------------------------------------------------------
void testApp::update(){

#if 1
	soundMutex.lock();
	ofSoundUpdate();
	volume = ofSoundGetSpectrum(FFT_VOLUME_NUM);
	fft_result = ofSoundGetSpectrum(INPUT_DATA_NUM); 
	memcpy(&(currentFFTamplitude[0]), fft_result, sizeof(float) * INPUT_DATA_NUM);
	soundMutex.unlock();
#endif // 
	if (setUpisDone)
	{
		updateArduino();
	}
}

//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
	ofRemoveListener(arduino[0].arduino.EInitialized, this, &testApp::setupArduino);
	//arduino[0].setupAfterInitialize(30);
}



//--------------------------------------------------------------
void testApp::updateArduino(){

	
		// update the arduino, get any data or messages.
    // the call to ard.update() is required

	for (int arduinoNum = 0; arduinoNum < ARDUINO_BOARD_NUM; ++arduinoNum)
	{
			arduino[arduinoNum].update();
	}	
		

	// do not send anything until the arduino has been set up
	#ifndef DEBUG_DISPLAY
	if (bSetupArduino) 
	#endif // 
	{
		{
			
	/* update color*/
	effect.setEffectType(khLib::BPM_MODE);
	effect.setEffect(volume, currentFFTamplitude, rgbColor);	/* for sound data(in PC) */

		/* update baffered(next display) data */
		for (int ledDriverNum = 0; ledDriverNum < LED_NUM; ++ledDriverNum)
		{
			arduino[0].setRGBvalue(ledDriverNum, rgbColor[ledDriverNum].r * 255, rgbColor[ledDriverNum].g * 255, rgbColor[ledDriverNum].b * 255);
		}

		/* Drawing Color*/
		#ifdef DEBUG_DISPLAY
		//ofBackground(0, 0, 0, 0);
		ofEnableAlphaBlending();
		#if 0
		ofSetColor(r * 255 , g * 255 , b * 255, 255);
		#endif //
		ofCircle(ofGetWidth()/2, ofGetHeight(), volume * 0.5);

		#else

		#endif //
		}
	}
}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void testApp::digitalPinChanged(const int & pinNum) {

#if 0
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard[0].getDigital(pinNum));
#endif // 
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void testApp::analogPinChanged(const int & pinNum) {
#if 0
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard[0].getAnalog(pinNum));
#endif // 
}


//--------------------------------------------------------------
void testApp::draw(){

#ifndef DEBUG_DISPLAY
	bgImage.draw(0,0);
    
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, 127);
    ofRect(510, 15, 275, 150);
   ofDisableAlphaBlending();
    
   ofSetColor(255, 255, 255);
#endif // 

#if 0
	for (int arduinoNum = 0; arduinoNum < ARDUINO_BOARD_NUM; ++arduinoNum)
	{
		arduino[arduinoNum].draw();
	}
#endif // 
 
#ifdef DEBUG_DISPLAY
	fft.draw();
#endif // 
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	setUpisDone = false;
	arduino[0].setupAfterInitialize(30);
	setUpisDone = true;
#if 0
    // turn on the onboard LED when the application window is clicked
	for (int i = 0; i < ARDUINO_BOARD_NUM; ++i)
	{
		arduino[i].requestGetAllReisterValue();
		arduino[i].sendDigitalOutput(13, ARD_HIGH);
	}
#endif // 
}


void testApp::exit()
{
	delete [] left;
	delete [] right;
	delete [] fft_result;
	delete [] volume;
}
