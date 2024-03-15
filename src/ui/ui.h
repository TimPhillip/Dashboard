#pragma once

#include <lvgl.h>
#include <Arduino.h>

class MyUI {

    private:
        String console_content;
        lv_obj_t* console;
        lv_obj_t* image;

    public:
        MyUI();
        void init();
        void append_text(const char* text);
        void set_image(const uint8_t* source);
};