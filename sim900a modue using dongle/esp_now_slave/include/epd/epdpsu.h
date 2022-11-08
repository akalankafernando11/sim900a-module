#ifndef EPDPSU_H
#define EPDPSU_H

#include "config.h"
#include "serial_debug.h"

bool epd_psu_on()
{
    log_message(DEBUG_EPD,  "now: epd_psu_on");

    digitalWrite(EPD_HVEN, HIGH);
    uint16_t timeOut = 1000;

    while (--timeOut) {
        if (digitalRead(EPD_POK)) {
            break;
        }

        delay(5);
    }

    if (!timeOut) {
        log_message(DEBUG_EPD,  "POK timeout");

        digitalWrite(EPD_HVEN, LOW);
        return false;
    }

    digitalWrite(EPD_VCOM, HIGH);
    delay(200);

    return true;
}

bool epd_psu_off()
{
    digitalWrite(EPD_HVEN, LOW);
    delay(100);
    digitalWrite(EPD_VCOM, LOW);
    delay(100);
    

    return true;
}

#endif /* EPDPSU_H */ 
