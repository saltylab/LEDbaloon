#ifndef FFT_H
#define FFT_H


#include "ofxFft.h"
#include "FFTspectrogram.h"

namespace khLib
{



class FFT
{
    public:
        FFT(int bufferSize = 512, int plotHeight = 128);
        virtual ~FFT();
        void setup(void);
        void update(void);
        void draw(void);
        void audioReceived(float* input, int bufferSize, int nChannels);
        int getBufferSize(void);
		int getBinSize(void);
		float* getAmplitude(void);
		float getAmplitudeAtFrequency(float freq);
		   /* FFT*/
    ofxFft* fft;

    protected:
    private:
        void plot(vector<float>& buffer, float scale, float offset);

 
    int binSize;

    /* For FFT result Image*/
    /* If do not draw Spectogram, this member is not necessary.*/
    FFTspectrogram spectrogram;

    /* application window size*/
    int appWidth, appHeight;

    ofMutex soundMutex;

    /* for Data bin */
    int plotHeight, bufferSize;

    vector<float> drawBins, middleBins, audioBins;
    vector<float> drawBuffer, middleBuffer, audioBuffer;

};
}
#endif // FFT_H
