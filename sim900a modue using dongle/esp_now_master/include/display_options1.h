#ifndef DISPLAY_OPTIONS_H
#define DISPLAY_OPTIONS_H


//StaticJsonDocument<220> doc1;
uint8_t ack_send_status2 = 0;
//uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x97, 0x37, 0x7C};
int8_t delivery_report2 = 0;
int report;
bool optFlash = false;

String completed = "==================== COMPLETED ====================";
String line = "====================";
String enter_number = "Please enter number again to continue\n";

bool optDisplay = false;

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

/*
void options_display() {
    Serial.println(line);   // line = "===========================================";
    Serial.println(F("Turn off espnow                                  < X >"));
    Serial.println(F("EXIT                                             < 0 >"));
    Serial.println(line);
}

void options_display_espnow() {
    Serial.println(line);       // line = "===========================================";
    Serial.println(F("Device send the Image                           < 1 >"));
    Serial.println(F("despay option                                   < M >"));
    Serial.println(F("Back to Main Menu                               < - >"));
    Serial.println(F("EXIT                                            < 0 >"));
    Serial.println(line);       // line = "===========================================";
}*/
/*
    void display_options_espnow(){
    options_display_espnow();
    Serial.flush();
    while (optFlash) {
        //timerWrite(timer_w, 0);
        debug_mem_en = 1;
        char read_char = serial_read_char();
        switch (read_char) {
            case '~':
                break;
            case '1':
                Serial.println(F("=========send=========="));
  if (!espsend(1)) {
                    Serial.println("Fail to save the changes");
                } 
               
                Serial.println(F("completed"));
                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case 'm':
            case 'M':
                options_display();

                break;

            case '0':
                optFlash = 0;
                DISPLAY_OPTIONS = false;
                Serial.println(F("============== EXIT =============================\n"));

                break;

            case '-':
                optFlash = 0;
                break;

            default:
                Serial.println(F("You are wrong..!\n"));
                Serial.println(F("Please enter number again to continue ..\n"));
                options_display_espnow();
        }
        
    }   
}*/


void display_options(){  
    DISPLAY_OPTIONS = false;
    char read_char = serial_read_char();
Serial.println(read_char);
    if(read_char == 'D' | read_char == 'd') {
        DISPLAY_OPTIONS = true; 
        Serial.println("log");   
    }

    while(DISPLAY_OPTIONS) {
      //  serial_debug_en = 1;
        espsend(1);
        bool tmp = 1;
        while (tmp) {
            //Serial.println(F("B");
            //timerWrite(timer_w, 0);

      //      char read_string = serial_read_char();
        /*
            switch (read_string) {
                case 'X':
                case 'x':
                  optFlash = 1;
                    //options_display_espnow();
                    display_options_espnow();
                    tmp = 0;
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
                
  //          }*/
        }
    }
}

#endif /* DISPLAY_OPTIONS_H  */