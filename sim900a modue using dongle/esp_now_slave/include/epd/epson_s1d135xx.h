#ifndef EPSON_S1D135XX_H
#define EPSON_S1D135XX_H

#define S1D13541_PROD_CODE 0x0053

#define S1D135XX_CMD_INIT_SET 0x00
#define S1D135XX_CMD_RUN 0x02
#define S1D135XX_CMD_STBY 0x04
#define S1D135XX_CMD_SLEEP 0x05
#define S1D135XX_CMD_INIT_STBY 0x06
#define S1D135XX_CMD_INIT_ROT_MODE 0x0B
#define S1D135XX_CMD_READ_REG 0x10
#define S1D135XX_CMD_WRITE_REG 0x11
#define S1D135XX_CMD_BST_RD_SDR 0x1C
#define S1D135XX_CMD_BST_WR_SDR 0x1D
#define S1D135XX_CMD_BST_END_SDR 0x1E
#define S1D135XX_CMD_LD_IMG 0x20
#define S1D135XX_CMD_LD_IMG_AREA 0x22
#define S1D135XX_CMD_LD_IMG_END 0x23
#define S1D135XX_CMD_WAIT_DSPE_TRG 0x28
#define S1D135XX_CMD_WAIT_DSPE_FREND 0x29
#define S1D135XX_CMD_UPD_INIT 0x32
#define S1D135XX_CMD_UPDATE_FULL 0x33
#define S1D135XX_CMD_UPDATE_FULL_AREA 0x34
#define S1D135XX_CMD_UPDATE_PARTIAL 0x35
#define S1D135XX_CMD_UPDATE_PARTIAL_AREA 0x36
#define S1D135XX_CMD_EPD_GDRV_CLR 0x37

#define S1D135XX_REG_REV_CODE 0x0002
#define S1D135XX_REG_SOFTWARE_RESET 0x0008
#define S1D135XX_REG_SYSTEM_STATUS 0x000A
#define S1D135XX_REG_I2C_CLOCK 0x001A
#define S1D135XX_REG_PERIPH_CONFIG 0x0020
#define S1D135XX_REG_HOST_MEM_PORT 0x0154
#define S1D135XX_REG_I2C_TEMP_SENSOR_VALUE 0x0216
#define S1D135XX_REG_I2C_STATUS 0x0218
#define S1D135XX_REG_PWR_CTRL 0x0230
#define S1D135XX_REG_SEQ_AUTOBOOT_CMD 0x02A8
#define S1D135XX_REG_DISPLAY_BUSY 0x0338
#define S1D135XX_REG_INT_RAW_STAT 0x033A

#define S1D135XX_WF_MODE(_wf) (((_wf) << 8) & 0x0F00)
#define S1D135XX_XMASK 0x0FFF
#define S1D135XX_YMASK 0x0FFF
#define S1D135XX_INIT_CODE_CHECKSUM_OK 0x01 //(1 << 7)
#define S1D135XX_PWR_CTRL_UP 0x8001
#define S1D135XX_PWR_CTRL_DOWN 0x8002
#define S1D135XX_PWR_CTRL_BUSY 0x0080
#define S1D135XX_PWR_CTRL_CHECK_ON 0x2200

#define S1D13541_PROD_CODE 0x0053
#define S1D13541_STATUS_HRDY (1 << 5)
#define S1D13541_INTERNAL_CLOCK_ENABLE (1 << 7)
#define S1D13541_I2C_CLOCK_DIV 7   /* 100 kHz */
#define S1D13541_PROT_KEY_1 0x5678 /* ToDo: add to s1d135xx_data */
#define S1D13541_PROT_KEY_2 0x1234
#define S1D13541_TEMP_SENSOR_CONTROL (1 << 14)
#define S1D13541_TEMP_SENSOR_EXTERNAL (1 << 6)
#define S1D13541_AUTO_TEMP_JUDGE_EN (1 << 2)
#define S1D13541_GENERIC_TEMP_EN (1 << 15)
#define S1D13541_GENERIC_TEMP_JUDGE_EN (1 << 14)
#define S1D13541_GENERIC_TEMP_MASK 0x01FF
#define S1D13541_INT_RAW_WF_UPDATE (1 << 14)
#define S1D13541_INT_RAW_OUT_OF_RANGE (1 << 10)
#define S1D13541_LD_IMG_1BPP (0 << 4)
#define S1D13541_LD_IMG_2BPP (1 << 4)
#define S1D13541_LD_IMG_4BPP (2 << 4)
#define S1D13541_LD_IMG_8BPP (3 << 4)

