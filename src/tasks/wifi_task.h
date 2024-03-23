#pragma once

#define _TASK_OO_CALLBACKS
#include <list>

#include <TaskSchedulerDeclarations.h>
#include <WiFi.h>


using OnScanCallback = std::function<void(const char*)>;
using OnConnectedCallback = std::function<void()>;

class WiFiTask: public Task {

    public:
        WiFiTask(Scheduler* scheduler);
        void connect(const char* ssid, const char* pwd);
        void scan_networks();

        bool Callback();
        
        void setOnScanCompletedCallback(OnScanCallback cb);
        void setOnConnectedCallback(OnConnectedCallback cb);
    
    private:

        bool _is_scanning;
        bool _is_connecting;

        std::list<char*> _scanned_ssids;
        OnScanCallback _on_scan_callback;
        OnConnectedCallback _on_connected_callback;

};