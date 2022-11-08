#include <Arduino.h>
#include <esp_now.h>
#include <ArduinoJson.h>
#include <FastCRC.h>
#include "FS.h"
#include "SPIFFS.h"
#include <string.h>
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <rom/rtc.h>
#include <Preferences.h>
#include "EEPROM.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/rtc.h"
#include "esp_bt.h"
#include "driver/adc.h"
//#include <TimeLib.h>
#include <esp_wifi.h>

#define VARIANT "esp32"
#define CURRENT_VERSION VERSION

Preferences preferences;

#include "config.h"
uint8_t spiffs_read_buffer[IMAGE_FILE_SIZE];
uint8_t ack_send_status1 = 0;

int SenderPacket;
String AllPacketCount;

String  esp_now_incoming_string = "";
bool    esp_now_incoming_trigger = 0;

StaticJsonDocument<350> doc;
StaticJsonDocument<290> doc1;

String esp_now_incoming_all_message = "";

String spiffs_read_string((char *)0);

time_t time_stamp; // execution_time;
uint32_t image_change_time; //, millis_start;
String AppendSection((char *)0);
uint8_t sleep_time_v;

unsigned int battery_voltage;
float battery_capacity;
float device_temperature = 0;
bool usb_detect = false;
bool developer_mode = false;

esp_sleep_wakeup_cause_t wakeup_reason;
bool usb_status = 0;
uint8_t pre_status = 0;

uint8_t rotation_image_number;

uint16_t status_code = 0;

time_t /*ota_update_checking_duration = 0, wifi_connection_duration = 0, server_connection_duration = 0, payload_receiving_duration = 0,*/ image_change_duration = 0, whole_running_duration = 0;
time_t millis_starting;
int wdtTimeout = 60000; //time in ms to trigger the watchdog
hw_timer_t *timer_w = NULL;

int ReceverPacket = 0;
String ttpp;

DRAM_ATTR int interruptCounter = 0;
DRAM_ATTR int rtc_alarm_trig = 0;

float temp_1 = 0;
float temp_2 = 0;

String display_status((char *)0);
String img_last((char *)0);
String img_curr((char *)0);
String free_heap((char *)0);
String free_stack((char *)0);
String last_event((char *)0);
String last_rotate_event((char *)0);
String last_action((char *)0);
String log_status((char *)0);
String last_status((char *)0);
String rtn_status((char *)0);
String error_code((char *)0);
String rotate_image((char *)0);

//String localIP((char *)0);
String battery_status((char *)0);

TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

int rssi_display;

#include "device_config.h"
#include "general_functions.h"
// #include "utility/i2c_lib.h"
// #include "utility/i2c_lib.h"
#include "I2Cone.h"
#include "I2Ctwo.h"

// Interface_LED_driver
#include "chip_libraries/led_driver.h"
// Real time clock  PCF85063
#include "chip_libraries/real_time_clock.h"

#include "gpio_init.h"
#include "serial_debug.h"
#include "memory.h"
#include "utils.h"

//  Temperature sensor library TMP101NA/3K
#include "chip_libraries/temp_sense.h"
//  plastic logic epaper libraries
#include "epd/epd_actions.h"

#include "display_options.h"
//  Battery monitor library
#include "chip_libraries/battery_monitor.h"
#include "crc_local.h"
#include "esp_now_local.h"
#include "espnow_actions.h"
#include "esp_sleep_local.h"


/////////////////////////////////////////////
//RSSI value
void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
  // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
  if (type != WIFI_PKT_MGMT)
    return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;

  int rssi = ppkt->rx_ctrl.rssi;
  rssi_display = rssi;
}
/////////////////////////////////////////////////

