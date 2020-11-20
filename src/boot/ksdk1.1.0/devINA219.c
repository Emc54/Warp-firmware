#include <stdlib.h>

#include "fsl_misc_utilities.h"
#include "fsl_device_registers.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_spi_master_driver.h"
#include "fsl_rtc_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_power_manager.h"
#include "fsl_mcglite_hal.h"
#include "fsl_port_hal.h"

#include "gpio_pins.h"
#include "SEGGER_RTT.h"
#include "warp.h"

extern volatile WarpI2CDeviceState deviceINA219State;
extern volatile uint32_t		gWarpI2cBaudRateKbps;
extern volatile uint32_t		gWarpI2cTimeoutMilliseconds;
extern volatile uint32_t		gWarpSupplySettlingDelayMilliseconds;

void
initINA219(const uint8_t i2cAddress, WarpI2CDeviceState volatile *  deviceStatePointer)
{

	//Not needed, the enableI2Cpins function does this
	//PORT_HAL_SetMuxMode(PORTB_BASE, 3, kPortMuxAlt2);
	//PORT_HAL_SetMuxMode(PORTB_BASE, 4, kPortMuxAlt2);
	
	
	deviceStatePointer->i2cAddress	= i2cAddress;
	deviceStatePointer->signalType	= kWarpTypeMaskShuntVoltage;
	
	return;
}

WarpStatus
writeSensorRegisterINA219(uint8_t deviceRegister, uint16_t payload, uint16_t menuI2cPullupValue)
{
	uint8_t		payloadByte[2], commandByte[1];
	i2c_status_t	returnValue;

	switch (deviceRegister)
	{
		case 0x00: case 0x05:
		{
			/* OK */
			break;
		}
		
		default:
		{
			return kWarpStatusBadDeviceCommand;
		}
	}

	i2c_device_t slave =
	{
		.address = deviceINA219State.i2cAddress,
		.baudRate_kbps = gWarpI2cBaudRateKbps
	};

	commandByte[0] = deviceRegister;
	payloadByte[0] = payload & 0xFF; //MSB
	payloadByte[1] = (payload >>8);  //LSB
	returnValue = I2C_DRV_MasterSendDataBlocking(
							0 /* I2C instance */,
							&slave,
							commandByte,
							1,
							payloadByte,
							2,
							gWarpI2cTimeoutMilliseconds);
	if (returnValue != kStatus_I2C_Success)
	{
		return kWarpStatusDeviceCommunicationFailed;
	}

	return kWarpStatusOK;
}

WarpStatus
configureSensorINA219(uint16_t payloadCTRL_CON, uint16_t payloadCTRL_CAL, uint16_t menuI2cPullupValue)
{
	WarpStatus	i2cWriteStatus1, i2cWriteStatus2;

	i2cWriteStatus1 = writeSensorRegisterINA219(0x00 /* register address for the Configuration Register */,
							payloadCTRL_CON /* payload */,
							menuI2cPullupValue);

	i2cWriteStatus2 = writeSensorRegisterINA219(0x05 /* register address for the Calibration Register */,
							payloadCTRL_CAL /* payload */,
							menuI2cPullupValue);

	return (i2cWriteStatus1 | i2cWriteStatus2);
}

WarpStatus
readSensorRegisterINA219(uint8_t deviceRegister, int numberOfBytes)
{
	uint8_t		cmdBuf[1]	= {0xFF};
	i2c_status_t	status;


	USED(numberOfBytes);
	i2c_device_t slave =
	{
		.address = deviceINA219State.i2cAddress,
		.baudRate_kbps = gWarpI2cBaudRateKbps
	};

	cmdBuf[0] = deviceRegister;

	status = I2C_DRV_MasterReceiveDataBlocking(
							0 /* I2C peripheral instance */,
							&slave,
							cmdBuf,
							1,
							(uint8_t *)deviceINA219State.i2cBuffer,
							numberOfBytes,
							gWarpI2cTimeoutMilliseconds);

	if (status != kStatus_I2C_Success) 
	{
		return kWarpStatusDeviceCommunicationFailed;
	}

	return kWarpStatusOK;
}

