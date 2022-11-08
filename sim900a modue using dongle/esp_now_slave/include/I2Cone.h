#ifndef I2C_LIB_ONE_H
#define I2C_LIB_ONE_H

byte read_I2Cone(uint8_t addr, uint8_t reg)
{
    byte I2Cone_read_byte = 0x00;
    I2Cone.beginTransmission(addr);
    I2Cone.write(reg);
    I2Cone.endTransmission();
    I2Cone.requestFrom(addr, 1);
    
    if (I2Cone.available()<=1) {
        I2Cone_read_byte = I2Cone.read();
    }
    
    return I2Cone_read_byte;
}

bool write_I2Cone(uint8_t addr, uint8_t reg, uint8_t val)
{
    I2Cone.beginTransmission(addr);
    I2Cone.write(reg);
    I2Cone.write(val);
    bool rtnVal = I2Cone.endTransmission();
    return rtnVal;
}

uint16_t read_I2Cone_16(uint8_t addr, uint8_t reg) {
  uint16_t result; // returned value
  I2Cone.beginTransmission(addr);
  I2Cone.write(reg);
  I2Cone.endTransmission(false);
  I2Cone.requestFrom(addr, 2); // now get the byte of data...
  result = I2Cone.read() | (I2Cone.read() << 8);  //LSB first

  return result; // and return it as a result of the function readData
}

void write_I2Cone_16(uint8_t addr, uint8_t reg, uint16_t val) {
    I2Cone.beginTransmission(addr);
    I2Cone.write(reg);
    I2Cone.write(val & 0xFF);       //LSB first
    I2Cone.write(val >> 8);
    I2Cone.endTransmission();
}
/*
void I2Cone_scanner(bool inf) {
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

    I2Cone.begin(I2C_SDA_1_PIN, I2C_SCL_1_PIN, 100000);

    Serial.println(String(read_I2Cone(0x08, 0)));

    while (inf) {
        timerWrite(timer_w, 0); //reset timer (feed watchdog)
        byte error, address;
        int nDevices;
       
        Serial.println("I2C One Scanning.");
       
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
            Serial.print("I2C device found at address 0x");
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
          Serial.println("No I2C one devices found\n");
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

#endif /* I2Cone_H */ 


