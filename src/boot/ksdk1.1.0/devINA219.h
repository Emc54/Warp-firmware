#ifndef WARP_BUILD_ENABLE_DEVINA219
#define WARP_BUILD_ENABLE_DEVINA219
#endif

void		initINA219(const uint8_t i2cAddress, WarpI2CDeviceState volatile *  deviceStatePointer);
int			readLATSensorRegisterINA219(uint8_t deviceRegister, int numberOfBytes);
WarpStatus  writeCalRegisterINA219(uint16_t payload, uint16_t menuI2cPullupValue);


WarpStatus	writeSensorRegisterINA219(uint8_t deviceRegister, uint16_t payload, uint16_t menuI2cPullupValue);
WarpStatus	configureSensorINA219(uint16_t payloadCTRL_CON, uint16_t payloadCTRL_CAL, uint16_t menuI2cPullupValue);
WarpStatus	readSensorRegisterINA219(uint8_t deviceRegister, int numberOfBytes);
void		printSensorDataINA219(bool hexModeFlag);