void
printSensorDataINA219(bool hexModeFlag)
{
	uint16_t	readSensorRegisterValueLSB;
	uint16_t	readSensorRegisterValueMSB;
	int16_t		readSensorRegisterValueCombined;
	WarpStatus	i2cReadStatus;


	i2cReadStatus = readSensorRegisterINA219(0x01, 2 /* numberOfBytes */); //Shunt Voltage
	readSensorRegisterValueMSB = deviceINA219State.i2cBuffer[0];
	readSensorRegisterValueLSB = deviceINA219State.i2cBuffer[1];
	readSensorRegisterValueCombined = ((readSensorRegisterValueMSB & 0xFF) << 8) | (readSensorRegisterValueLSB & 0xFF);

	/*
	 *	NOTE: Here, we don't need to manually sign extend since we are packing directly into an int16_t
	 */

	if (i2cReadStatus != kWarpStatusOK)
	{
		SEGGER_RTT_WriteString(0, " ----,");
	}
	else
	{
		if (hexModeFlag)
		{
			SEGGER_RTT_printf(0, " 0x%02x 0x%02x,", readSensorRegisterValueMSB, readSensorRegisterValueLSB);
		}
		else
		{
			SEGGER_RTT_printf(0, " %d,", readSensorRegisterValueCombined);
		}
	}


	i2cReadStatus = readSensorRegisterINA219(0x02, 2 /* numberOfBytes */); //Bus Voltage
	readSensorRegisterValueMSB = deviceINA219State.i2cBuffer[0];
	readSensorRegisterValueLSB = deviceINA219State.i2cBuffer[1];
	readSensorRegisterValueCombined = ((readSensorRegisterValueMSB & 0xFF) << 8) | (readSensorRegisterValueLSB & 0xFF);

	/*
	 *	NOTE: Here, we don't need to manually sign extend since we are packing directly into an int16_t
	 */

	if (i2cReadStatus != kWarpStatusOK)
	{
		SEGGER_RTT_WriteString(0, " ----,");
	}
	else
	{
		if (hexModeFlag)
		{
			SEGGER_RTT_printf(0, " 0x%02x 0x%02x,", readSensorRegisterValueMSB, readSensorRegisterValueLSB);
		}
		else
		{
			SEGGER_RTT_printf(0, " %d,", readSensorRegisterValueCombined);
		}
	}


	i2cReadStatus = readSensorRegisterINA219(0x03, 2 /* numberOfBytes */); //Current 
	readSensorRegisterValueMSB = deviceINA219State.i2cBuffer[0];
	readSensorRegisterValueLSB = deviceINA219State.i2cBuffer[1];
	readSensorRegisterValueCombined = ((readSensorRegisterValueMSB & 0xFF) << 8) | (readSensorRegisterValueLSB & 0xFF);

	/*
	 *	NOTE: Here, we don't need to manually sign extend since we are packing directly into an int16_t
	 */

	if (i2cReadStatus != kWarpStatusOK)
	{
		SEGGER_RTT_WriteString(0, " ----,");
	}
	else
	{
		if (hexModeFlag)
		{
			SEGGER_RTT_printf(0, " 0x%02x 0x%02x,", readSensorRegisterValueMSB, readSensorRegisterValueLSB);
		}
		else
		{
			SEGGER_RTT_printf(0, " %d,", readSensorRegisterValueCombined);
		}
	}


	i2cReadStatus = readSensorRegisterINA219(0x04, 2 /* numberOfBytes */); //Power 
	readSensorRegisterValueMSB = deviceINA219State.i2cBuffer[0];
	readSensorRegisterValueLSB = deviceINA219State.i2cBuffer[1];
	readSensorRegisterValueCombined = ((readSensorRegisterValueMSB & 0xFF) << 8) | (readSensorRegisterValueLSB & 0xFF);

	/*
	 *	NOTE: Here, we don't need to manually sign extend since we are packing directly into an int8_t
	 */

	if (i2cReadStatus != kWarpStatusOK)
	{
		SEGGER_RTT_WriteString(0, " ----,");
	}
	else
	{
		if (hexModeFlag)
		{
			SEGGER_RTT_printf(0, " 0x%02x 0x%02x,", readSensorRegisterValueMSB, readSensorRegisterValueLSB);
		}
		else
		{
			SEGGER_RTT_printf(0, " %d,", readSensorRegisterValueCombined);
		}
	}
}


///USEFUL ALREADY UNDERNEATH///

int
readLATSensorRegisterINA219(uint8_t deviceRegister, int numberOfBytes)
{
	uint8_t			cmdBuf[1];
	i2c_status_t	status;
	signed int 		regVoltage;
	
	USED(numberOfBytes);


	i2c_device_t slave =
	{
		.address = deviceINA219State.i2cAddress,
		.baudRate_kbps = gWarpI2cBaudRateKbps
	};


	cmdBuf[0] = deviceRegister;
	
	
	status = I2C_DRV_MasterReceiveDataBlocking(
							0 /* I2C peripheral instance */,
							&slave,
							cmdBuf,
							1,
							(uint8_t *)deviceINA219State.i2cBuffer,
							numberOfBytes,
							gWarpI2cTimeoutMilliseconds);

	if ((status != kStatus_I2C_Success))
	{
		return kWarpStatusDeviceCommunicationFailed;
	}

	regVoltage = (deviceINA219State.i2cBuffer[0] << 8) | (deviceINA219State.i2cBuffer[1] & 0xFF);
	
	return regVoltage;
}




WarpStatus
writeCalRegisterINA219(uint16_t payload, uint16_t menuI2cPullupValue)
{
	uint8_t		payloadByte[2], commandByte[1];
	i2c_status_t	status;

	uint8_t		deviceRegister = 0x04;
	
	i2c_device_t slave =
	{
		.address = deviceINA219State.i2cAddress,
		.baudRate_kbps = gWarpI2cBaudRateKbps
	};

	commandByte[0] = deviceRegister;
	//payloadByte[0] = payload;
	//payload is 16bits long, need to split into 2 8bit values
	payloadByte[1] = (payload >> 8) & 0xFF; //Gets only the upper 8 bits
	payloadByte[0] = payload & 0xFF; 		//Gets only the lower 8 bits
	
	status = I2C_DRV_MasterSendDataBlocking(
							0 /* I2C instance */,
							&slave,
							commandByte,
							1,
							payloadByte,
							2,
							gWarpI2cTimeoutMilliseconds);
	if (status != kStatus_I2C_Success)
	{
		return kWarpStatusDeviceCommunicationFailed;
	}

	return kWarpStatusOK;
}
