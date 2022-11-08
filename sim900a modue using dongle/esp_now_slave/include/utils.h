#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "serial_debug.h"


void gpio_init() {
	digitalWrite(EPD_RST,   LOW);
    pinMode(EPD_RST,    INPUT);
    digitalWrite(EPD_VCC_EN, LOW);
    pinMode(EPD_VCC_EN, 	OUTPUT);
    
    digitalWrite(LED_EN, 	HIGH);
	digitalWrite(SOFT_PWR_OFF, HIGH);

	pinMode(ON_BTN_SIG, 	INPUT);
	pinMode(BC_CHG_SIG, 	INPUT);
	pinMode(USB_DETECT_SIG, 	INPUT);

	pinMode(LED_EN, 		OUTPUT);
    digitalWrite(LED_EN, 	HIGH);
	pinMode(SOFT_PWR_OFF, 	OUTPUT);

	pinMode(RTC_INT, 		INPUT);
	pinMode(BM_ALARM, 		INPUT);
	pinMode(TEMP_ALERT_1, 	INPUT);
	pinMode(TEMP_ALERT_2, 	INPUT);

    pinMode(SPI_CS, 		OUTPUT);
	digitalWrite(SPI_CS, 	LOW);

	delay(20);
}

// Range : low/medium/high
void cpu_freq_set(String range) 
{
    delay(100);
    if(range == "low") {
        rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
    } else if (range == "medium") {
        rtc_clk_cpu_freq_set(RTC_CPU_FREQ_160M);
    } else {
        rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
    }
    delay(100);
}

void power_off() {
    digitalWrite(LED_EN, HIGH);
    I2Ctwo.begin(I2C_SDA_2, I2C_SCL_2, 100000);
    //led_blinking_poweroff();
    log_message(DEBUG_MSG, "Powering off");
    delay(200);
    pinMode(SOFT_PWR_OFF, OUTPUT);
    digitalWrite(SOFT_PWR_OFF, LOW);
    delay(100);
}

void power_off_check() {
    
    if(interruptCounter) {
        interruptCounter = 0;
        log_message(DEBUG_MSG, "Power Button press detected.!");
        if(!digitalRead(USB_DETECT_SIG)) {
            log_message(DEBUG_MSG, "Power off by user..!");

            save_battery_log(battery_voltage, battery_capacity, "PBR");  // PBR: Power butten pressed while running

            battery_status.remove(0);
            battery_status += "OFF";
            // save_battery_log(battery_voltage, battery_capacity);
            
            timerWrite(timer_w, 0); //reset timer (feed watchdog)

            delay(100);
            power_off();
        } else{
            write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "0");
            log_message(DEBUG_MSG, "Developer mode turned off");
            log_message(DEBUG_MSG, "Device is going to restart");
            delay(100);
            ESP.restart();
        }  
    } 
}

#endif /* UTILS_H */
