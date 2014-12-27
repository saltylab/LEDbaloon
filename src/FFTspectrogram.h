#ifndef FFTSPECTROGRAM_H
#define FFTSPECTROGRAM_H

#include "ofMain.h"
namespace khLib
{

class FFTspectrogram
{
public :
    FFTspectrogram();
    virtual ~FFTspectrogram();
    void allocateSpectrogram(int bufferSize, int binSize, ofImageType imageType);
    void draw(void);
    float getHeight(void);
    float getWidth(void);
    int getOffset(void);
    unsigned char* getPixels();
    void setOffset(int offset);

    void update(void);
    void draw(int x, int y);
private :
    int spectrogramOffset;
    ofImage spectrogram;
};

}
#endif // FFTSPECTROGRAM_H

