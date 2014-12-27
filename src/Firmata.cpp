#include "Firmata.h"
#include "ofMain.h"

namespace khLib
{

		I2Ccommand::I2Ccommand(	SlaveAddressType _address, 
								ControlRegisterType _controlRegisterAddress, 
								FirmataReadWriteMode _FirmataReadWriteMode,
								AddressMode _mode) :
			firmataReadWriteMode(_FirmataReadWriteMode),
			addressMode(_mode)
		{
		
		}

		void I2Ccommand::setSlaveAddress(SlaveAddressType address)
		{
			switch (addressMode)
			{
			case MODE_16BIT :
				break;
			
			case MODE_10BIT	:
				address &= 0x03;
				break;
			
			case MODE_7BIT:	/* go through*/
			default :
				address = 0x00;	/* Fix Address*/
			}
			slaveAddress = address;/* MSB is not Ignored*/
		}

		void I2Ccommand::setControlRegisterAddress(ControlRegisterType address)
		{
			controlRegisterAddress = address;
		}


		void I2Ccommand::getData(std::vector<I2CdataType>& data)
		{
			data = commandArray;
		}

		void I2Ccommand::setData(std::vector<I2CdataType>& data)
		{
			/* First Set Address*/
			this->getData(commandArray);

#if 0
			data.clear();
			data.reserve(commandArray.size() + 2);

			data.push_back(this->getSlaveAddress() << 1 | firmataReadWriteMode);	/* first byte‚ð‚·‚×‚Äslave addr‚Æ‚Ý‚È‚·*/

			/* Second, Set control Register*/
			data.push_back(this->getControlRegister());

			/* Firnaly, set Data*/
			for (std::vector<I2CdataType>::iterator it = commandArray.begin(); it != commandArray.end(); ++it)
			{
				data.push_back((*it));
			}
#else
			data[0] = (this->getSlaveAddress() << 1 | firmataReadWriteMode);
			data[1] = this->getControlRegister();
			data[2] = commandArray[0];
#endif // 
		}

		void I2Ccommand::setSlaveAddressMode(AddressMode _mode)
		{
			addressMode = _mode;
		}

		void I2Ccommand::setCommand(std::vector<I2CdataType>& data)
		{
			commandArray = data;
		}
};