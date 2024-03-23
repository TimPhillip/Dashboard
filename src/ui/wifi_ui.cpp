#include "wifi_ui.h"


void btn_test(lv_event_t* e){
    
    WifiUI* ui = (WifiUI*)lv_event_get_user_data(e);
    ui->handleConnectionRequest(ui->get_ssid(), ui->get_pwd());

    Serial.println(ui->get_ssid());
    Serial.println(ui->get_pwd());
}

void WifiUI::setOnConnectionRequestCallback(ConnectionRequestCallback cb){
    this->_connection_callback = cb;
}

void WifiUI::handleConnectionRequest(const char* ssid, const char* pwd){
    if(this->_connection_callback){
        this->_connection_callback(ssid, pwd);
    }
}

WifiUI::WifiUI(){

}


void WifiUI::init(){

    this->container = lv_obj_create(NULL);
    //lv_obj_set_flex_flow(this->container, LV_FLEX_FLOW_ROW);

    this->ssid_drop_down = lv_dropdown_create(this->container);
    lv_obj_set_pos(this->ssid_drop_down, 10, 30);
    lv_obj_set_size(this->ssid_drop_down, 300, 45);


    this->pwd_field = lv_textarea_create(this->container);
    lv_obj_set_pos(this->pwd_field, 320, 30);
    lv_obj_set_size(this->pwd_field, 300, 45);

    this->connect_btn = lv_btn_create(this->container);
    lv_obj_set_pos(this->connect_btn, 630, 30);
    lv_obj_set_size(this->connect_btn, 120, 45);
    lv_obj_add_event_cb(this->connect_btn, btn_test, LV_EVENT_CLICKED, this);

    this->connect_btn_label = lv_label_create(this->connect_btn);
    lv_obj_center(this->connect_btn_label);
    lv_label_set_text(this->connect_btn_label, "Connect");

    this->keyboard = lv_keyboard_create(this->container);
    lv_keyboard_set_textarea(this->keyboard, this->pwd_field);

    this->set_scanning_mode(true);
}

void WifiUI::set_scanning_mode(bool enabled){

    this->scanning_mode_enabled = enabled;

    if (enabled){
        lv_dropdown_set_options(this->ssid_drop_down, "Scanning Wifi Networks ...");
    }else{

    }

}

void WifiUI::set_wifi_options(const char* options){
    lv_dropdown_set_options(this->ssid_drop_down, options);
}

lv_obj_t* WifiUI::get_screen(){
    return this->container;
}

const char* WifiUI::get_ssid(){
    static char buf[256];
    lv_dropdown_get_selected_str(this->ssid_drop_down, buf, 256);
    return buf;
}

const char* WifiUI::get_pwd(){
    return lv_textarea_get_text(this->pwd_field);
}