void run(){ 
  unsigned int eeprom_read_time = timer_start();

  img_last += read_value_from_eeprom(EEPROM_MEM_ADDR_IMG_LAST);
  log_message(DEBUG_MSG, "Last Downloaded Image : " + img_last);

  img_curr += read_value_from_eeprom(EEPROM_MEM_ADDR_IMG_CURR);
  log_message(DEBUG_MSG, "Current image : " + img_curr);

  last_event += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_EVENT);
  log_message(DEBUG_MSG, "Last Event : " + last_event);

  last_rotate_event += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT);
  log_message(DEBUG_MSG, "Last Rotate Event : " + last_rotate_event);

  last_action += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ACTION);
  log_message(DEBUG_MSG, "Last Action : " + last_action);

  last_status += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_STATUS);
  log_message(DEBUG_MSG, "Last Status : " + last_status);
  log_message(DEBUG_TIMER, "File read : " + (String)timer_stop(eeprom_read_time));

  //////////////////////////////////////////////////////////////////////// Rotation ///////////////////////////////
  String current_image_number_str((char *)0);
  String last_rotate_event_tmp((char *)0);
  current_image_number_str += read_value_from_eeprom(EEPROM_MEM_IMG_ROTATION);
  last_rotate_event_tmp += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP);

  rotation_image_number = current_image_number_str.toInt();
  current_image_number_str.remove(0);

    if (last_rotate_event_tmp != last_rotate_event){
      rotation_image_number = 1;
      write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP, last_rotate_event);
    }
  last_rotate_event_tmp.remove(0);
    if (rotation_image_number){
      action_event_image_rotation(last_rotate_event, rotation_image_number);
    }
  //////////////////////////////////////////////////////////////////////// end Rotation ///////////////////////////////
  //temp_1 = read_temperature_sensor(0);
  //temp_2 = read_temperature_sensor(1);
  update_status_code(1); // None

  power_off_check(); // Check wether power butten is pressed
  display_options(); // Check command receive or not for display options

  uint8_t offline_mode = read_int_value_from_eeprom(EEPROM_MEM_ADDR_WIFI_CON_SW);
  uint8_t cycle_record = read_int_value_from_eeprom(EEPROM_MEM_ADDR_CYCLE_RECORD);

  log_message(DEBUG_MSG, "cycle_record: " + String(cycle_record));
    if (last_status != "none") {
    cycle_record = 0;
  }

  if (cycle_record >= CONNECTION_SKIP_CYCLES) 
  {
    cycle_record = 0;
  }
  if (offline_mode == 1)
   {
    log_message(DEBUG_MSG, "Device is offline mode !");
  }
  else if(cycle_record != 0)
  {
  log_message(DEBUG_MSG, "Device is not connecting this cycle");
  }else{
 
    }

}

void save_log()
{
  img_last.remove(0);
  img_curr.remove(0);
  last_event.remove(0);
  last_action.remove(0);
  last_rotate_event.remove(0);
  rotate_image.remove(0);
  rtn_status.remove(0);
  free_stack.remove(0);
  free_heap.remove(0);

  img_last += read_value_from_eeprom(EEPROM_MEM_ADDR_IMG_LAST);
  img_curr += read_value_from_eeprom(EEPROM_MEM_ADDR_IMG_CURR);
  last_event += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_EVENT);
  last_rotate_event += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT);
  last_action += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ACTION);
  rotate_image += String(rotation_image_number--);
  rtn_status += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_STATUS);

  free_stack += String(get_memory_free_stack());
  free_heap += String(get_memory_free_heap());

  time_stamp = rtc_get_time();

  String log_line((char *)0);
  // log_line += String(reset_reason);               
  //log_line += ",";
  log_line += time_stamp;
  log_line += ",";
  log_line += String(battery_voltage);
  log_line += ",";
  /*log_line += wifi_signal;
  log_line += ",";*/
  log_line += String(temp_1);
  log_line += ",";
  log_line += String(temp_2);
  log_line += ",";
  log_line += free_stack;
  log_line += ",";
  log_line += free_heap;
  log_line += ",";
  log_line += img_curr;
  log_line += ",";
  log_line += img_last;
  log_line += ",";
  log_line += rotate_image;
  log_line += ",";
  log_line += last_event;
  log_line += ",";
  log_line += last_rotate_event;
  log_line += ",";
  log_line += last_action;
  log_line += ",";
  log_line += rtn_status;
  log_line += ",";
  log_line += status_code;
  log_line += ",";
  log_line += image_change_time;
  log_line += ",";
 /* log_line += String(ota_update_checking_duration);
  log_line += ",";
  log_line += String(wifi_connection_duration);
  log_line += ",";
  log_line += String(server_connection_duration);
  log_line += ",";
  log_line += String(payload_receiving_duration);
  log_line += ",";*/
  log_line += String(image_change_duration);
  log_line += ",";
  log_line += String(whole_running_duration);
  log_line += ",";
  log_line += String(sleep_time_v);
  log_line += "\n";

  save_log_file(LOG_PATH, log_line);
  log_line.remove(0);
}

  volatile byte state = LOW;

