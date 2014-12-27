#include "FFTspectrogram.h"

namespace khLib
{
FFTspectrogram::FFTspectrogram()
{
    //ctor
}

FFTspectrogram::~FFTspectrogram()
{
    //dtor
}


void FFTspectrogram::allocateSpectrogram(int bufferSize, int binSize, ofImageType imageType)
{
    /* allocate buffer*/
    spectrogram.allocate(bufferSize, binSize, imageType);
    memset(spectrogram.getPixels(), 0, (int)(spectrogram.getWidth() * spectrogram.getHeight()));
    spectrogramOffset = 0;
}

void FFTspectrogram::draw(void)
{

}


float FFTspectrogram::getHeight(void)
{
    return spectrogram.getHeight();
}

float FFTspectrogram::getWidth(void)
{
    return spectrogram.getWidth();
}

unsigned char* FFTspectrogram::getPixels(void)
{
    return spectrogram.getPixels();
}

int FFTspectrogram::getOffset(void)
{
    return spectrogramOffset;
}
void FFTspectrogram::setOffset(int _offset)
{
    spectrogramOffset = _offset;
}

void FFTspectrogram::update()
{
    spectrogram.update();
}

void FFTspectrogram::draw(int x, int y)
{
    spectrogram.draw(x, y);
}
}