#define S1D13541_REG_CLOCK_CONFIG 0x0010
#define S1D13541_REG_PROT_KEY_1 0x042C
#define S1D13541_REG_PROT_KEY_2 0x042E
#define S1D13541_REG_FRAME_DATA_LENGTH 0x0400
#define S1D13541_REG_LINE_DATA_LENGTH 0x0406
#define S1D13541_REG_WF_DECODER_BYPASS 0x0420
#define S1D13541_REG_TEMP_SENSOR_VALUE 0x0576
#define S1D13541_REG_GENERIC_TEMP_CONFIG 0x057E

#define S1D13541_WF_CHECKSUM_ERROR 0x001F

#define VERBOSE_TEMPERATURE 0
#define S1D135XX_TEMP_MASK 0x00FF

#define S1D13541_CMD_RD_TEMP 0x12

#define PL_EPDC_RUN 1
#define PL_EPDC_STANDBY 2
#define PL_EPDC_SLEEP 3
#define PL_EPDC_OFF 4

enum pl_epdc_temp_mode
{
    PL_EPDC_TEMP_MANUAL,
    PL_EPDC_TEMP_EXTERNAL,
    PL_EPDC_TEMP_INTERNAL,
};

uint16_t epdc_xres, epdc_yres;

void spi_send_cmd(uint16_t cmd)
{
    digitalWrite(EPD_HDC, LOW);
    SPI.write(cmd >> 8);
    SPI.write(cmd);
    digitalWrite(EPD_HDC, HIGH);
}

void spi_send_param(uint16_t param)
{
    SPI.write(param >> 8);
    SPI.write(param);
}

void spi_send_cmd_cs(uint16_t cmd)
{
    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(cmd);
    digitalWrite(SPI_CS, HIGH);
}

void spi_send_array_uint8_t(const uint8_t *data, uint16_t n)
{
    for (int i = 0; i < n;)
    {
        uint8_t val_2 = data[i];
        i++;
        uint8_t val_1 = data[i];
        i++;
        SPI.write(val_1);
        SPI.write(val_2);
    }
}

void spi_send_array(const uint16_t *data, uint16_t n)
{
    for (int i = 0; i < n; i++)
    {
        uint8_t data_1 = data[i];
        uint8_t data_2 = data[i] >> 8;
        SPI.write(data_1);
        SPI.write(data_2);
    }
}

uint8_t spi_read_reg(uint16_t reg)
{
    uint8_t read_data = 0;

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_READ_REG);
    spi_send_param(reg);

    for (int i = 0; i < 4; i++)
    {
        read_data += SPI.transfer(0x00);
    }

    digitalWrite(SPI_CS, HIGH);

    return read_data / 2;
}

void spi_write_reg(uint16_t reg, uint16_t val)
{
    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_WRITE_REG);
    spi_send_param(reg);
    spi_send_param(val);
    digitalWrite(SPI_CS, HIGH);
}

int get_hrdy()
{
    uint8_t status;
    status = spi_read_reg(S1D135XX_REG_SYSTEM_STATUS);

    return ((status & S1D13541_STATUS_HRDY) == S1D13541_STATUS_HRDY);
}

bool s1d135xx_wait_idle()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_wait_idle");

    unsigned long timeout = 10;
    delay(100);

    while (!get_hrdy() && --timeout)
    {
    }

    if (!timeout)
    {
        log_message(DEBUG_ERROR, "HRDY timeout");
        //update_status_code(8609);
        return false;
    }

    return true;
}

void epson_gpio_init()
{
    log_message(DEBUG_EPD, "Now: EPSON_GPIO_init");

    digitalWrite(EPD_BB_SLEEP, HIGH);
    digitalWrite(EPD_HDC, HIGH);
    digitalWrite(EPD_RST, HIGH);
    digitalWrite(SPI_CS, HIGH);

    pinMode(EPD_BB_SLEEP, OUTPUT);
    pinMode(EPD_HDC, OUTPUT);
    pinMode(EPD_RST, OUTPUT);
    pinMode(SPI_CS, OUTPUT);
}

void s1d135xx_hard_reset()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_hard_reset");

    digitalWrite(EPD_RST, LOW);
    delay(4);
    digitalWrite(EPD_RST, HIGH);
    delay(10);
}

bool s1d135xx_soft_reset()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_soft_reset");

    spi_write_reg(S1D135XX_REG_SOFTWARE_RESET, 0xFF);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    return true;
}