//  Power button interrupt function
void IRAM_ATTR PWR_BT_INT()
{
  digitalWrite(LED_EN, LOW);
  interruptCounter++;
  detachInterrupt(32);
}

//  Power button interrupt function
void IRAM_ATTR USB_DETECT_INT()
{
  digitalWrite(SOFT_PWR_OFF, LOW);
  // interruptCounter++;
  // detachInterrupt(32);
}

//  Power button interrupt function
void IRAM_ATTR RTC_INTT()
{
  rtc_alarm_trig++;
  detachInterrupt(33);
}

// Watchdog iiteruupt function
void IRAM_ATTR resetModule(){
  log_message(DEBUG_MSG, "Processor reboot\n");
  ESP.restart();
}

void setup() {
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_160M);
  millis_starting = timer_start();

btStop();
//   esp_bt_controller_disable();
 // WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
  //esp_wifi_stop();
  //adc_power_off();

  ///////////////////////////////////////////////////////////////////////////////// watchdow dog setup /////////////////////////////////////
  timer_w = timerBegin(0, 80, true);                  // timer 0, div 80
  timerAttachInterrupt(timer_w, &resetModule, true);  // attach callback
  timerAlarmWrite(timer_w, wdtTimeout * 1000, false); // set time in us
  timerAlarmEnable(timer_w);                          // /enable interrupt
  ///////////////////////////////////////////////////////////////////////////////// end watchdow dog setup /////////////////////////////////////

  // Initialize Serial Monitor
  Serial.begin(115200); 

    if (!initialize_eeprom()){
    }

 timerWrite(timer_w, 0); //reset timer (feed watchdog)

  //Initialize the SPIFFS object
  /*  if(!SPIFFS.begin(true)){
      Serial.println("Error initialize the SPIFFS");
        while(true){}
      return;
    }*/
  if (!initialize_spiffs())
  {
    // device_error();
    // log_message(DEBUG_ERROR, "spiffs mount failed");
  }

  // Set device as a Wi-Fi Station  
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  //Serial.println("chip_id: " + get_chip_id());

  //ESP-NOW setup
  //WiFi.disconnect();

  // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
    
  // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
  // Register for a callback function that will be called when data is received 
  //esp_now_register_recv_cb(OnDataRecv1);

