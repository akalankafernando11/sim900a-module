#ifndef PATTERN_H
#define PATTERN_H

#include "epd/epd_files.h"
#include "epd/epson_s1d135xx.h"
#include "epd/pmic_tps65185.h"
#include "epd/probe.h"
#include "epd/epdpsu.h"

bool app_pattern(String image_name)
{
    log_message(DEBUG_EPD, "now: app_pattern");

    // unsigned int timer1 = timer_start();
    // log_message(DEBUG_TIMER, "Clear Display - Fill : " + (String)timer_stop(timer1));

    // unsigned int timer2 = timer_start();

    // log_message(DEBUG_TIMER, "Clear Display - Init : " + (String)timer_stop(timer2));

    unsigned int timer3 = timer_start();

    if (!s1d135xx_pattern_check(S1D13541_LD_IMG_8BPP, image_name))
    {
        log_message(DEBUG_ERROR, "failed to s1d135xx_pattern_check");
        update_status_code(8703);
        return false;
    }

    log_message(DEBUG_TIMER, "Display Image - Pattern check : " + (String)timer_stop(timer3));

    if (!s1d13541_update_temp(PL_EPDC_TEMP_INTERNAL))
    {
        log_message(DEBUG_ERROR, "Failed to update temperature");
        update_status_code(8704);
        return false;
    }

    if (!epd_psu_on())
    {
        log_message(DEBUG_ERROR, "failed to epd_psu_on");
        update_status_code(7101);
        return false;
    }

    unsigned int timer4 = timer_start();

    if (!epson_epdc_update())
    {
        log_message(DEBUG_ERROR, "fail to epson_epdc_update");
        update_status_code(8705);
        return false;
    }
    log_message(DEBUG_TIMER, "Display Image - EPDC Update : " + (String)timer_stop(timer4));

    if (!epson_wait_update_end())
    {
        log_message(DEBUG_ERROR, "failed to epson_wait_update_end");
        update_status_code(8706);
        return false;
    }

    delay(1000);

    log_message(DEBUG_TIMER, "Delay : 1000");

    // Power OFF
    return epd_psu_off();
}

void HBZ9_3V3_enable(){
	digitalWrite(EPD_VCC_EN, HIGH);
	delay(10);
    digitalWrite(EPD_RST, 	HIGH);
    delay(10);
}

void HBZ9_3V3_desable(){
	digitalWrite(EPD_VCC_EN, LOW);
	delay(10);
    digitalWrite(EPD_RST, 	LOW);
    delay(10);
}


void epd_gpio_init(){
	// 
	
    digitalWrite(EPD_BB_SLEEP, 	HIGH);
    digitalWrite(EPD_HDC, 	HIGH);
    digitalWrite(EPD_RST, 	HIGH);
    digitalWrite(SPI_CS, 	HIGH);

    pinMode(EPD_BB_SLEEP, OUTPUT);
    pinMode(SPI_CS, OUTPUT);
    pinMode(EPD_HDC, OUTPUT);
    pinMode(EPD_RST, OUTPUT);

    digitalWrite(EPD_HVEN, 	LOW);
    digitalWrite(EPD_VCOM, 	LOW);

    pinMode(EPD_HVEN, OUTPUT);
    pinMode(EPD_VCOM, OUTPUT);
    pinMode(EPD_POK, INPUT_PULLUP);

    pinMode(EPD_VCC_EN, OUTPUT);
}

bool display_image(String image_name)
{
    //Serial.println("11");
    //Serial.println(image_name);
    log_message(DEBUG_MSG, "Image changing: " + image_name);

    time_t timer_initial = timer_start();

    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if ((image_name != "pattern") && (image_name != "white") && (image_name != "black"))
    {
        if (!check_availability(SPIFFS, "/", image_name))
        {
            log_message(DEBUG_ERROR, "image not available");
            update_status_code(8001);
            return false;
        }
        if (!validate_file_size(SPIFFS, "/", image_name, IMAGE_FILE_SIZE, 0))
        {
            log_message(DEBUG_ERROR, "image size validation failed");
            update_status_code(8805);
            return false;
        }
    }

    I2Cone.endTransmission(); 
    SPI.endTransaction();
    SPI.end();

    cpu_freq_set("high");

    digitalWrite(EPD_VCC_EN, HIGH);
    epd_gpio_init();

    delay(100);

    // log_message(DEBUG_MSG, "I2C initializing");
    I2Cone.begin(I2C_SDA_1, I2C_SCL_1, 400000);   

    // log_message(DEBUG_MSG, "SPI initializing");
    SPI.begin(SPI_CLK, SPI_MISO, SPI_MOSI, SPI_CS);
    SPI.beginTransaction(SPISettings(9600000, MSBFIRST, SPI_MODE0));

    if (!epdc_pmic_init())
    {
        I2Cone.endTransmission(); 
        SPI.endTransaction();
        SPI.end();

        digitalWrite(EPD_VCC_EN, LOW);
        return false;
    }

    if (!app_pattern(image_name))
    {
        I2Cone.endTransmission(); 
        SPI.endTransaction();
        SPI.end();

        digitalWrite(EPD_VCC_EN, LOW);
        return false;
    }

    I2Cone.endTransmission(); 
    SPI.endTransaction();
    SPI.end();

    digitalWrite(EPD_VCC_EN, LOW);

    image_change_duration = timer_stop(timer_initial);

    return true;
}

#endif /* PATTERN_H */