bool s1d135xx_check_prod_code(uint16_t ref_code)
{
    log_message(DEBUG_EPD, "Now: s1d135xx_check_prod_code");

    uint8_t prod_code = 0;

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_READ_REG);
    spi_send_param(S1D135XX_REG_REV_CODE);
    for (int i = 0; i < 4; i++)
    {
        prod_code += SPI.transfer(0x00);
    }
    digitalWrite(SPI_CS, HIGH);
    prod_code = prod_code / 2;

    // log_hex_val_epd(DEBUG_EPD, "Product read: ", prod_code);

    if (prod_code == ref_code)
    {
        return true;
    }

    return false;
}

bool s1d13541_init_clocks()
{
    log_message(DEBUG_EPD, "Now: s1d13541_init_clocks");

    spi_write_reg(S1D135XX_REG_I2C_CLOCK, S1D13541_I2C_CLOCK_DIV);
    spi_write_reg(S1D13541_REG_CLOCK_CONFIG, S1D13541_INTERNAL_CLOCK_ENABLE);
    return true;
}

bool s1d13541_epson_epdc_early_init()
{
    log_message(DEBUG_EPD, "Now: epson_epdc_early_init_s1d13541");

    s1d135xx_hard_reset();
    delay(100);

    if (!s1d135xx_soft_reset())
    {
        log_message(DEBUG_ERROR, "soft_reset fail");
        update_status_code(8601);
        return false;
    }

    if (!s1d135xx_check_prod_code(S1D13541_PROD_CODE))
    {
        log_message(DEBUG_ERROR, "check product code invalid");
        update_status_code(8602);
        return false;
    }

    if (!s1d13541_init_clocks())
    {
        return false;
    }

    return true;
}

bool s1d135xx_load_init_code()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_load_init_code");

    uint16_t count_ = 0;
    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_INIT_SET);

    while (count_ < 2348)
    {
        uint8_t data_2 = ecodebin[count_];

        count_++;
        uint8_t data_1 = ecodebin[count_];

        count_++;
        SPI.write(data_1);
        SPI.write(data_2);
    }

    digitalWrite(SPI_CS, HIGH);

    log_message(DEBUG_EPD, "Init code transfered");

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    delay(5);
    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_INIT_STBY);
    spi_send_param(0x0500);
    digitalWrite(SPI_CS, HIGH);
    delay(100);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    uint16_t checksum = spi_read_reg(S1D135XX_REG_SEQ_AUTOBOOT_CMD);

    // log_hex_val_epd(DEBUG_EPD, "checksum:", checksum);

    if (!(checksum & (uint16_t)S1D135XX_INIT_CODE_CHECKSUM_OK))
    {
        log_message(DEBUG_ERROR, "Init code checksum error"); // 2222

        return false;
    }

    log_message(DEBUG_EPD, "Init code checksum success");

    return true;
}

bool s1d135xx_set_power_state(int state)
{
    log_message(DEBUG_EPD, "Now: s1d135xx_set_power_state");

    digitalWrite(SPI_CS, HIGH);
    digitalWrite(EPD_HDC, HIGH);
    digitalWrite(EPD_BB_SLEEP, HIGH);
    bool stat = false;
    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    switch (state)
    {
    case 1: //PL_EPDC_RUN
        spi_send_cmd_cs(S1D135XX_CMD_RUN);
        stat = ~s1d135xx_wait_idle();
        break;
    case 2: //PL_EPDC_STANDBY:
        spi_send_cmd_cs(S1D135XX_CMD_STBY);
        stat = ~s1d135xx_wait_idle();
        break;
    case 3: //PL_EPDC_SLEEP
        spi_send_cmd_cs(S1D135XX_CMD_STBY);
        stat = ~s1d135xx_wait_idle();
        digitalWrite(EPD_BB_SLEEP, LOW);
        break;
    case 4: // PL_EPDC_OFF
        spi_send_cmd_cs(S1D135XX_CMD_SLEEP);
        stat = ~s1d135xx_wait_idle();
        digitalWrite(EPD_BB_SLEEP, LOW);
        // Z6_VCC_EN_L;
        break;
    }

    return true;
}

bool epson_epdc_set_power(int state)
{
    log_message(DEBUG_EPD, "Now: epson_epdc_set_power");

    if (!s1d135xx_set_power_state(state))
    {
        return false;
    }

    return true;
}

bool s1d135xx_init_gate_drv()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_init_gate_drv");

    spi_send_cmd_cs(S1D135XX_CMD_EPD_GDRV_CLR);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    return true;
}

