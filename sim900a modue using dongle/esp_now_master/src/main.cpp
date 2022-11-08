#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "SPIFFS.h"   //have access to the methods needed both write and read from a file.
#include <FastCRC.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"
//#include <rBase64.h>
//#include <SPI.h>
//#include "EEPROM.h"
//#include "serial_debug.h"
//#include "esp_sleep_local.h"

//StaticJsonDocument<220> doc4;
//rBase64generic<2500> mybase64;
// bool esp_now_receiving = 1;
//bool esp_now_receiving1 = 1;
//String message_to_send2 = ""; 
//char buf1[45000];
//int numberOfPacket;
//int inByte = 0;         // incoming serial byte
//String missingRpacketno;

String  esp_now_incoming_string = "";
bool    esp_now_incoming_trigger = 0;
uint8_t esp_now_receive_address[6];
hw_timer_t  *timer_w = NULL;
char buf[28000];
uint8_t levels =0; 
String FileName;
String SendName;

#include "config.h"
#include "esp_now_local.h"
#include "device_config.h"
#include "crc_local.h"
#include "memory1.h"
#include "display_options1.h"
#include "esp_sleep_local.h"

uint16_t cycle_count = 0;
uint8_t ack_send_status = 0;

void setup() {
// serial_debug_en = 1;
// Initialize Serial Monitor
Serial.begin(115200);
/*if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    String texi = "|timestamp:1656564954804|event:0|action:none|end";
    strcpy(buf,texi.c_str());
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);*/

//ESP-NOW setup
//WiFi.disconnect();

timer_w = timerBegin(0, 80, true);                  // timer 0, div 80
timerAlarmEnable(timer_w);  

// Set device as a Wi-F Station
WiFi.mode(WIFI_STA);
Serial.println(WiFi.macAddress());
uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
Serial.println("chip_id: " + get_chip_id());
        
// Init ESP-NO
    if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
    }

esp_now_register_send_cb(OnDataSent);
    
esp_now_peer_info_t peerInfo = {};
peerInfo.channel = 0;  
peerInfo.encrypt = false;
uint8_t slaveAddress_1[6];
    
    for(uint8_t i = 0; i < 8; i++) {
        for(uint8_t j = 0; j < 6; j++) {
        slaveAddress_1[j] = slaveAddress[i][j]; 
        }
    memcpy(peerInfo.peer_addr, slaveAddress_1, 6);
    Serial.println(slaveAddress_1[5]);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        }
    }
    
esp_now_register_recv_cb(OnDataRecv);
if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    String texi = "|timestamp:1656564954804|event:0|action:none|end";
    strcpy(buf,texi.c_str());
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

//....................txt file read.................................// 
readFile(SD, "/text.txt"); //read all data(the saved all file names) in the permenent file
FileName = String(buf);//read char buffer data convert to string
Serial.println(FileName);
memset(buf,0,28000);//clear char buffer
String FoundFile[FileName.length()]; //all data(the saved all file names) in the permanent file is seperate one by one file name using the array
  int x1=0,x2=0;
    for(int i=0;i<(FileName.length()+1);i++){ //*1
        if(FileName.charAt(i) == ','){ //*2
          String tex = FileName.substring(x1,i);
          FoundFile[x2] = tex;
          x1 = (i+1);
          x2++;
        } //*2
    } //*1

          SendName = FoundFile[0];//reading the 1st file name from the file names separated by the array
          Serial.println(SendName);
  
 readFile(SD,SendName); //reading data from 1st file name
 Serial.println(buf); //read char buffer data

}


