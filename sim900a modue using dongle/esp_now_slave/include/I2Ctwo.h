#ifndef I2C_LIB_TWO_H
#define I2C_LIB_TWO_H

byte read_I2Ctwo(uint8_t addr, uint8_t reg)
{
    byte I2Ctwo_read_byte = 0x00;
    I2Ctwo.beginTransmission(addr);
    I2Ctwo.write(reg);
    I2Ctwo.endTransmission();
    I2Ctwo.requestFrom(addr, 1);
    
    if (I2Ctwo.available()<=1) {
        I2Ctwo_read_byte = I2Ctwo.read();
    }
    
    return I2Ctwo_read_byte;
}

bool write_I2Ctwo(uint8_t addr, uint8_t reg, uint8_t val)
{
    I2Ctwo.beginTransmission(addr);
    I2Ctwo.write(reg);
    I2Ctwo.write(val);
    bool rtnVal = I2Ctwo.endTransmission();
    return rtnVal;
}

uint16_t read_I2Ctwo_16(uint8_t addr, uint8_t reg) {
  uint16_t result; // returned value
  I2Ctwo.beginTransmission(addr);
  I2Ctwo.write(reg);
  I2Ctwo.endTransmission(false);
  I2Ctwo.requestFrom(addr, 2); // now get the byte of data...
  result = I2Ctwo.read() | (I2Ctwo.read() << 8);  //LSB first

  return result; // and return it as a result of the function readData
}

void write_I2Ctwo_16(uint8_t addr, uint8_t reg, uint16_t val) {
    I2Ctwo.beginTransmission(addr);
    I2Ctwo.write(reg);
    I2Ctwo.write(val & 0xFF);       //LSB first
    I2Ctwo.write(val >> 8);
    I2Ctwo.endTransmission();
}
/*
void I2Ctwo_scanner(bool inf) {
    EPSON_BB_SLEEP_H;
    EPSON_HDC_H;
    EPSON_RESET_H;
    EPSON_CS_H;

    EPSON_BB_SLEEP_PIN;
    EPSON_CS_PIN;
    EPSON_HDC_PIN;
    EPSON_RESET_PIN;
    
    PMIC_EN_L;
    HVSW_CTRL_L;

    PMIC_EN_PIN;
    HVSW_CTRL_PIN;
    PMIC_POK_PIN;

    EPD_VCC_EN;
    delay_ms(50);

    I2Cone.begin(I2C_SDA_2_PIN, I2C_SCL_2_PIN, 100000);

    Serial.println(String(read_I2Ctwo(0x08, 0)));

    while (inf) {
        timerWrite(timer_w, 0); //reset timer (feed watchdog)
        byte error, address;
        int nDevices;
       
        Serial.println("I2C Two Scanning");
       
        nDevices = 0;
        for(address = 1; address < 127; address++ )
        {
          // The i2c_scanner uses the return value of
          // the Write.endTransmisstion to see if
          // a device did acknowledge to the address.
          Wire.beginTransmission(address);
          error = Wire.endTransmission();
       
          if (error == 0)
          {
            Serial.print("I2C Two device found at address 0x");
            if (address<16)
              Serial.print("0");
            Serial.print(address,HEX);
            Serial.println("  !");
       
            nDevices++;
          }
          else if (error==4)
          {
            Serial.print("Unknown error at address 0x");
            if (address<16)
              Serial.print("0");
            Serial.println(address,HEX);
          }    
        }
        if (nDevices == 0)
          Serial.println("No I2C Two one devices found\n");
        else
          Serial.println("done\n");
       
        delay_ms(1000);           // wait 5 seconds for next scan
    }
    EPD_VCC_DIS;
    EPSON_BB_SLEEP_PIN_input;
    EPSON_CS_PIN_input;
    EPSON_HDC_PIN_input;
    EPSON_RESET_PIN_input;
    PMIC_EN_PIN_input;
    HVSW_CTRL_PIN_input;
}
*/

#endif /* I2Ctwo_H */ 
