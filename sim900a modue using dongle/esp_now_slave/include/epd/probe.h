#ifndef PROBE_H
#define PROBE_H

byte PMIC_Init_data[] = {HVPMIC_DATA_ENABLE, HVPMIC_DATA_VADJ, HVPMIC_DATA_VCOM1, HVPMIC_DATA_VCOM2, HVPMIC_DATA_INT_EN1, HVPMIC_DATA_INT_EN2,
HVPMIC_DATA_UPSEQ0, HVPMIC_DATA_UPSEQ1, HVPMIC_DATA_DWNSEQ0, HVPMIC_DATA_DWNSEQ1, HVPMIC_DATA_TMST1, HVPMIC_DATA_TMST2};

byte PMIC_Init_reg[] = {HVPMIC_REG_ENABLE, HVPMIC_REG_VADJ, HVPMIC_REG_VCOM1, HVPMIC_REG_VCOM2, HVPMIC_REG_INT_EN1, HVPMIC_REG_INT_EN2,
HVPMIC_REG_UPSEQ0, HVPMIC_REG_UPSEQ1, HVPMIC_REG_DWNSEQ0, HVPMIC_REG_DWNSEQ1, HVPMIC_REG_TMST1, HVPMIC_REG_TMST2};


bool probe_hvpmic()
{
	byte I2Cone_read_byte;
    int i;

    log_message(DEBUG_EPD,  "Now: probe_hvpmic");

    I2Cone_read_byte = read_I2Cone(HVPMIC_Address, HVPMIC_REG_REV_ID);

    // log_hex_val_epd(DEBUG_EPD, "probe_hvpmic read: ", i2c_read_byte);

    if (HVPMIC_VERSION != I2Cone_read_byte) {
        log_message(DEBUG_ERROR,  "Wrong version of HVPMIC!");      
        update_status_code(7600);
        return false;
    }
    log_message(DEBUG_EPD,  "Version ID is matched");

    int temp = 0;
    for (i = 0; i < sizeof(PMIC_Init_data); i++) {
        temp = sizeof(PMIC_Init_data);
        if (write_I2Cone(HVPMIC_Address, PMIC_Init_reg[i], PMIC_Init_data[i]))
        {
            log_message(DEBUG_ERROR,  "Fail send init comands to PMIC"); 
            update_status_code(7601);
            return false;
        }
    }
    uint8_t v1 = 0x9A, v2 = 0;
    if (write_I2Cone(HVPMIC_Address, HVPMIC_REG_VCOM1, v1))
    {
        log_message(DEBUG_ERROR,  "Fail send VCOM1 to PMIC");              
        update_status_code(7602);
        return false;
    }
    if (write_I2Cone(HVPMIC_Address, HVPMIC_REG_VCOM2, v2))
    {
        log_message(DEBUG_ERROR,  "Fail send VCOM2 to PMIC");          
        update_status_code(7603);
        return false;
    }
    return true;
}

bool probe_epdc()
{
    log_message(DEBUG_EPD,  "Now: probe_epdc");

    if (!s1d13541_epson_epdc_init()) {
        return false;
    }
    return true;
}

bool epdc_pmic_init()
{
    log_message(DEBUG_EPD,  "Now: epdc_pmic_init");

    if (!probe_hvpmic()) {
        return false;
    }
    if (!probe_epdc()) {
        return false;
    }
    return true;
}
#endif /* PROBE_H */
