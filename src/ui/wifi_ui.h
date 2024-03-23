#pragma once

#include <lvgl.h>
#include <Arduino.h>


using ConnectionRequestCallback = std::function<void(const char*, const char*)>;


class WifiUI{

    public:
        WifiUI();
        void init();
        void set_scanning_mode(bool enabled);
        void set_wifi_options(const char* options);
        lv_obj_t* get_screen();

        const char* get_ssid();
        const char* get_pwd();

        void setOnConnectionRequestCallback(ConnectionRequestCallback cb);
        void handleConnectionRequest(const char* ssid, const char* pwd);
        
    private:
        lv_obj_t* container;
        lv_obj_t* ssid_drop_down;
        lv_obj_t* pwd_field;
        lv_obj_t* keyboard;
        lv_obj_t* connect_btn;
        lv_obj_t* connect_btn_label;
        
        bool scanning_mode_enabled;
        ConnectionRequestCallback _connection_callback;


};