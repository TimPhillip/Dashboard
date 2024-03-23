#include "wifi_task.h"


WiFiTask::WiFiTask(Scheduler* scheduler) : Task(TASK_SECOND, TASK_FOREVER, scheduler, false)
{
    //WiFi.mode(WIFI_STA);
}

bool WiFiTask::Callback(){

    Serial.println("Wait");

    if (this->_is_connecting){
        if (WiFi.status() != WL_CONNECTED){
            return true;
        }

        this->_is_connecting = false;
        if(this->_on_connected_callback){
            this->_on_connected_callback();
        }
        this->disable();
        return false;
    }

    if (this->_is_scanning){

        int n = WiFi.scanComplete();

        if (n < 0){
            return true;
        }

        this->_is_scanning = false;
        if (this->_on_scan_callback){

            String ssids = "";

            for (int i=0; i < n; i++){
                ssids += WiFi.SSID(i);
                ssids += "\n";
            }

            this->_on_scan_callback(ssids.c_str());
        }

        this->disable();
        return false;
    }
    
}

void WiFiTask::scan_networks(){
    WiFi.scanNetworks(true);
    this->_is_scanning = true;
    this->restart();
}

void WiFiTask::connect(const char* ssid, const char* pwd){
    WiFi.begin(ssid, pwd);
    this->_is_connecting = true;
    this->restart();
}

void WiFiTask::setOnConnectedCallback(OnConnectedCallback cb){
    this->_on_connected_callback = cb;
}

void WiFiTask::setOnScanCompletedCallback(OnScanCallback cb){
    this->_on_scan_callback = cb;
}