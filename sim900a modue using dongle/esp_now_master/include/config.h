#define UART2_WRITE_WAIT_CYCLES 400
#define EEPROM_MEM_ADDR_WIFI_CON_SW 200
#define EEPROM_PRE_TIMELINES 250
//#define ESP_NOW_CONFIG_ESPNOW_RATE 40000
//bool serial_debug_en          = 0;
bool DISPLAY_OPTIONS = false;
#define EEPROM_MAX_CHAR_LENGTH 30
#define EEPROM_SIZE 4096

const char *LOG_PATH = "/log";
const char *BATT_LOG = "/log_batt";

const uint8_t EEPROM_FAIL_COUNT = 5;


int wdtTimeout  = 30000;  //time in ms to trigger the watchdog
int workTimeout = 20000;  //time in ms to trigger the watchdog


#define FORMAT_SPIFFS_IF_FAILED true

const uint16_t MAX_FILE_SIZE = 30000;       // File size => Bytes or Number of characters

const bool DEBUG_ERROR_EN   = 1;
const bool DEBUG_MSG_EN     = 0;
const bool DEBUG_WIFI_EN    = 1;
const bool DEBUG_MEM_EN     = 1;
const bool DEBUG_SQLITE_EN  = 0;
const bool DEBUG_ESPNOW_EN  = 1;