////////////////////////////////////////////////////////////////////////
    //CB para el RSSI
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);
/////////////////////////////////////////////////////////////////////////////


  esp_now_register_recv_cb(OnDataRecv); 

  gpio_init();
  delay(10);

   I2Ctwo.begin(I2C_SDA_2, I2C_SCL_2, 100000);
  delay(10);

  led_blinking_battery_level(100);

 rtc_begin();
  time_stamp = rtc_get_time();

 preferences.begin("on_status", false);

  wakeup_reason = esp_sleep_get_wakeup_cause();
  usb_status = digitalRead(USB_DETECT_SIG);
  pre_status = preferences.getUInt("on_status", 0);
  developer_mode = read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE);

  preferences.putUInt("on_status", 0);

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /*
  usb_status = 0;
  serial_debug_en = 1;  
  uint8_t count = 10;
    while (count){
      log_message(DEBUG_MSG, ".", true);
      delay(100);
      count--;
      power_off_check();
    }

  log_message(DEBUG_SUMMARY, "############################  Firmware version: " + String(CURRENT_VERSION));
  log_message(DEBUG_MSG, "time_stamp:" + String(time_stamp));
  log_message(DEBUG_MSG, "Vbatt = " + (String)battery_voltage + "mV");
  log_message(DEBUG_MSG, "Cbatt = " + (String)battery_capacity + "%");
  log_message(DEBUG_MSG, "millis_starting " + String(millis_starting));
  // log_message(DEBUG_MSG, "USB Charder detected");
  log_message(DEBUG_MSG, "usb_status|wakeup_reason|pre_status: " + String(usb_status) + "|" + String(wakeup_reason) + "|" + String(pre_status));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
    if (initBattMon()){
      battery_voltage = getVoltage();
      // log_message(DEBUG_MSG, "Vbatt = " + (String)battery_voltage + "mV");

      battery_capacity = getCapacity() * 0.1;
      // log_message(DEBUG_MSG, "Cbatt = " + (String)battery_capacity + "%");

      device_temperature = getCellTemperature();
    }else{
    // log_message(DEBUG_ERROR, "Battery monitor init fail");
    }

    if (!usb_status){
      write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "0");
    }

    if (usb_status){  //1// USB pluged
/////////////////////////////////////////// Charge indicationg ///////////////////////////
 bool charging = !digitalRead(BC_CHG_SIG);
    if (charging)
    {
      led_blinking_charging();
      log_message(DEBUG_MSG, "Battery charging.");
    }
    else
    {
      led_blinking_full_charge();
      log_message(DEBUG_MSG, "Device fully charged.");
    }
    attachInterrupt(digitalPinToInterrupt(USB_DETECT_SIG), USB_DETECT_INT, FALLING);
  //////////////////////////////////////////// Serial log /////////////////////////////////
      serial_debug_en = 1;
      Serial.begin(115200);
      uint8_t count = 10;
        if(developer_mode) {
          count = 0;
        }
        while (count){//4
          log_message(DEBUG_MSG, ".", true);
          delay(500);
          count--;
          power_off_check();
        }//4

       /* if (!digitalRead(USB_DETECT_SIG)) {//5
          power_off();
        }//5*/
    
      log_message(DEBUG_SUMMARY, "############################  Firmware version: " + String(CURRENT_VERSION));
    log_message(DEBUG_SUMMARY, "DEVICE ID:" + get_chip_id());
    log_message(DEBUG_MSG, "time_stamp:" + String(time_stamp));
    log_message(DEBUG_MSG, "Vbatt = " + (String)battery_voltage + "mV");
    log_message(DEBUG_MSG, "Cbatt = " + (String)battery_capacity + "%");
    log_message(DEBUG_MSG, "millis_starting " + String(millis_starting));
    // log_message(DEBUG_MSG, "USB Charder detected");
    log_message(DEBUG_MSG, "usb_status|wakeup_reason|pre_status: " + String(usb_status) + "|" + String(wakeup_reason) + "|" + String(pre_status));
      
    preferences.putUInt("on_status", 2);
    preferences.end();

      power_off_check();
      display_options(); // Check command receive or not for display options

        if (read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE)){//6
          log_message(DEBUG_MSG, "Developer mode is on");
          // check_for_updates();//6
        }else{//7
          log_message(DEBUG_MSG, "Developer mode is off");
           if (!charging) {
             // check_for_updates(); 
           }
          display_options(); // Check command receive or not for display options

            if (read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE)){//8
              log_message(DEBUG_MSG, "Device restarting with developer mode");
              ESP.restart();
            }//8

            if ((pre_status == 2) & (wakeup_reason == 3)){//9
              display_options(); // Check command receive or not for display options
                if (read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE)){//10
                  log_message(DEBUG_MSG, "Device restarting with developer mode");
                  ESP.restart();
                }//10
          
              log_message(DEBUG_MSG, "\n", 1);
            }//9
          power_off_check();

          log_message(DEBUG_MSG, "device going to sleep");
            if (charging){//11
              save_battery_log(battery_voltage, battery_capacity, "CHR"); // Charging
              deep_sleep_while_charging(600);//11
            }else{//12
              save_battery_log(battery_voltage, battery_capacity, "FUL"); // Battery full
              deep_sleep_when_pluged_not_charging(600);
            }//12
        }//7
    }//1
    else if ((pre_status == 2) & (wakeup_reason == 2)){ // USB cable unplugged13
      save_battery_log(battery_voltage, battery_capacity, "UNP"); // UNP: USB un-plugged

      log_message(DEBUG_MSG, "USB Unplugged > Power off.");
      power_off();
    }//13
    else if ((pre_status == 1) & (wakeup_reason == 3)){//14
      battery_status.remove(0);
    
      save_battery_log(battery_voltage, battery_capacity, "PBS"); // PBS: Power butten pressed while sleeping
      log_message(DEBUG_MSG, "User pressed power butten > Power off.");
      power_off();
    }//14
  attachInterrupt(digitalPinToInterrupt(ON_BTN_SIG), PWR_BT_INT, HIGH);
  led_blinking_battery_level(battery_capacity);
  
  timerWrite(timer_w, 0); //reset timer (feed watchdog)

  power_off_check(); // Check wether power butten is pressed
  display_options(); // Check command receive or not for display options

    if (battery_voltage < VBATT_MIN){
      log_message(DEBUG_ERROR, "Battery is empty");

      save_battery_log(battery_voltage, battery_capacity, "EMT");

        if (!digitalRead(USB_DETECT_SIG)){
          log_message(DEBUG_MSG, "battery:" + (String)battery_voltage);
          log_message(DEBUG_MSG, "Power off");
          log_message(DEBUG_MSG, "Powering off due to the low battery");
          power_off();
        }
    }
    if (battery_voltage > 9999){ // battery voltage set to zero when it wrong
      battery_voltage = 0;
    }
  timerWrite(timer_w, 0); //reset timer (feed watchdog)
}

