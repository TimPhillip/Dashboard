#include "ui.h"
#include "plant.c"

MyUI::MyUI(){

}

void MyUI::init(){
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    //lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Klick mich!");                     /*Set the labels text*/
    lv_obj_center(label);

    this->console = lv_label_create(lv_scr_act());
    lv_obj_set_pos(this->console, 10, 100);
    lv_label_set_long_mode(this->console, LV_LABEL_LONG_WRAP);
    
    this->console_content = String("Console:\n");
    lv_label_set_text(this->console, this->console_content.c_str());

    this->image = lv_img_create(lv_scr_act());
    lv_obj_set_pos(this->image, 200, 10);
    lv_obj_set_size(this->image, 300, 300);

    LV_IMG_DECLARE(plant);
    lv_img_set_src(this->image, &plant);
}

void MyUI::append_text(const char* text){

    this->console_content += String(text);
    lv_label_set_text(this->console, this->console_content.c_str());
}

void MyUI::set_image(const uint8_t* source){
    //lv_img_set_src(this->image, source);
}