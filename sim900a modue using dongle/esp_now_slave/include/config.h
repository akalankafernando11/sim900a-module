#ifndef CONFIG_H
#define CONFIG_H

#define EEPROM_MAX_CHAR_LENGTH 30
#define EEPROM_SIZE 4096

#define EEPROM_MEM_ADDR_IMG_LAST 0
#define EEPROM_MEM_ADDR_IMG_CURR 50
#define EEPROM_MEM_ADDR_LST_ACTION 100
#define EEPROM_MEM_ADDR_LST_STATUS 150
#define EEPROM_MEM_ADDR_WIFI_CON_SW 200
#define EEPROM_PRE_TIMELINES 250
#define EEPROM_MEM_ADDR_STAT_CODE 300
#define EEPROM_MEM_IMG_ROTATION 350
#define EEPROM_MEM_IMG_NUMBER 400
#define EEPROM_MEM_ADDR_LST_EVENT 450
#define EEPROM_MEM_ADDR_LST_ROTATE_EVENT 500
#define EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP 550


// #define EEPROM_MEM_LAST_SLEEP_TIME              600

#define EEPROM_MEM_ADDR_SERIAL_DEBUG_EN         1000
#define EEPROM_MEM_ADDR_DEBUG_EPD_EN            1005
#define EEPROM_MEM_ADDR_DEBUG_MEM_EN            1010
#define EEPROM_MEM_ADDR_DEBUG_SUM_EN            1015
#define EEPROM_MEM_ADDR_DEVELOPER_MODE          1020
#define EEPROM_MEM_ADDR_CYCLE_RECORD            1025

const uint8_t EEPROM_FAIL_COUNT = 5;

const uint8_t SLEEP_TIME = 10;
const uint8_t CYCLE_TIME = 30;
const uint8_t CONNECTION_SKIP_CYCLES = 0;

#define PCB_VERSION_6_0
// const String FIRMWARE_VERSION = "v0.0.3";

bool DISPLAY_OPTIONS = false;

const bool AUTO_IMAGE_CHANGE     = 0;
const bool AUTO_FORMAT           = 0;
const bool DELETE_AFTER_CHANGE   = 1;
const bool SAVE_DEVICE_LOGS      = 1;

//const uint8_t IMAGE_ORIENTATION = 2;
//wifi
//const int WIFI_RETRY_COUNT = 20;

const int VBATT_MIN = 3400;                 // Voltage in miliVolts
// const int VBATT_LOW = 3550;

const uint16_t MAX_FILE_SIZE = 24026; // File size => Bytes or Number of characters
const uint16_t IMAGE_FILE_SIZE = 24026;;

const uint16_t CLIENT_CONNECTION_TIMEOUT = 18000; // Time in ms

// const char* WIFI_SSID      = "Mi9T";
// const char* WIFI_PASSWORD  = "WiFi@1234";
// const char* WIFI_SSID      = "DIVOLGO_MIFI";
//const char* WIFI_SSID      = "DIVOLGO_D";
//const char* WIFI_SSID      = "mFi_03A4B8";
// const char* WIFI_SSID      = "Tech Labs-MiFi";
//const char* WIFI_PASSWORD  = "asdf1234";
//const char* WIFI_PASSWORD  = "79961992";
// const char* WIFI_PASSWORD  = "password1";
// const char* WIFI_PASSWORD  = "Techlabs20!9";
// const char* WIFI_SSID      = "Technology Providers";
// const char* WIFI_PASSWORD  = "12345678";

// file paths
const char *IMG_PATH = "/i";
const char *DEF_IMG_PATH = "/j";
const char *LOG_PATH = "/log";
const char *BATT_LOG = "/log_batt";

// const String IMG_LAST   = "/stats/last_image";
// const String IMG_CURR   = "/stats/current_image";
// const String LST_ACTION = "/stats/last_action";
// const String LST_STATUS = "/stats/last_status";
// const String IMAGE_LIST_PATH = "/images/list";

// actions
const String ACT_NONE = "none";
const String ACT_CHANGE = "change";
const String ACT_DOWNLOAD = "download";
const String ACT_FORMAT = "format";
const String ACT_DELETE = "delete";
const String ACT_ROTATE = "rotate";
const String ACT_WHITE = "white";
const String ACT_BLACK = "black";
const String ACT_FILE_COUNT = "file-count";
const String ACT_SUCESS = "success";

//#define ESP 1
/*// 1= Production, 2=DEV or localhost
#define WEBHOST 7 // Running with webhost
#define HTTPS 1
#define HTTP 0

#if WEBHOST == 1
// const char* HOST   = "34.73.62.212";   // Staging

const char *HOST = "35.227.121.131"; // Production
const int PORT = 80;
const String CLIENT_ADDR = "http://" + (String)HOST + ":" + (String)PORT;

#elif WEBHOST == 2
const char *HOST = "192.168.8.143"; // Jenkins cloud functions
const int PORT = 80;
const String CLIENT_ADDR = "http://" + (String)HOST + ":" + (String)PORT;

#elif WEBHOST == 3
const char *HOST = "divolgo-controlpanel-uk5dsv7paa-uc.a.run.app";

#elif WEBHOST == 4
const char *HOST = "35.227.121.131/api/ack";
const int protocol = HTTP;

#elif WEBHOST == 5
const char *HOST = "cpanel-stg.divolgodigital.com/api/ack";
const int protocol = HTTPS;

#elif WEBHOST == 6
const char *HOST = "cpanel-qa.divolgodigital.com/api/ack";
const int protocol = HTTPS;
*/
/*#elif WEBHOST == 7
const char *HOST = "control-panel-simulator.herokuapp.com/api/ack";
const int protocol = HTTPS;*/


/*#else
//const char *HOST = "control-panel-simulator.herokuapp.com/api/ack";
//const int protocol = HTTPS;
const char *HOST = "192.168.8.109";
const int PORT = 8080;
const String CLIENT_ADDR = "http://" + (String)HOST + ":" + (String)PORT;
#endif*/

#endif /* CONFIG_H */
