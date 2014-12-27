#include "FFT.h"

namespace khLib
{
FFT::FFT(int _bufferSize, int _plotHeight) :
    bufferSize(_bufferSize),
    plotHeight(_plotHeight)
{
    //ctor
}

FFT::~FFT()
{
    //dtor
}

void FFT::setup(void)
{
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_BASIC);
    binSize = fft->getBinSize();
    spectrogram.allocateSpectrogram(bufferSize, binSize, OF_IMAGE_GRAYSCALE);

    drawBuffer.resize(bufferSize);
    middleBuffer.resize(bufferSize);
    audioBuffer.resize(bufferSize);

    drawBins.resize(binSize);
    middleBins.resize(binSize);
    audioBins.resize(binSize);


    appWidth = ofGetWidth();
    appHeight = ofGetHeight();
}

void FFT::update(void)
{

}

void FFT::plot(vector<float>& buffer, float scale, float offset)
{
   	ofNoFill();
	int n = buffer.size();
	/* draw frame */
	ofRect(0, 0, n, plotHeight);
	glPushMatrix();


	glTranslatef(0, plotHeight / 2 + offset, 0);
	ofBeginShape();
	for (int i = 0; i < n; i++) {
		ofVertex(i, buffer[i] * scale);
	}
	ofEndShape();
	glPopMatrix();
}
void FFT::draw(void)
{
    ofPushMatrix();
    ofTranslate(16, 16);
    ofDrawBitmapString("Time Domain", 0, 0);

    /* update draw object*/
 	soundMutex.lock();
	drawBuffer = middleBuffer;
	drawBins = middleBins;
	soundMutex.unlock();

    /* draw Time Domain Data*/
    plot(drawBuffer, plotHeight / 2, 0);


    /* draw Frequency Domain*/
    ofTranslate(0, plotHeight + 16);
    ofDrawBitmapString("Frequency Domain", 0, 0);
    plot(drawBins, -plotHeight, plotHeight / 2);

    /* draw Spectrogram*/
    ofTranslate(0, plotHeight + 16);
    spectrogram.update();
    spectrogram.draw(0, 0);

    ofRect(0, 0, bufferSize, bufferSize /2);
    ofDrawBitmapString("Spectrogram", 0, 0);
    ofPopMatrix();

    string msg = ofToString((int) ofGetFrameRate()) + "fps";

    ofDrawBitmapString(msg, appWidth - 80, appHeight - 20);
}

int FFT::getBufferSize()
{
    return bufferSize;
}

float powFreq(float i)
{
    return powf(i, 3);
}
void FFT::audioReceived(float*input, int bufferSize, int nChannels)
{
    /* buffering input data */
    // if use mic, following state is needed
     memcpy(&audioBuffer[0], input, sizeof(float) * bufferSize);
    //



    fft->setSignal(&audioBuffer[0]);

    /* set histogram*/
    float* curFft = fft->getAmplitude();
    memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());

    float maxValue = 0;
    for (int i = 0; i < fft->getBinSize(); ++i)
    {
        if(abs(audioBins[i]) > maxValue)
        {
            maxValue = abs(audioBins[i]);
        }
    }

    /* normalize */
    for (int i = 0; i < fft->getBinSize(); ++i)
    {
        audioBins[i] /= maxValue;
    }

    int spectrogramWidth = (int) spectrogram.getWidth();
    int n = (int) spectrogram.getHeight();

    unsigned char* pixels = spectrogram.getPixels();
    int offset = spectrogram.getOffset();

    for (int i = 0; i < n; i ++)
    {
        int j = (n - i - 1) * spectrogramWidth + offset;
        int logi = ofMap(powFreq(i), powFreq(0), powFreq(n), 0, n); /* map to amplitude [0, n]*/
        pixels[j] = (unsigned char)(255. * audioBins[logi]);
    }

    spectrogram.setOffset((offset + 1) % spectrogramWidth);

    /* update middle buffer*/
	soundMutex.lock();
	middleBuffer = audioBuffer;
	middleBins = audioBins;
	soundMutex.unlock();

}


int FFT::getBinSize(void)
{
	return fft->getBinSize();
}


float* FFT::getAmplitude(void)
{
	return fft->getAmplitude();
}

float FFT::getAmplitudeAtFrequency(float freq)
{
	return fft->getAmplitudeAtFrequency(freq);
}

}
