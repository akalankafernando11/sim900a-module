#ifndef DISPLAY_OPTIONS_H
#define DISPLAY_OPTIONS_H

String completed = "==================== COMPLETED ====================";
String line = "====================";
String enter_number = "Please enter number again to continue\n";

bool optFlash = false;
bool optDisplay = false;
bool optMisc = false;
bool opt_settings = false;
bool opt_wifi = false;
bool dev_options = false;

char serial_read_char(){
    char read_char = '~', tmp;
    while(Serial.available()) {
        timerWrite(timer_w, 0);
        tmp = Serial.read();
        if(tmp != '\n') {
            Serial.println("You pressed: " + (String)tmp);
            read_char = tmp;
        }
    }
    if(read_char != '~'){
    }
    return read_char;
}

bool serial_read_check(char inchar1, char inchar2){
    while(1){
        char read_char = serial_read_char();
        if(read_char == '~'){
            // ignore
        } else if((read_char == inchar1) || (read_char == inchar2)) {
            return 1;
        } else {
            return 0;
        }
    }
}

void options_display() {
    Serial.println(line);   // line = "===========================================";
    Serial.println("Flash file options\t\t\t\t< F >");
    Serial.println("Device Settings   \t\t\t\t< S >");
    Serial.println("EPD options       \t\t\t\t< E >");
    Serial.println("Misc Settings     \t\t\t\t< M >");
    Serial.println("Turn off device   \t\t\t\t< X >");
    Serial.println("EXIT              \t\t\t\t< 0 >");
    Serial.println(line);
}

void options_display_EPD() {
    Serial.println(line);   // line = "===========================================";
    // Serial.println("Pattern Check                                    < P >");
    Serial.println("Fill White       \t\t\t\t< W >");
    Serial.println("Fill Black       \t\t\t\t< B >");
    Serial.println("EPD options Menu \t\t\t\t< M >");
    Serial.println("Back to Main Menu\t\t\t\t< - >");
    Serial.println("EXIT             \t\t\t\t< 0 >");
    Serial.println(line);
}

void options_display_memory() {
    Serial.println(line);       // line = "===========================================";
    Serial.println("Show all file list      \t\t\t\t< 1 >");
    Serial.println("Read all log files      \t\t\t\t< 3 >");
    Serial.println("Read all log_batt files \t\t\t\t< 5 >");
    Serial.println("Delete all log files    \t\t\t\t< 6 >");
    Serial.println("Delete all image files  \t\t\t\t< 7 >");
    Serial.println("Format SPIFFS           \t\t\t\t< 8 >");
    Serial.println("Format EEPROM           \t\t\t\t< 9 >");
    Serial.println("Flash file options Menu \t\t\t\t< M >");
    Serial.println("Back to Main Menu       \t\t\t\t< - >");
    Serial.println("EXIT                    \t\t\t\t< 0 >");
    Serial.println(line);       // line = "===========================================";
}

void options_display_settings() {
    Serial.println(line);       // line = "===========================================";
    Serial.println("Developer options    \t\t\t\t< 1 >");
    Serial.println("Connection settings  \t\t\t\t< 2 >");
    Serial.println("Device log settings  \t\t\t\t< 3 >");
    Serial.println("Back to Main Menu    \t\t\t\t< - >");
    Serial.println("EXIT                 \t\t\t\t< 0 >");
    Serial.println(line);       // line = "===========================================";
}

void options_display_serial_log() {
    Serial.println(line);       // line = "===========================================";
    Serial.println("Device log all      \t\t\t\t< 1 >");
    Serial.println("Device log memory   \t\t\t\t< 2 >");
    Serial.println("Device log EPD      \t\t\t\t< 3 >");
    Serial.println("Device Summary      \t\t\t\t< 4 >");
    Serial.println("Back to Main Menu   \t\t\t\t< - >");
    Serial.println("EXIT                \t\t\t\t< 0 >");
    Serial.println(line);       // line = "===========================================";
}

void options_display_developer_options() {
    Serial.println("By turning on developer mode device will run while USB plugged");
    Serial.println("You can see the device logs\n");
    
    Serial.println(line);       // line = "===========================================";
    Serial.println("Turn on developer mode  \t\t\t\t< 1 >");
    Serial.println("Turn off developer mode \t\t\t\t< 2 >");
    Serial.println("Back to Previous        \t\t\t\t< - >");
    Serial.println("EXIT                    \t\t\t\t< 0 >");
    Serial.println(line);       // line = "===========================================";
}

