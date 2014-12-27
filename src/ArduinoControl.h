#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "Firmata.h"
#include "Pca9633.h"
#include "IOdefines.h"

namespace khLib
{

	typedef struct PWM
	{
		int portNum;
		int value;
	}PWM;


	typedef struct PWMportNum
	{
		PWM r;
		PWM g;
		PWM b;
	} RgbPort;

#if 0	/* for Use LED driver*/
	typedef struct LedDiverData
	{
		RgbPort rgbData;
		int DiverAddress;
	} LedDriver;
#endif // 

	class ArduinoControl
	{
		public :
			ArduinoControl(void);
			void setup(std::string comPortName, int baudrate = 57600 /* ofArduino Default value*/ );
			void setRGBport(int rgbPortNum, RgbPort portSetting);
			void setRGBvalue(int rgbPortNum, int r, int g, int b);
			void setupAfterInitialize(const int& version);
	//		void setupAfterInitialize(void);
			void sendDigitalOutput(int pinNum, int value);
			
			void getSysExData(std::vector<unsigned char>& data);
			void requestGetAllReisterValue(void);

			void update(void);
			void draw(void);
			bool setupIsDone;

			ofArduino arduino;
			
			WriteToSpecificRegister writeCommand;
			std::vector<I2CdataType> sysExI2CsendData;

		private :
			/* This function called by occuring "ESysExReceived" event internally */
			void updateSysExdata(const int& version);

			void setSysExData(I2Ccommand& commandObj,
						I2CdataType slaveAddress, 
						I2CdataType controlRegisterAddress, 
						std::vector<I2CdataType>& command,
						std::vector<I2CdataType>& data);

			std::string comPortName;
			int baudrate;
			std::vector<I2CdataType> sysExReceivedData;
			std::vector<I2CdataType> commandBuffer;

#if 1
			RgbPort rgbPort[LED_NUM];
#else
			RgbPort rgbPort[ARDUINO_PWM_PORT_NUM/3];
			LedDiverData drivers[LED_NUM];	/* For I2C ver.*/
#endif // 

	};

};