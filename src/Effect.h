#pragma once

#include <vector>
#include "IOdefines.h"

typedef struct RGBcolor
{
	float r;
	float g;
	float b;
} RGBcolor;
const int INPUT_DATA_NUM = 512;
const int FFT_SAMPLE_FREQ = 44100;

namespace khLib
{
	
	typedef enum EffectType
	{
		FftColorDivide/* MinamigiSP1 */,
		Shift /*MinamigiSP2*/,
		Waving /* MinamigiSP3*/,
		BPM_MODE, 
		EffectNum
	}EffectType;

	void HSV2RGB( double *rr, double *gg, double *bb, double H, double S, double V );
	
	class Effect 
	{
		public :
			
			Effect(EffectType effectType = FftColorDivide);

			/* �G�t�F�N�g�̎�ނ�ݒ� */
			void setEffectType(EffectType effectType);

			/* ���ݐݒ肳��Ă���G�t�F�N�g�̎�ނ���rgb�̒l���Z�o*/
			void setEffect(float* amplitude, std::vector<float>& fftResult, RGBcolor*  rgbColor);		

		private :
			EffectType type;
	};
};