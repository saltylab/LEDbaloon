#pragma once
#include "Firmata.h"

namespace khLib
{


			class Pca9633I2C : public I2Ccommand
			{
					public :
						Pca9633I2C();
						//virtual ~Pca9633I2C();
						ControlRegisterType getControlRegister(void);

					private :
				
					public :
					/** Register definitions */
					typedef enum ControlRegisterAddress
					{
						MODE1		=	0x00,
						MODE2		=	0x01,
						PWM0		=	0x02,
						PWM1		=	0x03,
						PWM2		=	0x04,
						PWM3		=	0x05,
						GRPPWM		=	0x06,
						GRPFREQ		=	0x07,
						LEDOUT		=	0x08,
						SUBADR1		=	0x09,
						SUBADR2		=	0x0a,
						SUBADR3		=	0x0B,
						ALLCALLADR	=	0x0c,
						/* Other Mode is not Supported yet...*/
					}ControlRegisterAddress;		

					static const int AutoIncrementBit = 5;

					enum ControlRegisterMode
					{
						NO_AUTO_INCREMENT							= 0x00,
						AUTO_INCREMENT_FOR_ALL						= 0x04,
						AUTO_INCREMENT_FOR_INDIVISUAL_BRIGHTNESS	= 0x05,
						AUTO_INCREMENT_GLOBAL_REGISTERS_ONLY		= 0x06,	
					};

			};

			class WriteToSpecificRegister : public Pca9633I2C
			{
				public :
				private :
			};



#if 0	/* Not Supported*/
			class WriteToAllRegistersWithAllIncrement : public Pca9633I2C
			{
			
			};

			class ReadAllRegistersWithAllIncrement : public Pca9633I2C
			{
			
			};

#endif // 
};
