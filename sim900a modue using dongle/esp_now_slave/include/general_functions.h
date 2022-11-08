#ifndef GENERAL_H
#define GENERAL_H

#include "config.h"
#include "serial_debug.h"

void update_status_code (uint16_t status_code_val) 
{
    if ( status_code < status_code_val) {
        status_code = status_code_val;
        log_message(DEBUG_MSG, "status_code changed to: " + (String)status_code);
    }
}

String number_to_string(uint8_t number)  {
    String string ((char *)0);
    string.concat("000");

    string.concat(number);

    string = string.substring(string.length()-3, string.length());

    return string;
}
void delay_ms(uint16_t delay_ms_ms) {
  rtc_cpu_freq_t clock = rtc_clk_cpu_freq_get();
  delay_ms_ms = delay_ms_ms / 3 * clock; 
  delay(delay_ms_ms);
}
#endif /* GENERAL_H */ 
