#include "config.h"
#include "serial_debug.h"
uint16_t status_update_code = 0;

String request((char *)0);
String image_path((char *)0);
String image_name((char *)0);
String image_name_s((char *)0);
String image_type((char *)0);
String image_duration((char *)0);
String current_event((char *)0);
String image_data_color((char *)0);
String image_meta_data((char *)0);
String current_action((char *)0);

String rotate_param((char *)0);
uint8_t action_number = 0;
uint16_t file_size = 0;

bool action_event_image_change(String event, String image_name){
/* String free_heap((char *)0);
String free_stack((char *)0);
  free_stack += String(get_memory_free_stack());
  free_heap += String(get_memory_free_heap());
  log_message(DEBUG_MSG, "MEM STACK: " + free_stack);
  log_message(DEBUG_MSG, "MEM HEAP: " + free_heap);*/
    String image_path((char *)0);
    timerWrite(timer_w, 0); //reset timer (feed watchdog)
     String img_curr_temp((char *)0);
    img_curr_temp += img_curr.substring(0, img_curr.length());

        if (image_name != "default.jpg" && image_name.endsWith(img_curr_temp)){
        log_message(DEBUG_MSG, "Same image is displaying right now");
        // update_status_code(8802);      // There is only one image to rotation
        return true;
    }
    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, image_name))
    { // Image name
        return false;
    }

    String image_name_tmp((char *)0);
        if (image_name == "default.jpg"){
            image_path += DEF_IMG_PATH;
            image_path += "/";
            image_path += event;
        }else{
            image_path += IMG_PATH;
            image_path += "/";
            image_path += event;
        }  
    image_name_tmp = find_file(SPIFFS, image_path, image_name, 0);
        if (image_name != NULL && image_name != ""){
                if (!display_image(image_name_tmp)){
                    return false;
                }
                if (DELETE_AFTER_CHANGE){
                    String rotation_num_str = read_value_from_eeprom(EEPROM_MEM_IMG_ROTATION);
                    uint8_t rotation_num = rotation_num_str.toInt();
                        if (!rotation_num && image_name != "default.jpg"){
                            image_name_tmp = find_file(SPIFFS, IMG_PATH, image_name, 0);
                            delete_file(SPIFFS, "/", image_name_tmp);
                        }
                }
        }
    return true;
}


uint8_t fail_count = 0;
bool action_event_image_rotation(String event, uint8_t image_number){
    timerWrite(timer_w, 0); //reset timer (feed watchdog)
    String image_path((char *)0);
    image_path += IMG_PATH;
    image_path += "/";
    image_path += event;
    int image_count = count_files(SPIFFS, image_path);
        if (image_count == 0){
            log_message(DEBUG_ERROR, "No image to rotate");
            return false;
        }
        else if (image_number > image_count){
            image_number = 1;
        }
    log_message(DEBUG_MSG, "Image count = " + (String)image_count);
    String image_name((char *)0);
    image_name += find_file_by_number(SPIFFS, image_path, image_number);
        if (image_name == "0"){
            image_number++;
            log_message(DEBUG_ERROR, "Image not found");
            fail_count++;
                if (fail_count > 5){
                    image_path.remove(0);
                    return false;
                }
            action_event_image_rotation(event, image_number);
        }
        else if (!validate_file_size(SPIFFS, "/", image_name, IMAGE_FILE_SIZE, true)){
            image_number++;
            log_message(DEBUG_ERROR, "File size validation fail");
            fail_count++;
        }else{
            image_name.replace(image_path, "");
            image_name = image_name.substring(4, image_name.length());
            log_message(DEBUG_MSG, "event: " + event + " image_name: " + image_name);
            action_event_image_change(event, image_name);
            image_number++;
                if (image_count < image_number){
                    image_number = 1;
                }
            write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, String(image_number)); // Image number
            write_value_to_eeprom(EEPROM_MEM_IMG_NUMBER, String(image_number));   // Image number
            image_path.remove(0);
            return true;
        }
}

                            //REMOVE DUPICATE PACKET//