void options_display_connection_settings() {
    Serial.println("By activaing offline mode device will cont connect to WiFi");
    Serial.println("This will not affected to the Rotation\n");
    
    Serial.println(line);       // line = "===========================================";
    Serial.println("Turn on offline mode  \t\t\t\t< 1 >");
    Serial.println("Turn off offline mode \t\t\t\t< 2 >");
    Serial.println("Back to Previous      \t\t\t\t< - >");
    Serial.println("EXIT                  \t\t\t\t< 0 >");
    Serial.println(line);       // line = "===========================================";
}

void display_options_developer_options() {
    options_display_developer_options();
    
    Serial.flush();
    while (dev_options) {
        char read_char = serial_read_char();
        switch(read_char) {
            case '~':
                break;
            case '1':
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "1")) {
                    Serial.println("Fail to save the changes");
                } 
                Serial.println("Your change successfully saved");
                dev_options = 0;
                break;
            case '2':
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "0")) {
                    Serial.println("Fail to save the changes");
                } 
                Serial.println("Your change successfully saved");
                dev_options = 0;
                break;
            case '0':
                opt_settings = 0;
                DISPLAY_OPTIONS = false;
                // DEBUG_MEM_EN = 0;
                Serial.println("============== EXIT =============================\n");

                break;

            case '-':
                dev_options = 0;
                break;
            default:
                Serial.println("You are wrong..!\n");
                Serial.println("Please enter number again to continue ..\n");
                options_display_developer_options();
        }
    }
}

void display_options_connection_settings() {
    options_display_connection_settings();
    Serial.flush();
    while (opt_wifi) {
        char read_char = serial_read_char();
        switch(read_char) {
            case '~':
                break;
            case '1':
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_WIFI_CON_SW, "1")) {
                    Serial.println("Fail to save the changes");
                } 
                Serial.println("Your change successfully saved");
                opt_wifi = 0;
                break;
            case '2':
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_WIFI_CON_SW, "0")) {
                    Serial.println("Fail to save the changes");
                } 
                Serial.println("Your change successfully saved");
                opt_wifi = 0;
                break;
            case '0':
                opt_wifi = 0;
                DISPLAY_OPTIONS = false;
                // DEBUG_MEM_EN = 0;
                Serial.println("============== EXIT =============================\n");
                break;
            case '-':
                opt_wifi = 0;
                break;
            default:
                Serial.println("You are wrong..!\n");
                Serial.println("Please enter number again to continue ..\n");
                options_display_connection_settings();
        }
        
    }
}


