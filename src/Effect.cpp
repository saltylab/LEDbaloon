#include "Effect.h"
#include "ofMain.h"

namespace khLib
{
#define VOLUME_BPM_THRESHOLD 0.2

//#define VOLUME_BPM_THRESHOLD 5
	 

void HSV2RGB( double *rr, double *gg, double *bb, double H, double S, double V )
{
    int in;
    double fl;
    double m, n;
    in = (int)floor( H / 60 );
    fl = ( H / 60 ) - in;
    if( !(in & 1)) fl = 1 - fl; // if i is even
 
    m = V * ( 1 - S );
    n = V * ( 1 - S * fl );
    switch( in ){
       case 0: *rr = V; *gg = n; *bb = m; break;
       case 1: *rr = n; *gg = V; *bb = m; break;
       case 2: *rr = m; *gg = V; *bb = n; break;
       case 3: *rr = m; *gg = n; *bb = V; break;
       case 4: *rr = n; *gg = m; *bb = V; break;
       case 5: *rr = V; *gg = m; *bb = n; break;
    }
}


const int  FREQ_DELTA = FFT_SAMPLE_FREQ / INPUT_DATA_NUM / 2;



bool isColorChange(std::vector<float>& fftResult)
{
	static float previousVolume = 0.0;
	bool isChanged = false;

#if 1
	double max = fftResult[0];
	int max_index;
	for (int i = 0; i < INPUT_DATA_NUM; ++i)
	{
		if (max < fftResult[i])
		{
			max = fftResult[i];
			max_index = i;
		}
	}
	ofLogNotice() << max_index << ": " << max;
#endif // 

	if ((max > previousVolume) && (fabs(previousVolume - max) >= VOLUME_BPM_THRESHOLD))
	{
		ofLogNotice() << "update: " << max << " " << previousVolume; 

		isChanged = true;
	}

	previousVolume = max;
	return isChanged;
}

typedef struct HSV
{
	double h;
	double s;
	double v;
}HSV;

void bpmRgb(float* amplitude, std::vector<float>& fftResult, RGBcolor*  rgbColor)
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	static HSV hsvData[LED_NUM];

	double h, s, v;
	static bool isWhite = false;
	static unsigned int frameCnt = 0;
	bool isUpdate = false;
	v = 1.0;
	frameCnt++;


//#ifdef DEBUG_DISPLAY


	
	frameCnt++;
	isUpdate = isColorChange(fftResult);

		/** Setting of the Color */
		#ifndef DEBUG_DISPLAY
		for (int rgbPortIndex = 0; rgbPortIndex < LED_NUM; ++rgbPortIndex)
		#endif // 
		{
			h = hsvData[rgbPortIndex].h;
			s = hsvData[rgbPortIndex].s;
			
			if (isUpdate)
			{
			//	if (++frameCnt >= (ofRandom(1, 3) * 60))
				//if (isUpdate)
				{
				#ifdef DEBUG_DISPLAY
				ofBackground(0, 0, 0, 0);
				#endif //
					frameCnt = 0;
					/* HSV -> RGB*/
					s = 1.0;	/* Satulation is always MAX value. We determine only hue angle. */
					h = ( 360 / 6.0 ) * ofRandom(0, 6) + 10;
					//h = (360/6.0) * (frameCnt % 7) + 10;
				}
#if 0

#endif // 
			} 
			else
			{
				/* fade color */
				s *= fabs(sin(0.1 * ofGetElapsedTimef()));		
			}

			khLib::HSV2RGB(&r, &g, &b, h, s, v);

#ifdef DEBUG_WITH_KEY	/* key bordÇ≈ÇÃèàóùà»äOå¸Ç±Ç§*/
			if (testApp::isKeyPressed() == false)
			{
				rgbColor[rgbPortIndex].r = 0;
				rgbColor[rgbPortIndex].g = 0;
				rgbColor[rgbPortIndex].b = 0;
			}
			else
			{
			rgbColor[rgbPortIndex].r = r;
			rgbColor[rgbPortIndex].g = g;
			rgbColor[rgbPortIndex].b = b;			
			}
#else
			rgbColor[rgbPortIndex].r = r;
			rgbColor[rgbPortIndex].g = g;
			rgbColor[rgbPortIndex].b = b;
#endif // 
			hsvData[rgbPortIndex].h = h;
			hsvData[rgbPortIndex].s = s;
			hsvData[rgbPortIndex].v = v;
		}

}
	Effect::Effect(EffectType effectType) : type(effectType)
	{
	
	}


	void Effect::setEffectType(EffectType effectType)
	{
		type = effectType;
	}

	void Effect::setEffect(float* amplitude, std::vector<float>& fftResult, RGBcolor*  rgbColor)
	{
		switch (type)
		{

		case BPM_MODE:
			bpmRgb(amplitude,fftResult,rgbColor);

			break;
		default :
			bpmRgb(amplitude,fftResult,rgbColor);
			break;

		}
			
	}



};