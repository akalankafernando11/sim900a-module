#ifndef MEMORY_H
#define MEMORY_H
#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"

StaticJsonDocument<350> doc;
StaticJsonDocument<290> doc1;

uint8_t received_slaveAddress22[6];
String mgs1;
int numberOfPacket1;
String message_to_send22 = "";
uint8_t ack_send_status22 = 0;
uint16_t crc_read22;
uint16_t crc_received22;
String read_str22;
uint8_t esp_now_incoming_integer22;
String read_str1_1;

void readFile(fs::FS &fs, String filename){
  Serial.printf("Reading file: %s\n", filename);
const char * path = filename.c_str();
  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
      for (int i = 0; i < file.size(); i++){
        buf[i] = file.read();
        }
          }
  file.close();
}

void writeFile(fs::FS &fs, String filename, String message){
  Serial.printf("Writing file: %s\n", filename);
  /* if(filename != "/") {
        filename =  "/" + filename;
    }*/

    const char * path = filename.c_str();
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
//uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x97, 0x37, 0x7C};
//bool esp_now_receiving1 = 1;
//String missingRpacketno;
//int FileSize;
//String readdata;
//uint8_t ack_send_status1 = 0;
//int Size;
//uint16_t crc_read_1;
// uint16_t crc_received_1;
// String savemgs = ""; 
// String message_to_send1 = ""; 
// uint8_t esp_now_incoming_integer_1;
//uint8_t received_slaveAddress1[6];
// String crc_received_str1_1;           
// String mgss;
//  int massage2;
//String mgs;
//String  esp_now_incoming_string1 = "";
//bool    esp_now_incoming_trigger1 = 0;
//String  countM = "";
void deleteFile(fs::FS &fs, String path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
//..................how find file in flash-memory..................//
void listAllFiles(){
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while(file){
      Serial.print("FILE..: ");
      Serial.println(file.name());
      file = root.openNextFile();
  }
}

//......................send the read file data....................//
bool espsend(int content){//**1
    if (content == 1) {//**2
        if(crc_read22 == crc_received22) {  //4
        esp_now_incoming_integer22 = read_str22.toInt();
            for(uint8_t i = 0; i < 6; i++) {  //***
            received_slaveAddress22[i] = slaveAddress[esp_now_incoming_integer22 - 1][i];
            }  //***
        String message_to_send = "";
        message_to_send.reserve(250);
        mgs1 = String(buf);
        memset(buf,0,28000);
        Serial.println(mgs1.length()); //file size

//.................find the packet numbers........................//
        numberOfPacket1 =((mgs1.length() % 214)==0)? 0 : 1; 
        numberOfPacket1 += mgs1.length() /214;
        Serial.println(numberOfPacket1);
        
        String sqlite3_return_string = mgs1;
        int count= 1;   
        Serial.println(">>>>>>>>>>>>>>>>>>>>");

        uint16_t send_timer = millis();
        uint16_t message_count = 0; 
            while(1) { //5

//......................file data into json message format............//
            doc1["N"] = String(count++);
            doc1["P"] = mgs1.substring(0, 214);
            mgs1.remove(0, 214);
            doc1["A"] = String(numberOfPacket1);
            String json_message = "";
            serializeJson(doc1, json_message);
            message_to_send22 =json_message;
            doc1.clear();
            Serial.println(message_to_send22);           
                if(sqlite3_return_string.length()) {  //****
                message_to_send22 += 1;
                } else {  //*****
                 message_to_send22 += 0;
                }  //*****

            message_count++;
            ack_send_status22 = esp_now_send_message(received_slaveAddress22, message_to_send22);
            message_to_send22 = "";
                if(!ack_send_status22) { //*1
                break;
                } //*1
                    if(!mgs1.length()) {  //*2
                    break;
                    } //*2   
            } //5 
        
        Serial.println(">>>>>>>>>>>>>>>>>>>>" + String(message_count));
        send_timer = millis() - send_timer;
        Serial.print("send_timer ");
        Serial.println(send_timer);
        //delay(5);   
        ESP.restart();
          
        } else {  //*3
        ack_send_status22 = esp_now_send_message(received_slaveAddress22, ">NO 0");
        Serial.println("CRC > N");
        } //*3
    }//**2
}//**1

#endif 