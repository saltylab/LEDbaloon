#include "ArduinoControl.h"
namespace khLib
{
	static std::vector<unsigned char> blightness;

	ArduinoControl::ArduinoControl(void)
		: setupIsDone(false)

	{

	}

	void ArduinoControl::setup(std::string _comPortName, int _baudrate /* ofArduino Default value*/ )
	{ 
		comPortName = _comPortName;
		baudrate = _baudrate; 

		arduino.connect(comPortName, baudrate);
//
//		ofAddListener(arduino.EInitialized, this, &khLib::ArduinoControl::setupAfterInitialize);

		/* For Sysex Data read*/
//		ofAddListener(arduino.ESysExReceived, this, &khLib::ArduinoControl::updateSysExdata);
	}

	void ArduinoControl::setRGBport(int rgbPortNum, RgbPort portSetting)
	{
		rgbPort[rgbPortNum].r.portNum =  portSetting.r.portNum;
		rgbPort[rgbPortNum].g.portNum =  portSetting.g.portNum;
		rgbPort[rgbPortNum].b.portNum =  portSetting.b.portNum;

	}


	void ArduinoControl::setRGBvalue(int rgbPortNum, int r, int g, int b)
	{
		rgbPort[rgbPortNum].r.value =  r;
		rgbPort[rgbPortNum].g.value =  g;
		rgbPort[rgbPortNum].b.value =  b;
	}


	void ArduinoControl::setSysExData(I2Ccommand& commandObj,
						I2CdataType slaveAddress, 
						I2CdataType controlRegisterAddress, 
						std::vector<I2CdataType>& command,
						std::vector<I2CdataType>& data)
	{
		commandObj.setSlaveAddress(slaveAddress);
		commandObj.setControlRegisterAddress(controlRegisterAddress);
		commandObj.setCommand(command);
		commandObj.setData(data);
	}

	void ArduinoControl::setupAfterInitialize(const int & version)
	{

		//ofRemoveListener(arduino.EInitialized, this, &khLib::ArduinoControl::setupAfterInitialize);

		ofLogNotice() << "after initialize called! " << endl;

		/* setup I2C data */	
		commandBuffer.clear();
		commandBuffer.push_back(0x00);
		arduino.sendSysEx(I2C_CONFIG, commandBuffer);

		sysExI2CsendData.clear();
		sysExI2CsendData.resize(3);

		for (int ledDriverNum = 0; ledDriverNum < LED_NUM; ++ledDriverNum)
		{
		
			commandBuffer.clear();
			commandBuffer.push_back((I2CdataType)0x0F);

			/* Mode 1 command settings */
		
			setSysExData(writeCommand, LED_DRIVER_SLAVE_START_ADDRESS + ledDriverNum, Pca9633I2C::ControlRegisterAddress::MODE1, 
						 commandBuffer, sysExI2CsendData);
			arduino.sendSysEx(I2C_REQUEST, sysExI2CsendData);

			/* LEDOUT command setting */
			commandBuffer[0] = (0xFF);
			setSysExData(writeCommand, LED_DRIVER_SLAVE_START_ADDRESS + ledDriverNum, Pca9633I2C::ControlRegisterAddress::LEDOUT, commandBuffer, sysExI2CsendData);
			arduino.sendSysEx(I2C_REQUEST, sysExI2CsendData);
		}
		/* For Debug*/

		sysExI2CsendData.clear();
		sysExI2CsendData.resize(1+2);
		commandBuffer.clear();
		commandBuffer.resize(1);

		//arduino.sendDigitalPinMode(13, ARD_OUTPUT);

#if 0
		for (int i = 0; i < 30; ++i)
		{
			blightness.push_back((unsigned char)(6*i + 30));
		}
#endif //
		setupIsDone = true;
	}

	void ArduinoControl::sendDigitalOutput(int pinNum, int value)
	{
		if (setupIsDone)
		{
			arduino.sendDigital(pinNum, value);
		}
	}

	void ArduinoControl::getSysExData(std::vector<unsigned char>& data)
	{
		/* get Last updated data */
		data = sysExI2CsendData;
	}

	

	void ArduinoControl::update(void)
	{
		static int frameCnt = 0;

		if (setupIsDone)
		{
			arduino.update();

			//if (frameCnt++ >= 15) /* 30 ok, 10 NG, 15NG */
			{
	
				frameCnt = 0;
				for (int ledNum = 0; ledNum < LED_NUM; ++ledNum)
				{			
				for (int i = 0x03; i <= 0x05; ++i)
				{
				/* setup I2C data */	
					/* PWMx command settings */
					switch (i)
					{
						case 3:	/* B */
							commandBuffer[0] = (0xFF - rgbPort[ledNum].b.value);
							break;
						case 4:	/* G */
							commandBuffer[0] = (0xFF - rgbPort[ledNum].g.value);
							break;
						
						case 5:		/* R */	
							commandBuffer[0] = (0xFF - rgbPort[ledNum].r.value);
							break;
						default :
							break;
					}
					setSysExData(writeCommand, LED_DRIVER_SLAVE_START_ADDRESS + ledNum, (I2CdataType)i, commandBuffer, sysExI2CsendData);
					arduino.sendSysEx(I2C_REQUEST, sysExI2CsendData);
				//	ofSleepMillis(1);
				}
			}
			}
		}
	}

	void ArduinoControl::draw(void)
	{
		// do not send anything until the arduino has been set up
		if (setupIsDone) 
		{
		//	if (((frameCnt++) % 10) == 0)
			{			
#if 0
				for (int rgbPortNum = 0; rgbPortNum < (ARDUINO_PWM_PORT_NUM/3); rgbPortNum++)
				{
					arduino.sendPwm(rgbPort[rgbPortNum].r.portNum, rgbPort[rgbPortNum].r.value, false);
					arduino.sendPwm(rgbPort[rgbPortNum].g.portNum, rgbPort[rgbPortNum].g.value, false);
					arduino.sendPwm(rgbPort[rgbPortNum].b.portNum, rgbPort[rgbPortNum].b.value, false);
				}
#endif // 
			}
		}
	}
	void ArduinoControl::requestGetAllReisterValue(void)
	{
		std::vector<unsigned char> allRegisterReq;
		allRegisterReq.push_back(0x00);	/* slave address(LSB)*/
		allRegisterReq.push_back(0x10);	/* slave address(MSB) + Read Continuously*/

		allRegisterReq.push_back(0x80);	/* auto increment*/
		allRegisterReq.push_back(0x00);	/* data (MSB)*/

		allRegisterReq.push_back(0x01);	/* mmm only read bit is enabled*/
		allRegisterReq.push_back(0x00);	/* slave address(MSB)*/

		arduino.sendSysEx(I2C_REQUEST, allRegisterReq);

	
	}

	void ArduinoControl::updateSysExdata(const int& version)
	{
		sysExReceivedData = arduino.getSysEx();
	
	}
};