void loop() { //1
  //readdata.clear();
  int count =0;
cycle_count = 5500;
    while(cycle_count--) { //**1
      
  //Serial.println(count++);
        if(esp_now_incoming_trigger) {   //2
          esp_now_incoming_trigger = 0;

          //.......................slave id no found..............//
          String crc_received_str = esp_now_incoming_string.substring(0, 5);
          uint16_t crc_received = crc_received_str.toInt();
          String crc_received_str1 = esp_now_incoming_string.substring(5);
          uint16_t crc_read = get_crc16(crc_received_str1);
          uint8_t esp_now_incoming_integer;

          esp_now_incoming_integer22 =  esp_now_incoming_integer;
          uint8_t received_slaveAddress[6];
          received_slaveAddress22[6] == received_slaveAddress[6];
          crc_read22 =  crc_read;
          crc_received22 = crc_received;
          //   Serial.println(crc_received_str1);
          // strcpy(recevedata,crc_received_str1.c_str());
          deserializeJson(doc, crc_received_str1);
          JsonObject json_obj = doc.as<JsonObject>();
          Serial.println(crc_received_str1);
          String read_str = json_obj["slave_id"]; 
          String read_str2 = json_obj["ac"];
          String img_curr1 = json_obj["i_c"];
          String img_last1= json_obj["i_l"];
          String last_event1 = json_obj["l_e"];
          String last_action1 = json_obj["l_a"];
          String last_status1 = json_obj["l_s"];
          String image_count1 = json_obj["i_co"];
          String battery_voltage1 = json_obj["b_v"];
          String battery_capacity1 = json_obj["b_c"];
          String temp_11 = json_obj["t1"];
          String temp_21 = json_obj["t2"];
          // Serial.println(last_status1);
      /*   img_last = img_last1;
          img_curr = img_curr1;
          last_event = last_event1;
          last_action = last_action1;
          rtn_status = last_status1;
          image_count = image_count1;
          battery_voltage = battery_voltage1;
          battery_capacity = battery_capacity1;
          temp_1 = temp_11;
          temp_2 = temp_21;*/
          Serial.println(read_str);
          Serial.println(read_str2);
          read_str1_1 = read_str2;
          read_str22 = read_str;
    
            if(read_str1_1 == "non"){
              /*if(crc_read == crc_received) {  //4 
                esp_now_incoming_integer = read_str.toInt();
                Serial.println( esp_now_incoming_integer);
                  for(uint8_t i = 0; i < 6; i++) {  //***
                    received_slaveAddress[i] = slaveAddress[esp_now_incoming_integer - 1][i];
                  }  //***
                String message_to_send = "";
                message_to_send.reserve(250);*/
                espsend(1);//4
            /*  } else {  //*3
              ack_send_status = esp_now_send_message(received_slaveAddress, ">NO 0");
              Serial.println("CRC > N");
              } */
            } 
   
          } //2
    
          if(read_str1_1 == "All packet not recived!..."){
           // espsend(1);
            deep_sleep_for_seconds(1);
          } 
          if(read_str1_1 == "All packet recived!..."){
           deleteFile(SD, SendName); 
           FileName.remove(0,(SendName.length()+1));  
            Serial.println(FileName); 
             writeFile(SD,"/text.txt", FileName);  
             esp_now_deinit();
           /*  if(last_action == "download" || last_action == "change"){
          write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_LAST, img_last);
         break;
           }
          write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, img_curr);
          write_value_to_eeprom(EEPROM_MEM_ADDR_LST_EVENT, last_event);
          write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, last_action);
          write_value_to_eeprom(EEPROM_MEM_ADDR_LST_STATUS, rtn_status);
          write_value_to_eeprom(EEPROM_MEM_ADDR_image_count, image_count);
          write_value_to_eeprom(EEPROM_MEM_ADDR_battery_voltage, battery_voltage);      
          write_value_to_eeprom(EEPROM_MEM_ADDR_battery_capacity, battery_capacity);
          write_value_to_eeprom(EEPROM_MEM_ADDR_temp_1, temp_1);
          write_value_to_eeprom(EEPROM_MEM_ADDR_temp_2, temp_2);*/
 /*    
          } */
        //   SPIFFS.remove("/Image.txt");
        deep_sleep_for_seconds(1);  
       
          }
        delay(2);
    }  //**1

Serial.print("\n");
Serial.print("\n");
} //1