bool s1d135xx_wait_dspe_trig()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_wait_dspe_trig");

    spi_send_cmd_cs(S1D135XX_CMD_WAIT_DSPE_TRG);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    return true;
}

bool s1d13541_set_temp_mode(int mode)
{
    log_message(DEBUG_EPD, "Now: s1d13541_set_temp_mode");

    uint16_t reg;
    reg = spi_read_reg(S1D135XX_REG_PERIPH_CONFIG);
    reg &= S1D13541_TEMP_SENSOR_CONTROL;

    switch (mode)
    {
    case PL_EPDC_TEMP_MANUAL:
        break;
    case PL_EPDC_TEMP_EXTERNAL:
        reg &= ~S1D13541_TEMP_SENSOR_EXTERNAL;
        break;
    case PL_EPDC_TEMP_INTERNAL:
        reg |= S1D13541_TEMP_SENSOR_EXTERNAL;
        break;
    default:
        break;
    }

    spi_write_reg(S1D135XX_REG_PERIPH_CONFIG, reg);

    reg = spi_read_reg(S1D13541_REG_WF_DECODER_BYPASS);
    reg |= S1D13541_AUTO_TEMP_JUDGE_EN;
    spi_write_reg(reg, S1D13541_REG_WF_DECODER_BYPASS);

    return true;
}

bool tmp = 1;
uint8_t in_byte_1, in_byte_2;

void spi_send_bytes(uint8_t in_byte)
{
    if (tmp)
    {
        in_byte_1 = in_byte;
        tmp = 0;
    }
    else
    {
        in_byte_2 = in_byte;
        uint16_t send_bytes = (in_byte_2 * 256) + in_byte_1;
        spi_send_param(send_bytes);
        tmp = 1;
    }
}

bool wflib_wr()
{
    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_WRITE_REG);
    spi_send_param(S1D135XX_REG_HOST_MEM_PORT);

    uint16_t tmp2 = 0;
    for (uint32_t tmp1 = 0; tmp1 < 22689;)
    {
        uint8_t data_byte_1 = waveform[tmp1];
        uint8_t data_buye_2 = waveform[tmp1 + 1];
        if (data_buye_2 == 0xFF)
        {
            tmp1++;
            uint8_t similar_data_count = (waveform[tmp1 + 1]);
            tmp1++;
            tmp1++;
            while (similar_data_count--)
            {
                //Serial.println(F(data_byte_1, HEX);
                spi_send_bytes(data_byte_1);
                tmp2++;
            }
        }
        else
        {
            //Serial.println(F(data_byte_1, HEX);
            spi_send_bytes(data_byte_1);
            tmp2++;
            tmp1++;
        }
    }

    digitalWrite(SPI_CS, HIGH);
    return true;
}

bool s1d13541_load_wflib()
{
    uint16_t busy;

    log_message(DEBUG_EPD, "Now: s1d13541_load_wflib");

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    uint16_t vcom[4] = {0x0000, 0x0800, 0xE658, 0x0000};

    log_message(DEBUG_EPD, "Now: Send vcom");

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_BST_WR_SDR);
    spi_send_array(vcom, 4); // Send vcom data
    digitalWrite(SPI_CS, HIGH);

    if (!wflib_wr())
    {
        return false;
    }

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    log_message(DEBUG_EPD, "Now: send cs S1D135XX_CMD_BST_END_SDR");

    spi_send_cmd_cs(S1D135XX_CMD_BST_END_SDR);

    // Check waveform file loaded properly to display
    busy = spi_read_reg(S1D135XX_REG_DISPLAY_BUSY);
    if (!(busy & S1D13541_WF_CHECKSUM_ERROR))
    {
        log_message(DEBUG_ERROR, "Waveform file checksum error : " + (String)busy);
        update_status_code(8612);
        return false;
    }

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    return true;
}

