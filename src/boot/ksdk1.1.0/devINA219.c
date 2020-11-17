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

#include "devINA219.h"

extern volatile WarpI2CDeviceState deviceINA219State;
extern volatile uint32_t		gWarpI2cBaudRateKbps;
extern volatile uint32_t		gWarpI2cTimeoutMilliseconds;
extern volatile uint32_t		gWarpSupplySettlingDelayMilliseconds;

void
initINA219(const uint8_t i2cAddress, WarpI2CDeviceState volatile *  deviceStatePointer)
{

	//Npt needed, the enableI2Cpins function does this
	//PORT_HAL_SetMuxMode(PORTB_BASE, 3, kPortMuxAlt2);
	//PORT_HAL_SetMuxMode(PORTB_BASE, 4, kPortMuxAlt2);
	
	
	deviceStatePointer->i2cAddress	= i2cAddress;
	deviceStatePointer->signalType	= kWarpTypeMaskShuntVoltage;
	
	return;
}



int
readSensorRegisterINA219(uint8_t deviceRegister, int numberOfBytes)
{
	uint8_t			cmdBuf[1];
	i2c_status_t	status;
	int 			regVoltage;
	
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

	regVoltage = deviceINA219State.i2cBuffer[1] | (deviceINA219State.i2cBuffer[0] << 8);
	
	return regVoltage;
}