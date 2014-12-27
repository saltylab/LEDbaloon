#pragma once
#include <vector>

namespace khLib
{

		typedef unsigned char I2CdataType;
		typedef I2CdataType SlaveAddressType;
		typedef I2CdataType ControlRegisterType;


		const int FirmataSlaveMSBModeField = 0;
		const int FirmataSlaveMSBmask = ((1 << 3) - 1);

		const int FiramateReadWriteField = 3;
		const int FirmataReadWriteMask = ((1 << 5) - 1) - FirmataSlaveMSBmask;
		

		typedef enum FirmataReadWriteMode
		{
			FIRMATA_MODE_WRITE = 0x00,
			FIRMATA_MODE_READ = 0x01,
			FIRMATA_MODE_READ_CONTINUOUS = 0x02,
			FIRMATA_MODE_STOP_READING = 0x03
		}FirmataReadWriteMode;

		typedef enum AddressMode
		{
			MODE_7BIT = 0x00,
			MODE_10BIT = 0x01,
			MODE_16BIT
		}AddressMode;
		
		typedef enum CommandType
		{
			WRITE = 0x00,
			READ = 0x01,
		}CommandType;

		/* */
		class I2Ccommand
		{
			public :
				I2Ccommand(	SlaveAddressType slaveAddress = 0x00, 
							ControlRegisterType controlRegisterAddress = 0x00, 
							FirmataReadWriteMode FirmataReadWriteMode = FirmataReadWriteMode::FIRMATA_MODE_WRITE,
							AddressMode mode = MODE_16BIT);
				//virtual ~I2Ccommand();

				void setSlaveAddress(SlaveAddressType address);	/* 16bit mode */
				void setControlRegisterAddress(ControlRegisterType address);
				void setData(std::vector<I2CdataType>& data);
				void setSlaveAddressMode(AddressMode mode);
				void setCommand(std::vector<I2CdataType>& data);

				SlaveAddressType getSlaveAddress(void){return slaveAddress;}
				ControlRegisterType getControlRegister(void){return controlRegisterAddress;}
				void getData(std::vector<I2CdataType>& data);

			protected :
				SlaveAddressType slaveAddress;
				AddressMode addressMode;
				ControlRegisterType controlRegisterAddress;
				std::vector<I2CdataType> commandArray; /* It shows data stream for I2C command*/
				unsigned char firmataReadWriteMode;
		};
};
