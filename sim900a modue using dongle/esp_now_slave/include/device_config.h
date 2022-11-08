// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xC8, 0x39, 0x84};//C8:C9:A3:C8:39:84 
//uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x97, 0x37, 0x7C};  // 4C:11:AE:CE:5D:64
//uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0x33, 0x09, 0x68};

String get_chip_id() {
    byte mac[6];
    WiFi.macAddress(mac);
    return (String)mac[5] + (String)mac[4] + (String)mac[3] + (String)mac[2] + (String)mac[1] + (String)mac[0];
}