bool NoDupicatePacket(String p){
  int AllParray[AllPacketCount.length()];
  int x1=0,x2=0;
    for(int i=0;i<(AllPacketCount.length()+1);i++){ //*1
        if(AllPacketCount.charAt(i) == ','){ //*2
          String tex = AllPacketCount.substring(x1,i);
          AllParray[x2] = tex.toInt();
          x1 = (i+1);
          x2++;
        } //*2
    } //*1

    for(int k=0; k<=x2-1; k++){ //*3
        if(AllParray[k]!=0){
          SenderPacket = AllParray[k];
        }
    }  //*3
    AllPacketCount.remove(0);
//.........................string convert to string array................................//
    String strArray[300]; //empty string array
    int l1 = 0, l2 = 0;
        for(int i=0;i<p.length();i++){ //*1
            if(p.charAt(i) == '#'){ //*2
                String tex = p.substring(l1,i);
                strArray[l2] = tex;
                l1 = (i+1);
                l2++;
            } //*2
        } //*1

//......................no repete find mising packet String..............................//
    Serial.println("non repeat String packet: ");  
    int l3 = 0;
        while (l3<=l2-1){ //*4
                for(int j= l3; j <= l2-1; j++){ //66
                        if(strArray[l3] != strArray[j+1]){//55
                            String mpm = String(strArray[l3]);
                            int pos = mpm.indexOf("%");
                            String Cnumber1 = mpm.substring(0,pos);
                            String sub1 = mpm.substring(pos+1);
                            //strcpy(buf1,sub1.c_str());
                            ReceverPacket++;
                            Serial.print("SP: ");
                            Serial.print(Cnumber1);
                            Serial.print("\t");
                            Serial.println(sub1);
                            AppendSection += sub1;
                            //strcat(buf,buf1);
                            delay(10);
                        }else{
                            ttpp += String(strArray[13]) + ",";
                        }
                    l3++;
                    strArray[j] = "0";
                }
        }
 //   p.clear();
    //     Serial.println(payloads.length());
}

