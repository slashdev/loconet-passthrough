#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "event_handlers.hpp"

#include "wificonnector.hpp"
#include "wifisettings.h"

#include "httpd/server.hpp"

#include "wifirequesthandler.hpp"

#if __has_include("wifisettings.h")
#  include "wifisettings.h"
#endif


#ifndef DEFAULT_WIFI_SSID
#define DEFAULT_WIFI_SSID "defaultSSID"
#endif

#ifndef DEFAULT_WIFI_PASSWORD
#define DEFAULT_WIFI_PASSWORD "defaultPassword"
#endif



size_t before = 0;

TaskHandle_t webserverTaskHandler = NULL;

void webserver_task(void* args)
{
  httpd::Server* webserver = new httpd::Server();
  WifiRequestHandler* wifireqhandler = new WifiRequestHandler();
  webserver->add(wifireqhandler);

  webserver->start();

  for(;;)
  {
    webserver->thread();
  }
}

extern "C" {

<<<<<<< HEAD

  void memory_usage(void * pvParameter)
  {
    TickType_t xDelay = pdMS_TO_TICKS( 2000 );
    for(;;)
    {
      size_t now = xPortGetFreeHeapSize();
      ESP_LOGI("MAIN", "Memory: %d", now);
      ESP_LOGW("MAIN", "Difference:                  %d", now-before);
      before = now;

      vTaskDelay(xDelay);
    }
  }

  void app_main()
    {
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
        {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);

        ESP_ERROR_CHECK(esp_event_loop_create_default());
        tcpip_adapter_init();

        WifiConnector::ap()->ssid("sd_loconet");
        WifiConnector::ap()->password("Aa123456");

        WifiConnector::station()->ssid(DEFAULT_WIFI_SSID);
        WifiConnector::station()->password(DEFAULT_WIFI_PASSWORD);

        WifiConnector::start();
    }
}
