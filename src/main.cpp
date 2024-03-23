#define _TASK_OO_CALLBACKS

#include <Arduino.h>
#include <lv_conf.h>
#include <lvgl.h>
#include <TaskScheduler.h>

#include "gui/gui.h"
#include "ui/ui.h"
#include "ui/wifi_ui.h"
#include "tasks/wifi_task.h"

#include <WiFi.h>
#include <HTTPClient.h>

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"


//const char* uri = "home.timphs.de/api/media_player_proxy/media_player.living_sonos?token=67b12ad525f8b55c6bfdb28758d995aa404128c8c1cb5b0df093c154506188a3&cache=8a9f1abaf36865de";
const char* uri = "https://icon2.cleanpng.com/20180720/ace/kisspng-vfb-stuttgart-201718-bundesliga-football-encaps-milton-keynes-dons-fc-5b5267f38548f9.233100681532127219546.jpg";

MyUI ui;
WifiUI wifi_ui;
Scheduler scheduler;

void setup_wifi();
void wait_for_wifi();
void wifi_connected();
void init_sd_card();

WiFiTask wifi_task(&scheduler);
//Task logging_task(0, TASK_ONCE, setup_wifi, &scheduler);

void setup_wifi(){

  WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks();

  String wifi = "";
  for (int i =0; i <n; i++){

    wifi += WiFi.SSID(i);
    //wifi += " (";
    //wifi += WiFi.RSSI(i);
    wifi += "\n";
  }

  wifi_ui.set_wifi_options(wifi.c_str());

  WiFi.begin("Connecto Patronum", "12121212");
  //ui.append_text("- Try to connect to WiFi ...\n");

  //logging_task.setCallback(&wait_for_wifi);
}

void wait_for_wifi(){

  //ui.append_text("- ...\n");

  if (WiFi.status() == WL_CONNECTED){
    //logging_task.setCallback(&wifi_connected);
  }
  else{
    //logging_task.setCallback(&wait_for_wifi);
    //logging_task.enableDelayed(1000);
  }
  
}

void wifi_connected(){
  //ui.append_text("- WiFi connected.\n");
  //ui.append_text("- ");
  //ui.append_text(WiFi.localIP().toString().c_str());
  //ui.append_text("\n");


  


  //logging_task.setCallback(&init_sd_card);
}

static sdmmc_card_t* sdcard;

void init_sd_card(){

  ui.append_text("- Initializing the SD card ...\n");

  sdspi_device_config_t device_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  device_config.host_id = SPI2_HOST;
  device_config.gpio_cs = GPIO_NUM_10;  

    
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = device_config.host_id;

  esp_err_t ret;

  spi_bus_config_t bus_cfg = {
    .mosi_io_num = GPIO_NUM_11,
    .miso_io_num = GPIO_NUM_13,
    .sclk_io_num = GPIO_NUM_12,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4000,
  };

  //ui.append_text("- Initializinbg SPI Bus ...\n");
  ret = spi_bus_initialize(device_config.host_id, &bus_cfg,SDSPI_DEFAULT_DMA);

  
  if (ret != ESP_OK){
    ui.append_text("- SPI Failed.\n");
  }else{
    ui.append_text("- SPI Done :)\n");
  }
  

  esp_vfs_fat_mount_config_t mount_config = 
    {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

  ui.append_text("- Mounting SD card ...\n");
  ret = esp_vfs_fat_sdspi_mount("/sd", &host, &device_config, &mount_config, &sdcard);

  
  if (ret != ESP_OK){
    if (ret == ESP_FAIL){
      ui.append_text("- Mounting Failed\n");
    }else{
      ui.append_text("- SD Init Failed\n");
    }
    
  }else{
    ui.append_text("- Mounting Done\n");
  }
  

  const char* test_file = "/sd/info.txt";
  char data[256];
  snprintf(data, 256, "%s %s!\n", "Hello", sdcard->cid.name);
  
  FILE* f = fopen(test_file, "w");
  if (f == NULL){
    //ui.append_text("- Failed to open the file.\n");
  }
  fprintf(f, data);
  fclose(f);

  ui.append_text("- File saved.\n");

  f = fopen(test_file, "r");
  if (f == NULL){
    ui.append_text("- Failed to open the file.\n");
  }
  
  char line[256];
  fgets(line, sizeof(line), f);
  fclose(f);

  ui.append_text(line);

  // strip newline
  char *pos = strchr(line, '\n');
  ui.append_text(pos);
  
  if (pos) {
      *pos = '\0';
  }

  

  fclose(f);

  //logging_task.disable();

}


void on_scan_completed(const char* options){
  wifi_ui.set_wifi_options(options);
}

void on_connection_request(const char* ssid, const char* pwd){
  wifi_task.connect(ssid, pwd);
}

void on_connected_to_wifi(){
  Serial.println("Connected!");
  lv_scr_load(ui.getScreen());

  init_sd_card();

  return;
  
  HTTPClient client;
  client.begin(uri);

  int code = client.GET();

  if (code > 0){
    ui.append_text("- HTTP OK\n");
    ui.append_text(String(client.getString().length()).c_str());

    const uint8_t* img_data = (uint8_t*)client.getString().c_str();
    lv_img_dsc_t img;
    img.data = img_data;
    img.header.always_zero = 0;


    //ui.set_image(img_data);

  }else{
    //ui.append_text("- HTTP failed\n");
  }
}

void setup() {
  gui_start();
  ui.init();
  wifi_ui.init();
  lv_scr_load(wifi_ui.get_screen());

  Serial.begin(115200);
  //logging_task.enable();
  
  wifi_task.setOnScanCompletedCallback(on_scan_completed);
  wifi_task.setOnConnectedCallback(on_connected_to_wifi);
  wifi_ui.setOnConnectionRequestCallback(on_connection_request);
  wifi_task.scan_networks();
  wifi_task.enable();
}

void loop() {
  lv_timer_handler();

  scheduler.execute();
}