void display_options_settings() {
    options_display_settings();
    Serial.flush();
    while (opt_settings) {
        //timerWrite(timer_w, 0);
        debug_mem_en = 1;
        char read_char = serial_read_char();
        switch (read_char) {
            case '~':
                break;
            case '1':
                dev_options = true;
                display_options_developer_options();
                break;
            case '2':
                opt_wifi = 1;
                display_options_connection_settings();
                break;
                
            case 'm':
            case 'M':
                options_display_settings();
                break;

            case '0':
                opt_settings = 0;
                DISPLAY_OPTIONS = false;
                // DEBUG_MEM_EN = 0;
                Serial.println("============== EXIT =============================\n");

                break;

            case '-':
                opt_settings = 0;
                break;

            default:
                Serial.println("You are wrong..!\n");
                Serial.println("Please enter number again to continue ..\n");
                options_display_settings();
        }

    }
}
void display_options_memory(){
    options_display_memory();
    Serial.flush();
    while (optFlash) {
        //timerWrite(timer_w, 0);
        debug_mem_en = 1;
        char read_char = serial_read_char();
        switch (read_char) {
            case '~':
                break;
            case '1':
                Serial.println("========= Files list ==========");

                list_dir(SPIFFS, "/", 0);

                Serial.println(completed);
                Serial.println(enter_number);

                break;

            case '2':
                Serial.println("========= Read individual log files ... ==========");
                print_dir_individual_files_content(SPIFFS, LOG_PATH);
                Serial.println(completed);
                Serial.println(enter_number);

                break;

            case '3':
                Serial.println("========= Read all log files at once ... ==========");
                // DEBUG_MEM_EN = 0;
                print_dir_all_files_content(SPIFFS, LOG_PATH);
                Serial.println(completed);
                Serial.println(enter_number);

                break;

            case '4':
                Serial.println("========= Read individual log_batt files ... ==========");
                print_dir_individual_files_content(SPIFFS, BATT_LOG);
                Serial.println(completed);
                Serial.println(enter_number);

                break;

            case '5':
                Serial.println("========= Read all log_batt files at once ... ==========");
                // DEBUG_MEM_EN = 0;
                print_dir_all_files_content(SPIFFS, BATT_LOG);
                Serial.println(completed);
                Serial.println(enter_number);

                break;

            case '6': 
                Serial.println("========= Are you sure you want to DELETE log files? (Y/N) ==========");

                if(serial_read_check('y', 'Y')) {
                    delete_dir(SPIFFS, String(LOG_PATH));
                    delete_dir(SPIFFS, String(BATT_LOG));
                    Serial.println("completed");
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                Serial.println(enter_number);

                break;

            case '7':
                Serial.println("========= Are you sure you want to DELETE image files? (Y/N) ==========");
                if(serial_read_check('y', 'Y')) {
                    delete_dir(SPIFFS, IMG_PATH);
                    Serial.println("completed");
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }

                Serial.println(enter_number);

                break;

            case '8':
                Serial.println("========= *** Are you sure you want to FORMAT device? This will remove all logs, images .etc (Y/N) ==========");
                if(serial_read_check('y', 'Y')) {
                    Serial.println("SPIFFS is formatting. Please wait..\n");
                    format_spiffs();

                    timerWrite(timer_w, 0); //reset timer (feed watchdog)
                    
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                
                Serial.println(enter_number);
            case '9':
                Serial.println("========= *** Are you sure you want to Format EEPROM? (Y/N) ==========");
                if(serial_read_check('y', 'Y')) {
                    Serial.println("EEPROM Formatting. Please wait..\n");
                    bool erase_status = false;
                    
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_LAST, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_EVENT, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_STATUS, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_IMG_NUMBER, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP, "none");

                    if ( erase_status ) {
                        Serial.println("EEPROM Formatting Success..\n");
                    } else {

                    }

                    timerWrite(timer_w, 0); //reset timer (feed watchdog)
                    
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                
                Serial.println(enter_number);



                break;

            case 'm':
            case 'M':
                options_display_memory();

                break;

            case '0':
                optFlash = 0;
                DISPLAY_OPTIONS = false;
                // DEBUG_MEM_EN = 0;
                Serial.println("============== EXIT =============================\n");

                break;

            case '-':
                optFlash = 0;
                break;

            default:
                Serial.println("You are wrong..!\n");
                Serial.println(enter_number);
                options_display_memory();
        }
        
    }   
}

void display_options_EPD() {
    options_display_EPD();
    debug_epd_en = 1;
    while (optDisplay) {
        char read_string = serial_read_char();
        switch (read_string) {
            // case 'P':
            // case 'p':
            //     Serial.println("Pattern check");
            //     display_image("pattern");
            //     Serial.println("Pattern check complete");
            //     break;
            case 'W':
            case 'w':
                Serial.println("EPD Check > White");
                display_image("white");
                Serial.println("EPD Check > Complete");
                break;
            case 'B':
            case 'b':
                Serial.println("EPD Check > Black");
                display_image("black");
                Serial.println("EPD Check > Complete");
                break;
            case 'M':
            case 'm':
                options_display_EPD();
                break;    
            case '-':
                optDisplay = 0;
                break;
            case '0':
                optDisplay = 0;
                DISPLAY_OPTIONS = 0;
                break;
            case '~':
                break;
            default:
                Serial.println("You were wrong..!>>\n");
                Serial.println(enter_number);
                options_display_EPD();
        }
    }
}

void display_options() {
    
    DISPLAY_OPTIONS = false;
    char read_char = serial_read_char();

    if(read_char == 'D' | read_char == 'd') {
        DISPLAY_OPTIONS = true;
    }

    while(DISPLAY_OPTIONS) {
        serial_debug_en = 1;
        options_display();
        bool tmp = 1;
        while (tmp) {
            //Serial.println(F("B");
            //timerWrite(timer_w, 0);

            char read_string = serial_read_char();
        
            switch (read_string) {
                case 'F':
                case 'f':
                    optFlash = 1;
                    display_options_memory();
                    tmp = 0;
                    break;
                case 'E':
                case 'e':
                    optDisplay = 1;
                    display_options_EPD();
                    tmp = 0;
                    break;
                case 'S':
                case 's':
                    opt_settings = 1;
                    display_options_settings();
                    tmp = 0;
                    break;
                case 'X':
                case 'x':
                    Serial.println("========= *** Are you sure you want to Turn off device?  (Y/N) ==========");
                    if(serial_read_check('y', 'Y')) {
                        Serial.println("Device will Turn off withing 5 seconds. Please unplug the USB");
                        delay(5000);
                        // power_led_blink(10, 100);
                        power_off();
                    } else if(serial_read_check('n', 'N')) {
                        break;
                    }
                    break;
                case '0':
                    DISPLAY_OPTIONS = 0;
                    tmp = 0;
                    break;
                case '~':
                    break;
                default:
                    Serial.println("You were wrong..!\n");
                    Serial.println(enter_number);
                    options_display();
                
            }
        }
    }
}


#endif /* DISPLAY_OPTIONS_H  */
