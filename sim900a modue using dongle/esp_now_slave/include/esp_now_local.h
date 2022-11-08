const uint8_t ESPNOW_SEND_TRY_COUNT = 20;

// Variable to store if sending data was successful
int8_t delivery_report = 0;
/// Create a struct_message to hold incoming sensor readings
char incomingReadings[250];

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status ==0){
        delivery_report = 1;
    }
    else{
        delivery_report = 0;
    }
}


// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
    esp_now_incoming_string = String(incomingReadings);
     esp_now_incoming_string.remove(len);
    esp_now_incoming_trigger = 1;
}

bool esp_now_send_message(uint8_t broadcastAddress[], String message_string) {
char send_message[message_string.length()];
message_string.toCharArray(send_message, message_string.length() + 1);   
uint8_t retry_count = 0;
    while(delivery_report == 0) {
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_message, sizeof(send_message));
    delay(10);
        if (result == ESP_OK) {
        Serial.print('.');
            if (delivery_report) {
            Serial.println("Delivery Sucess");
            delivery_report = 0;
            return true;
            }
        }else {
        Serial.print('-');
        }
            if(retry_count >= ESPNOW_SEND_TRY_COUNT) {
            Serial.println("Delivery Fail");
            return false;
            }   
        retry_count++;
        }
}
