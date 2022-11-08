#ifndef DEBUG_H
#define DEBUG_H

// debug
bool serial_debug_en          = 1;
bool debug_epd_en             = 0;
bool debug_mem_en             = 0;
bool debug_eeprom_en          = 0;

const bool DEBUG_ERROR_EN           = 1;
const bool DEBUG_ESP_EN            = 1;
const bool DEBUG_MSG_EN             = 1;
const bool DEBUG_TIMER_EN           = 1;
const bool DEBUG_SUMMARY_EN         = 1;


#define DEBUG_EPD       2
#define DEBUG_ERROR     3
#define DEBUG_ESP       4
#define DEBUG_MSG       5
#define DEBUG_TIMER     6
#define DEBUG_EEPROM    7
#define DEBUG_STEP      8
#define DEBUG_MEM       9
#define DEBUG_SUMMARY   10
#define DEBUG_OTA       11

void log_message(uint8_t debug_type, String str="", bool printSameLine=false) {
    if (serial_debug_en) {
        String prefix ((char *)0);
        bool log_en = false;
        bool summary_en = false;
        switch(debug_type){
            case DEBUG_EPD:
                prefix += "DEBUG_EPD";
                log_en = debug_epd_en;
                break;
            case DEBUG_ERROR:
                prefix += "DEBUG_ERROR";
                log_en = DEBUG_ERROR_EN;
                break;
            case DEBUG_ESP:
                prefix += "DEBUG_ESP";
                log_en = DEBUG_ESP_EN;
                break;
            case DEBUG_MSG:
                prefix += "DEBUG_MSG";
                log_en = DEBUG_MSG_EN;
                break;
            case DEBUG_TIMER:
                prefix += "DEBUG_TIMER";
                log_en = DEBUG_TIMER_EN;
                break;
            case DEBUG_EEPROM:
                prefix += "DEBUG_EEPROM";
                log_en = debug_eeprom_en;
                break;
            case DEBUG_MEM:
                prefix += "DEBUG_MEM";
                log_en = debug_mem_en;
                break;  
            case DEBUG_SUMMARY:
                prefix += "DEBUG_SUMMARY";
                summary_en = DEBUG_SUMMARY_EN;
                break;  
            case DEBUG_OTA:
                prefix += "DEBUG_OTA";
                log_en = true;
                break;
        }
        log_en = log_en and serial_debug_en;
        
        if (log_en or summary_en) {
            if(printSameLine) {
                Serial.print(str);
            } else {
                String logMsg ((char *)0);
                logMsg += prefix + "\t:\t" + str;
                Serial.println(logMsg);
                logMsg.remove(0);
            }
        }
        prefix.remove(0);
    }
}

time_t timer_start() {
    unsigned int timer = millis();
    return timer; 
}

time_t timer_reset() {
    unsigned int timer = 0;
    return timer; 
}

time_t timer_stop(unsigned int timer) 
{
    return (millis() - timer);
}

#endif /* DEBUG_H */