bool s1d13541_epson_epdc_init()
{
    log_message(DEBUG_EPD, "Now epson_epdc_init_s1d13541");
    unsigned int timer = timer_start();

    if (!s1d13541_epson_epdc_early_init())
    {

        log_message(DEBUG_ERROR, "Failed to epdc_early init");
        update_status_code(8600);
        return false;
    }
    log_message(DEBUG_TIMER, "EPSON EPDC Early Init : " + (String)timer_stop(timer));

    if (!s1d135xx_load_init_code())
    {
        update_status_code(8610);
        log_message(DEBUG_ERROR, "Failed to load init code");
        return false;
    }
    log_message(DEBUG_TIMER, "Load Init Code : " + (String)timer_stop(timer));

    log_message(DEBUG_EPD, "Now: Sending S1D13541_REG_PROT_KEY_1");

    spi_write_reg(S1D13541_REG_PROT_KEY_1, S1D13541_PROT_KEY_1);

    log_message(DEBUG_EPD, "Now: Sending S1D13541_REG_PROT_KEY_2");

    spi_write_reg(S1D13541_REG_PROT_KEY_2, S1D13541_PROT_KEY_2);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    if (!epson_epdc_set_power(PL_EPDC_RUN))
    {
        return false;
    }

    if (!s1d135xx_init_gate_drv())
    {
        return false;
    }

    if (!s1d135xx_wait_dspe_trig())
    {
        return false;
    }

    log_message(DEBUG_EPD, "Now: Read reg S1D13541_REG_LINE_DATA_LENGTH");

    epdc_xres = spi_read_reg(S1D13541_REG_LINE_DATA_LENGTH);
    epdc_yres = spi_read_reg(S1D13541_REG_FRAME_DATA_LENGTH);

    // log_hex_val_epd(DEBUG_EPD, "epdc_yres: ", epdc_xres);
    // log_hex_val_epd(DEBUG_EPD, "epdc_yres: ", epdc_yres);

    if (!s1d13541_set_temp_mode(PL_EPDC_TEMP_INTERNAL))
    {
        return false;
    }

    timer = timer_start();
    if (!s1d13541_load_wflib())
    {
        return false;
    }
    log_message(DEBUG_TIMER, "Wafeform file load : " + (String)timer_stop(timer));

    return true;
}

/**
* Set white to each pixels in the display to clean the existing image
**/
bool clear_display()
{
    uint32_t lines = 240;
    uint32_t pixels_per_line = 400;
    uint32_t display_pixels = lines * pixels_per_line;

    for (uint32_t read_count = 0; read_count < display_pixels; read_count++)
    {
        SPI.write(0xFF);
    }

    return true;
}

bool s1d13541_fill()
{
    log_message(DEBUG_EPD, "NOW: ss1d13541_fill");

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_LD_IMG);
    spi_send_param(0x0030);
    digitalWrite(SPI_CS, HIGH);

    log_message(DEBUG_EPD, "Now: do_fill");

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_WRITE_REG);
    spi_send_param(S1D135XX_REG_HOST_MEM_PORT);

    clear_display();

    digitalWrite(SPI_CS, HIGH);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    spi_send_cmd_cs(S1D135XX_CMD_LD_IMG_END);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    return true;
}

bool clear_init()
{
    log_message(DEBUG_EPD, "Now: clear_init");

    spi_send_cmd_cs(S1D135XX_CMD_UPD_INIT);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    if (!s1d135xx_wait_dspe_trig())
    {
        return false;
    }

    return true;
}

bool epson_epdc_update()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_update");

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_UPDATE_FULL);
    spi_send_param(0x0100);
    digitalWrite(SPI_CS, HIGH);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    if (!s1d135xx_wait_dspe_trig())
    {
        return false;
    }

    return true;
}

bool epson_wait_update_end()
{
    log_message(DEBUG_EPD, "Now: s1d135xx_wait_update_end");

    spi_send_cmd_cs(S1D135XX_CMD_WAIT_DSPE_FREND);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    return true;
}

bool spi_send_image_pattern()
{
    // uint8_t lines = 240;
    // uint8_t pixels = 400;
    uint32_t count_ = 95997;

    spi_send_param(0x0000);
    spi_send_param(0xFFFF);
    count_--;

    while (count_--)
    {
        spi_send_param(count_ % 65535);
    }

    return true;
}

bool spi_send_image_white()
{
    // uint8_t lines = 240;
    // uint8_t pixels = 400;
    uint32_t count_ = 95997;

    while (count_--)
    {
        spi_send_param(0xFFFF);
    }

    return true;
}

bool spi_send_image_black()
{
    // uint8_t lines = 240;
    // uint8_t pixels = 400;
    uint32_t count_ = 95997;

    while (count_--)
    {
        spi_send_param(0x0000);
    }

    return true;
}