bool Breaking_in_to_parts(){

    timerWrite(timer_w, 0);
    String ImageDataArray[300]; //empty string array
    int a1 = 0, a2 = 0;
        for(int i=0;i<AppendSection.length();i++){ //*1
                if(AppendSection.charAt(i) == '|'){ //*2
                    String tex = AppendSection.substring(a1,i);
                    ImageDataArray[a2] = tex;
                    a1 = (i+1);
                    a2++;
                    tex.remove(0);
                } //*2
        } //*1
        for(int k=0; k<=a2-1; k++){ //*3
            String ReservedPortion = String(ImageDataArray[k]);
            int pos1 = ReservedPortion.indexOf(":");
            String ColonPointBefore = ReservedPortion.substring(0,pos1+1);
            String ColonPointAfter = ReservedPortion.substring(pos1+1);
                if(ColonPointBefore =="timestamp:"){
                    Serial.print(ColonPointBefore);
                    Serial.println(ColonPointAfter);
                    bitWrite(status_update_code, 0, 1);
                    time_t time_stamp_receive = ColonPointAfter.toInt();
                    time_t time_stamp_2021_01_01 = 1609502400;
                        if (time_stamp_2021_01_01 > time_stamp_receive){
                            log_message(DEBUG_ERROR, "wrong timestamp");
                            update_status_code(2401);
                            return false;
                        }else{
                            time_stamp = rtc_get_time();
                            time_t time_def = time_stamp - time_stamp_receive;
                                if (time_def > 100 | time_def < -100){
                                    log_message(DEBUG_MSG, "RTC Time Updated");
                                    rtc_set_time(time_stamp_receive);
                                }
                        }
                        AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                      //  mpm1.remove(0);
                    //Cnumber11.remove(0);
                    //sub11.remove(0);
                }
                if(ColonPointBefore =="event:"){
                    log_message(DEBUG_MSG, "event:           \t\t" + ColonPointAfter);
                    current_event += ColonPointAfter;
                        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_EVENT, current_event)){
                            return false;
                        }
                    bitWrite(status_update_code, 1, 1);
                    AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                   /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
                if(ColonPointBefore =="action:"){
                    current_action += ColonPointAfter;
                    log_message(DEBUG_MSG, "action:       \t\t" + ColonPointAfter);  
                        if (ColonPointAfter == "none"){
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_NONE)){
                                    return false;
                                }
                            action_number = 1;
                        }  
                        else if (ColonPointAfter == "change"){
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_CHANGE)){
                                    return false;
                                }
                            action_number = 2;
                        }
                        else if (ColonPointAfter == "download"){
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_DOWNLOAD)){
                                    return false;
                                }
                            action_number = 3;
                        }
                        else if (ColonPointAfter == "delete"){
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_DELETE)){
                                    return false;
                                }
                            action_number = 4;
                        }
                        else if (ColonPointAfter == "format"){
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_FORMAT)){
                                    return false;
                                }
                            action_number = 5;
                        }
                        else if (ColonPointAfter == "rotate"){
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_ROTATE)){
                                    return false;
                                }
                            action_number = 6;
                        }
                        else if (ColonPointAfter == "file-count"){
                            action_number = 7;
                        }
                        else if (ColonPointAfter == "white"){
                            action_number = 8;
                        }
                        else if (ColonPointAfter == "black"){
                            action_number = 9;
                        }
                        AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                  /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
                if(ColonPointBefore =="data_size:"){
                    log_message(DEBUG_MSG, "data_size:       \t\t" + ColonPointAfter);
                    bitWrite(status_update_code, 2, 1);
                    file_size = ColonPointAfter.toInt();
                    AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                     /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
                if(ColonPointBefore =="name:"){
                    log_message(DEBUG_MSG, "name:            \t\t" + ColonPointAfter);
                        if (ColonPointAfter != NULL && ColonPointAfter != "" && ColonPointAfter.length() <= 32){
                            image_name = ColonPointAfter;
                            bitWrite(status_update_code, 3, 1);
                        }else{
                            update_status_code(2404);
                            return false;
                        }  
                        AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                         /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
                if(ColonPointBefore =="type:"){ 
                    log_message(DEBUG_MSG, "type:\t\t" + ColonPointAfter);
                        if (ColonPointAfter == "bw" | ColonPointAfter == "rw" | ColonPointAfter == "br"){
                            image_type = ColonPointAfter;
                            bitWrite(status_update_code, 4, 1);
                        }else{
                            log_message(DEBUG_ERROR, "Wrong data format!");
                            update_status_code(2403);
                            return false;
                        }  
                        AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                       /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
                if(ColonPointBefore =="duration:"){
                    log_message(DEBUG_MSG, "duration:        \t\t" + ColonPointAfter);
                    image_duration += ColonPointAfter;
                    bitWrite(status_update_code, 5, 1);
                    AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                     /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
                if(ColonPointBefore =="blk:"){
                    image_data_color += "blk";
                    bitWrite(status_update_code, 6, 1);
                     Serial.println(ColonPointAfter);
                   // log_message(DEBUG_MSG, "blk >>>>>>>>> " + Cnumber11);
                    AppendSection.remove(0,5); 
                    /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
                if(ColonPointBefore =="param:"){
                    bitWrite(status_update_code, 7, 1);
                    rotate_param += ColonPointAfter;
                    AppendSection.remove(0,String(ImageDataArray[k]).length()+1);
                     /* mpm1.remove(0);
                    Cnumber11.remove(0);
                    sub11.remove(0);*/
                }
           // payloads.remove(0,String(kArray[k]).length()+1);
            ReservedPortion.remove(0);
            ColonPointBefore.remove(0);
            ColonPointAfter.remove(0);
        }
       // Serial.println(payloads);
         int pos2 = AppendSection.indexOf("|");
            String OnyImageData = AppendSection.substring(0,pos2+1);
            String END = AppendSection.substring(pos2+1);
            AppendSection.remove(OnyImageData.length(),END.length());
            ImageDataArray[0] = "0";
           // String(kArray[300]).remove(0);
            log_message(DEBUG_MSG, END);
            END.remove(0); 
            OnyImageData.remove(0);

    bitWrite(status_update_code, 15, 1);

    log_message(DEBUG_MSG, "status_update_code:\t\t" + String(status_update_code));
    timerWrite(timer_w, 0); //reset timer (feed watchdog)*/
}
bool action(){
        switch (action_number){  
            case 1: /// none  ///
            log_message(DEBUG_MSG, "NONE");
                if (status_update_code == 0b1000000000000011){
                    update_status_code(1); // 1 = Action event "none"
                    break;
                }else{
                    update_status_code(2411);
                    return false;
                }
            case 2: /// change  ///
            //Serial.println(AppendSection);
            AppendSection.remove(0);
            log_message(DEBUG_MSG, "CHANGE");
                if (status_update_code == 0b1000000000001111){
                        if (image_name != NULL && image_name != ""){//
                            log_message(DEBUG_ESP, "Image changing: " + image_name);
                            image_name.trim();
                                if (!action_event_image_change(current_event, image_name)){
                                    return false;
                                }

                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, image_name)){
                                    return false;
                                }
                        }
                    write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"); // Image rotation off
                    update_status_code(2); // 2 = Action event "change" is Success
                }else{
                    update_status_code(2411);
                    return false;
                }
            break;
            case 3: /// donload  ///
            log_message(DEBUG_MSG, "DOWNLOAD");
                if (status_update_code == 0b1000000001111111){
                        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_LAST, image_name)){
                            return false;
                        }
                        if (image_name == "default.jpg"){
                            image_path.remove(0);
                            image_path += DEF_IMG_PATH;
                            image_path += "/";
                            image_path += current_event;
                            image_name_s += image_name;
                        }else{
                            image_path += IMG_PATH;
                            image_path += "/";
                            image_path += current_event;
                            int image_count = count_files(SPIFFS, image_path);
                            log_message(DEBUG_MSG, "Image count = " + (String)image_count);
                            image_count++;
                            image_name_s += number_to_string(image_count);
                            image_name_s += image_name;
                        }
                    log_message(DEBUG_MSG, "Image name: " + image_path + "/" + image_name_s);
                    image_meta_data += "type:";
                    image_meta_data += image_type;
                    image_meta_data += "|duration:";
                    image_meta_data += image_duration;
                    image_meta_data += "|";
                    image_meta_data += image_data_color;
                    image_meta_data += ":";
      
                    uint16_t received_file_size = image_meta_data.length();
                    received_file_size = received_file_size + AppendSection.length();
                        if (received_file_size != file_size){
                            update_status_code(2406);
                        }
                    log_message(DEBUG_MSG, "received_file_size" + String(received_file_size));
                        if (!write_file(SPIFFS, image_path, image_name_s, image_meta_data)){
                            log_message(DEBUG_ERROR, "Fail to write file:" + image_name_s);
                            return false;
                        }
                       // Serial.println(Cnumber3);
                        if (!append_file(SPIFFS, image_path, image_name_s)){
                            log_message(DEBUG_ERROR, "Fail to append file:" + image_name_s);
                        }
                        AppendSection.remove(0);
                        if (!validate_file_size(SPIFFS, image_path, image_name_s, file_size, true)){
                            log_message(DEBUG_ERROR, "File size doesn't match");
                            update_status_code(2405);
                            // delete_file(SPIFFS, image_path, image_name_s);
                            return false;
                        }
                    update_status_code(3); // 3 = Action event "Download" is Success
                }else{
                    update_status_code(2411);
                    return false;
                }
            break;
            case 4: /// delete   ///
            log_message(DEBUG_MSG, "DELETE");
                if (status_update_code == 0b1000000000001011){
                        if (image_name != NULL && image_name != ""){
                            log_message(DEBUG_ESP, "Image name: " + image_name);
                            log_message(DEBUG_ESP, "Image data deleting");
                                if (!delete_file(SPIFFS, IMG_PATH, image_name)){
                                    return false;
                                }
                        }
                    update_status_code(4); // 4 = Action event "Delete" is Success
                }else{
                    update_status_code(2411);
                }
            break;
            case 5: /// format   ///
            log_message(DEBUG_MSG, "FORMAT");
                if (status_update_code == 0b1000000000000111){   
                    // timerWrite(timer_w, 0); //reset timer (feed watchdog)
                        if (current_event == "0"){
                                if (!write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0")){ // Image rotation off
                                    return false;
                                }
                            log_message(DEBUG_MSG, "SPIFFS Formatting.. Please wait...");
                                if (!format_spiffs()){
                                    timerWrite(timer_w, 0); //reset timer (feed watchdog)
                                    return false;
                                }
                        }else{
                            image_path += IMG_PATH;
                            image_path += "/";
                            image_path += current_event;
                                /* if (last_rotate_event == current_event){
                                        if (!write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0")){ // Image rotation off
                                            return false;
                                        }
                                }*/
                                if (!delete_dir(SPIFFS, image_path)){
                                    log_message(DEBUG_ERROR, "Failed to delete /images");
                                    return false;
                                }
                        }
                    update_status_code(5); // 5 = Action event "Format" is Success
                }else{
                    update_status_code(2411);
                    return false;
                }
            break;
            case 6: /// rotate   ///
            log_message(DEBUG_MSG, "ROTATE");
                if (status_update_code == 0b1000000010000111){
                        if (rotate_param == "1"){
                            String last_image_number_str((char *)0);
                            uint8_t last_image_number = 0;
                            last_image_number_str += read_value_from_eeprom(EEPROM_MEM_IMG_NUMBER);
                            last_image_number = last_image_number_str.toInt();
                                if (last_image_number < 1 || last_image_number > 100){
                                    last_image_number = 1;
                                }
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, current_event)){
                                    return false;
                                }
                                if (!write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, String(last_image_number))){ // Image rotation will start from image 001
                                    return false;
                                }
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_ROTATE)){
                                    return false;
                                }
                            log_message(DEBUG_MSG, "Rotation mode turned on");
                        }
                        else if (rotate_param == "0"){
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, current_event)){
                                    return false;
                                }
                                if (!write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0")){ // Image rotation will start from image 001
                                    return false;
                                }
                                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_ROTATE)){
                                    return false;
                                }
                            log_message(DEBUG_MSG, "Rotation mode turned off");
                        }else{
                            log_message(DEBUG_ERROR, "Wrong data format!");
                            update_status_code(2403);
                            return false;
                        }
                    update_status_code(6); // 6 = Action event "Rotation" is Success
                }else{
                    update_status_code(2411);
                }
            break;
            case 7: /// file count  ///
            log_message(DEBUG_MSG, "FILE_COUNT");
                if (status_update_code == 0b1000000000000111){
                        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_FILE_COUNT)){
                            return false;
                        }
                    update_status_code(7); // 7 = Action event "File-list" is Success
                    break;
                }else{
                    update_status_code(2411);
                }
            break;
            case 8: /// display-white  ///
            log_message(DEBUG_MSG, "DISPLAY > WHITE");
                if (status_update_code == 0b1000000000000111){
                    write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"); // Image rotation off
                        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_WHITE)){
                            return false;
                        }
                        if (!display_image("white")){
                            return false;
                        }
                        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "white")){
                            return false;
                        }
                    update_status_code(8); // 7 = Action event "File-list" is Success
                }else{
                    update_status_code(2411);
                }
            break;
            case 9: /// display-black  ///
            log_message(DEBUG_MSG, "DISPLAY > BLACK");
                if (status_update_code == 0b1000000000000111){
                    write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"); // Image rotation off
                        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_BLACK)){
                            return false;
                        }
                        if (!display_image("black")){
                            return false;
                        }
                        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "black")){
                            return false;
                        }
                    update_status_code(9); // 7 = Action event "File-list" is Success
                }else{
                    update_status_code(2411);
                }
            break;
            default:
            log_message(DEBUG_MSG, "default");
            update_status_code(2401);
            return 0;
        }
}
