#include <Arduino.h>
#include <lv_conf.h>
#include <lvgl.h>
#include <TaskScheduler.h>

#include "gui/gui.h"
#include "ui/ui.h"
#include <WiFi.h>
#include <HTTPClient.h>


//const char* uri = "home.timphs.de/api/media_player_proxy/media_player.living_sonos?token=67b12ad525f8b55c6bfdb28758d995aa404128c8c1cb5b0df093c154506188a3&cache=8a9f1abaf36865de";
const char* uri = "https://icon2.cleanpng.com/20180720/ace/kisspng-vfb-stuttgart-201718-bundesliga-football-encaps-milton-keynes-dons-fc-5b5267f38548f9.233100681532127219546.jpg";

MyUI ui;
Scheduler scheduler;

void setup_wifi();
void wait_for_wifi();
void wifi_connected();

Task logging_task(0, TASK_FOREVER, setup_wifi, &scheduler);

void setup_wifi(){

  WiFi.begin("Connecto Patronum", "12121212");
  ui.append_text("- Try to connect to WiFi ...\n");

  logging_task.setCallback(&wait_for_wifi);
}

void wait_for_wifi(){

  ui.append_text("- ...\n");

  if (WiFi.status() == WL_CONNECTED){
    logging_task.setCallback(&wifi_connected);
  }
  else{
    logging_task.setCallback(&wait_for_wifi);
    logging_task.enableDelayed(1000);
  }
  
}

void wifi_connected(){
  ui.append_text("- WiFi connected.\n");
  ui.append_text("- ");
  ui.append_text(WiFi.localIP().toString().c_str());
  ui.append_text("\n");


  HTTPClient client;
  client.begin(uri);

  int code = client.GET();

  if (code > 0){
    ui.append_text("- HTTP OK\n");
    //ui.append_text(String(client.getString().length()).c_str());

    const uint8_t* img_data = (uint8_t*)client.getString().c_str();
    lv_img_dsc_t img;
    img.data = img_data;
    img.header.always_zero = 0;


    ui.set_image(img_data);

  }else{
    ui.append_text("- HTTP failed\n");
  }


  logging_task.disable();
}


void setup() {
  gui_start();
  ui.init();

  logging_task.enable();
}

void loop() {
  lv_timer_handler();

  scheduler.execute();
}

