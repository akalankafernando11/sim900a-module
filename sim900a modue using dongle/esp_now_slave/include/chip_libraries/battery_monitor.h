#ifndef LC709203F_H
#define LC709203F_H

#define BATTERY_MONITOR_ADDRESS   0x0B

uint16_t getVoltage(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x09);
}

uint16_t getCapacity(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x0F);
}

uint16_t getBConst(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x06);
}

uint16_t getCellTemperature(void) 
{
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x16, 0x0001);
     
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x08);
}

uint16_t getDirection(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x0A);
}

uint16_t getAPT(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x0C);
}

uint16_t getRSOC(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x0D);
}

uint16_t getICVersion(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x11);
}

uint16_t getProfCode(void) 
{
    return read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x1A);
}

void setAlarm(uint8_t value) 
{
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x13, value);
}

void setVoltageAlarm(uint16_t value) 
{
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x14, value);
}

void adjust(uint16_t value) 
{
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x0B, value);
}




bool initBattMon() {
    if (read_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x11) == 0x00)
        return false;
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x04, 0xAA55);        //set initial RSOC (necessary?)
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x07, 0xAA55);        //set initial RSOC (necessary?)
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x15, 0x0001);        //set power mode : Operating
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x0B, 0x000B);        //set Adjustment Pack Application Value
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x12, 0x0000);        //choose default battery profile (see datasheet)
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x16, 0x0001);        //Enable thermistor
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x06, 0x0D49);        //Sets B−constant of the thermistor to be measured (0x0D49 = 3401K)
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x0A, 0x0000);        //set auto current detection mode
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x13, 0x0005);        //activate alarm if RSOC lower than 5%
    // delay_ms(1);
    write_I2Ctwo_16(BATTERY_MONITOR_ADDRESS, 0x14, 0x3500);        //activate alarm if voltage under 2,8volts
    // delay_ms(1);
    return true;
}



#endif /* LC709203F_H */