void loop(){  //1

  power_off_check(); // Check wether power butten is pressed
  display_options(); // Check command receive or not for display options

  run();

 //...............send json format to slave id no master device......................//
  doc["slave_id"] = "001";
  doc["ac"] = "non";
  String json_message = "";
  serializeJson(doc, json_message);

  // CRC generation    
  uint16_t CRC16_read = get_crc16(json_message);
  char crc_buf[6];
  sprintf(crc_buf, "%5d", CRC16_read);
  String esp_now_send_message_a;
  esp_now_send_message_a += String(crc_buf);
  esp_now_send_message_a += json_message;
    
  Serial.print("esp_now_send_message_a.length > ");
  Serial.println(esp_now_send_message_a.length());
  Serial.println("esp_now_sending>" + esp_now_send_message_a);
    
  uint16_t time1, time2;
  //uint16_t time3, time4;
  bool esp_now_receiving = 1;
 // delay(600);
    if(esp_now_send_message(broadcastAddress, esp_now_send_message_a)) {  //2
    esp_now_send_message_a.remove(0);
      time1 = millis(); 
        while(esp_now_receiving) {  //3
            while(1) {  //4
              time2 = millis() - time1;
                if(esp_now_incoming_trigger) {   //5
                  break;
                } //5
                if(time2 > 10000) { //6
                  break;
                }  //6
            } //4
            if(esp_now_incoming_trigger) { //7
              esp_now_incoming_trigger = 0;
                if(!esp_now_incoming_string.endsWith("1")) { //8
                  esp_now_receiving = 0;
                }  //8
//..........................recever file data slow................................//
              deserializeJson(doc1, esp_now_incoming_string);
              JsonObject json_obj1 = doc1.as<JsonObject>();
              String SendPacket = json_obj1["P"];
              String AllSendPacket = json_obj1["A"];
              String SendPacketID = json_obj1["N"];
              String NonPacket = json_obj1["A1P"];
              Serial.print("no: ");
              Serial.print(SendPacketID);
              Serial.print("\t");
              Serial.print(SendPacket);
              doc1.clear();
              Serial.print("\t");
              Serial.println(NonPacket);
              AllPacketCount += AllSendPacket + ",";
              esp_now_incoming_all_message += SendPacketID + "%" + SendPacket +"#";//7  
            SendPacket.remove(0);
           AllSendPacket.remove(0);
           SendPacketID.remove(0);
           NonPacket.remove(0);
            }else { //*
              Serial.println("ESP_NOW not received");
              esp_now_receiving = 0;
            } //*
        }  //3
        esp_now_incoming_string.clear();
    } //2

  NoDupicatePacket(esp_now_incoming_all_message); 
  esp_now_incoming_all_message.remove(0);
  //Serial.print("\n");
  
  //////////////////////////////////////////////////////////////////////////////////////
 Serial.print("\n");
 Serial.print("RSSI: ");
 Serial.println(rssi_display);
 Serial.println("dBm");
    /////////////////////////////////////////////////////////////////////////////////////

    if(SenderPacket>0){
      delay(2);
      Serial.print("\n");
      Serial.print("Recever Packets: ");
      Serial.println(ReceverPacket);
      Serial.print("sender Packets: ");
      Serial.println(SenderPacket);
      Serial.print("Recever loss Data packet: ");
      double lossPackets = SenderPacket - ReceverPacket;
      Serial.println(lossPackets);

        if(lossPackets != 0){ 
          Serial.println("All packet not recived!...");
          doc["slave_id"] = "01";
          doc["ac"] = "All packet not recived!...";
          String json_mgs1 = "";
          serializeJson(doc, json_mgs1);
          uint16_t CRC16_read1 = get_crc16(json_mgs1);
          char crc_buf2[6];
          sprintf(crc_buf2, "%5d", CRC16_read1);
          String esp_now_send1;
          esp_now_send1 += String(crc_buf2);
          esp_now_send1 += json_mgs1;
          //Serial.print(esp_now_send1);
          doc.clear();
          ack_send_status1 = esp_now_send_message(broadcastAddress, esp_now_send1);
          //ESP.restart();
          esp_now_send1.remove(0);
          deep_sleep(10); 
        }
        
 sleep_time_v = CYCLE_TIME - whole_running_duration / 1000;
    if (sleep_time_v > CYCLE_TIME) {
      sleep_time_v = 20;
    }
    if(sleep_time_v == 0) {
      sleep_time_v = 1;
    }

       if(lossPackets == 0){ 
        //  filewite();
         Breaking_in_to_parts();
          action();
          timerWrite(timer_w, 0);//reset timer (feed watchdog)
          last_action.remove(0);
          last_action += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ACTION);
          if (AUTO_FORMAT){
          if (status_code == 3318 | status_code == 3320){
          // delete_dir(SPIFFS, "/log");
          // delete_dir(SPIFFS, "/log_batt");
          delete_dir(SPIFFS, IMG_PATH);
        }
      }
 if (AUTO_IMAGE_CHANGE){

        String tmp_image_name = IMG_PATH + img_last;

        if (status_code == 0)
        {
          if (!display_image(tmp_image_name))
          {
            display_status = "FL";
          }
          else
          {
            display_status = "OK";
          }
          tmp_image_name.remove(0);
        }
      }
      //  }

//cycle_record++;
 // write_value_to_eeprom(EEPROM_MEM_ADDR_CYCLE_RECORD, String(cycle_record));

  switch (status_code){
              case 1:
                rtn_status += "none";
                break;
              case 2: // Change success
              case 3: // Downaload success
              case 4: // Delete success
              case 5: // Format success
              case 6: // Rotate success
              case 7: // File_count success
              case 8: // Display clear (white) success
              case 9: // Display clear (black) success
                rtn_status += ACT_SUCESS;
                break;
              case 1200:
              case 1300:
              case 1401:
              case 1402:
                rtn_status += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_STATUS);
                break;
              default:
                rtn_status += "error_";
                rtn_status += String(status_code);
            }
          write_value_to_eeprom(EEPROM_MEM_ADDR_STAT_CODE, String(status_code));

          log_message(DEBUG_MSG, "STAT CODE: " + String(status_code));
          log_message(DEBUG_MSG, "RTN_STATUS: " + rtn_status);

          write_value_to_eeprom(EEPROM_MEM_ADDR_LST_STATUS, rtn_status);
          //push_pull();//no dplicate packet and all packet save
  temp_1 = read_temperature_sensor(0);
  temp_2 = read_temperature_sensor(1);
          /////////////////////////////////////
     //     if(lossPackets == 0){
            String dir_path((char *)0);
        dir_path += IMG_PATH;
 
          if (last_event != "0")
        {
            dir_path += "/";
            dir_path += last_event;
        }
        int image_count = count_files(SPIFFS, dir_path);
          Serial.print("\n");
          Serial.println("All packet recived!...");
          Serial.print("\n");
          doc["slave_id"] = "01";
          doc["ac"] = "All packet recived!...";
          doc["i_c"] = img_curr;
          doc["i_l"] = image_name;
          doc["l_e"] = current_event;
          doc["l_a"] = current_action;
          doc["l_s"] = rtn_status;
          doc["i_co"] = String(image_count);
          doc["b_v"] = String(battery_voltage);
          doc["b_c"] = String(battery_capacity);
          doc["t1"] = String(temp_1);
          doc["t2"] = String(temp_2);
          String json_mgs2 = "";
          serializeJson(doc, json_mgs2);
          uint16_t CRC16_read2 = get_crc16(json_mgs2);
          char crc_buf1[6];
          sprintf(crc_buf1, "%5d", CRC16_read2);
          String esp_now_send2;
          esp_now_send2 += String(crc_buf1);
          esp_now_send2 += json_mgs2;
          Serial.println(esp_now_send2);
          doc.clear();
         /* if(current_action=="change" && rtn_status!= "success"){
            delay(8000);
            Serial.println("ggggg");
          ack_send_status1 = esp_now_send_message(broadcastAddress, esp_now_send2);
          }else{*/
            delay(70);
          ack_send_status1 = esp_now_send_message(broadcastAddress, esp_now_send2);
         // }
          esp_now_send2.remove(0);
          esp_now_deinit();
       // Serial.println(payloads);
          timerWrite(timer_w, 0); 
   
   
  power_off_check(); // Check wether power butten is pressed
  display_options(); // Check command receive or not for display options

  // cpu_freq_set("low");

  display_options();

  String image_path((char *)0);
  image_path += IMG_PATH;
  image_path += "/";
  image_path += last_rotate_event;
  //int 
  image_count = count_files(SPIFFS, image_path);
  image_path.remove(0);
  
  battery_status += " | " + (String)battery_voltage + "mV | " + (String)battery_capacity + "%";
  timerWrite(timer_w, 0); //reset timer (feed watchdog)

  time_stamp = rtc_get_time();
  whole_running_duration = timer_stop(millis_starting);
//push_pull();
 
  // sleep_time_v = SLEEP_TIME;
    if (SAVE_DEVICE_LOGS){
      save_log();
    }
 

  log_message(DEBUG_SUMMARY, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>  ", true);
  rtc_printTime();
  log_message(DEBUG_SUMMARY, "FIRMWARE VERSION:" + String(CURRENT_VERSION));
  log_message(DEBUG_SUMMARY, "DEVICE ID:" + get_chip_id());
  log_message(DEBUG_SUMMARY, "DEVICE_ID_NEW:" + WiFi.macAddress());
  //log_message(DEBUG_SUMMARY, "DEVICE IP:" + localIP);
  // log_message(DEBUG_SUMMARY, "reset_reason:" + String(reset_reason));
  log_message(DEBUG_SUMMARY, "TIME_STAMP:" + String(time_stamp));
  log_message(DEBUG_SUMMARY, "Battery: " + battery_status);
  log_message(DEBUG_SUMMARY, "Temperature Reading (Sensor 1): " + String(temp_1) + " Celcius");
  log_message(DEBUG_SUMMARY, "Temperature Reading (Sensor 2): " + String(temp_2) + " Celcius");
  log_message(DEBUG_SUMMARY, "ESPNOW:" + String(rssi_display) + "dB");
  log_message(DEBUG_SUMMARY, "CRNT_IMAGE:" + img_curr);
  log_message(DEBUG_SUMMARY, "LAST_IMAGE:" + img_last);
  log_message(DEBUG_SUMMARY, "ROTATE_IMAGE:" + rotate_image + "/" + String(image_count));
  log_message(DEBUG_SUMMARY, "LAST RECEIVED EVENT NO:" + last_event);
  log_message(DEBUG_SUMMARY, "ROTATE EVENT NO:" + last_rotate_event);
  log_message(DEBUG_SUMMARY, "ACTION:" + last_action);
  log_message(DEBUG_SUMMARY, "STATUS:" + rtn_status);
  log_message(DEBUG_SUMMARY, "STAT_CODE:" + String(status_code));
  //log_message(DEBUG_SUMMARY, "ota_update_checking_duration:" + String(ota_update_checking_duration) + "ms");
  //log_message(DEBUG_SUMMARY, "wifi_connection_duration:" + String(wifi_connection_duration) + "ms");
  //log_message(DEBUG_SUMMARY, "server_connection_duration: " + String(server_connection_duration) + "ms");
  //log_message(DEBUG_SUMMARY, "payload_receiving_duration: " + String(payload_receiving_duration) + "ms");
  log_message(DEBUG_SUMMARY, "image_change_duration: " + String(image_change_duration) + "ms");
  log_message(DEBUG_SUMMARY, "whole_running_duration:" + String(whole_running_duration) + "ms");
  log_message(DEBUG_SUMMARY, "MEM STACK: " + free_stack);
  log_message(DEBUG_SUMMARY, "MEM HEAP: " + free_heap);
  log_message(DEBUG_SUMMARY, "Sleep_time:" + String(sleep_time_v) + "s");

  // log_message(DEBUG_SUMMARY, "wakeup_reason:" + String(wakeup_reason));
  // log_message(DEBUG_SUMMARY, "pre_status:" + String(pre_status));

  log_message(DEBUG_SUMMARY, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

  power_off_check(); // Check wether power butten is pressed

  preferences.putUInt("on_status", 1);
  preferences.end();
    btStop();

  //action_event_image_change("1","puma.jpg");
  //display_image("/i/1/002puma.jpg");
 // Serial.println(String(buf));
  display_options();
 
  deep_sleep_for_seconds(15);
   }
          ////////////////////////////////////
                 } 
                 
deep_sleep(sleep_time_v);

}//1