bool spi_image_data_send(String image_name)
{
    uint32_t read_count;
    uint8_t image_byte = 0;

    if (!read_file_to_array(SPIFFS, "/", image_name, 24000))
    {
        return false;
    }

    for (read_count = 25; read_count < 24025; read_count++)
    {
        uint8_t read_val = spiffs_read_buffer[read_count];

        if (read_val > 59)
        {
            read_val = read_val - 55;
        }
        else
        {
            read_val = read_val - 48;
        }

        if (!(read_count % 2))
        {
            image_byte = read_val << 4;
        }
        else
        {
            read_val = read_val;
            image_byte += read_val;
            uint8_t bit_count = 0;
            bool pixel_val1 = 0, pixel_val2 = 0;

            while (bit_count < 8)
            {
                pixel_val2 = image_byte & (0x80 >> bit_count);
                bit_count++;
                pixel_val1 = image_byte & (0x80 >> bit_count);
                bit_count++;

                if (pixel_val1)
                {
                    SPI.write(0xFF);
                }
                else
                {
                    SPI.write(0x00);
                }

                if (pixel_val2)
                {
                    SPI.write(0xFF);
                }
                else
                {
                    SPI.write(0x00);
                }

                pixel_val1 = pixel_val2 = 0;
            }

            image_byte = 0;
        }
    }

    read_count = 0;
    return true;
}

bool s1d135xx_pattern_check(uint16_t mode, String image_name)
{
    log_message(DEBUG_EPD, "Now: s1d135xx_pattern_check");

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_LD_IMG);
    spi_send_param(mode);
    digitalWrite(SPI_CS, HIGH);

    digitalWrite(SPI_CS, LOW);
    spi_send_cmd(S1D135XX_CMD_WRITE_REG);
    spi_send_param(S1D135XX_REG_HOST_MEM_PORT);

    if (image_name == "pattern")
    {
        spi_send_image_pattern();
    }
    else if (image_name == "white")
    {
        spi_send_image_white();
    }
    else if (image_name == "black")
    {
        spi_send_image_black();
    }
    else
    {
        spi_image_data_send(image_name);
    }

    digitalWrite(SPI_CS, HIGH);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    spi_send_cmd_cs(S1D135XX_CMD_LD_IMG_END);

    return true;
}

uint8_t update_temp(uint16_t reg)
{
    log_message(DEBUG_EPD, "Now: update_temp");

    uint16_t regval = 0;
    regval = spi_read_reg(S1D135XX_REG_INT_RAW_STAT);
    spi_write_reg(S1D135XX_REG_INT_RAW_STAT, (S1D13541_INT_RAW_WF_UPDATE | S1D13541_INT_RAW_OUT_OF_RANGE));
    regval = spi_read_reg(reg) & S1D135XX_TEMP_MASK;
    return regval;
}

int update_temp_auto(uint16_t temp_reg)
{
    log_message(DEBUG_EPD, "Now: update_temp_auto");

    if (!s1d135xx_set_power_state(PL_EPDC_STANDBY))
    {
        return false;
    }

    spi_send_cmd_cs(S1D13541_CMD_RD_TEMP);

    if (!s1d135xx_wait_idle())
    {
        return false;
    }

    if (!s1d135xx_set_power_state(PL_EPDC_RUN))
    {
        return false;
    }

    return update_temp(temp_reg);
    ;
}

bool s1d13541_update_temp(enum pl_epdc_temp_mode temp_mode)
{
    log_message(DEBUG_EPD, "Now: s1d13541_update_temp");

    int stat = -1;
    switch (temp_mode)
    {
    case PL_EPDC_TEMP_MANUAL:
    {
        stat = update_temp_auto(S1D135XX_REG_I2C_TEMP_SENSOR_VALUE);
        break;
    }
    case PL_EPDC_TEMP_EXTERNAL:
        break;
    case PL_EPDC_TEMP_INTERNAL:
    {
        stat = update_temp_auto(S1D13541_REG_TEMP_SENSOR_VALUE);
        break;
    }
    }

    if (!stat)
    {
        log_message(DEBUG_EPD, "Unknown temp mode");
        return false;
    }
    log_message(DEBUG_EPD, "Temperature mode updated");
    log_message(DEBUG_EPD, "Temperature = " + (String)stat + "°C");

    return true;
}

bool init_check()
{
    uint16_t checksum = spi_read_reg(S1D135XX_REG_SEQ_AUTOBOOT_CMD);
    // log_hex_val_epd(DEBUG_EPD, "checksum:", checksum);

    if (!(checksum & (uint16_t)S1D135XX_INIT_CODE_CHECKSUM_OK))
    {
        log_message(DEBUG_ERROR, "Init code checksum error");
        return false;
    }
    return true;
}

#endif /* EPSON_S1D135XX_H */
