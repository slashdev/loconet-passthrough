#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "event_handlers.hpp"

#include "wifi_connector.hpp"

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

  for(;;)
  {
    webserver->thread();
  }
}

extern "C"
{

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

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    EventHandlers::init();

    tcpip_adapter_init();

    WifiConnector::ap()->ssid("sd_loconet");
    WifiConnector::ap()->password("Aa123456");

    WifiConnector::station()->ssid(DEFAULT_WIFI_SSID);
    WifiConnector::station()->password(DEFAULT_WIFI_PASSWORD);

    WifiConnector::start();

    // Create a task for the webserver
    xTaskCreate(webserver_session_processing_task, "Web Server - session handling", 8000, NULL, 2, NULL);
    xTaskCreate(webserver_accept_connections_task, "Web Server - accept connections", 8000, NULL, 2, NULL);
    xTaskCreate(memory_usage, "Memory printer", 4000, NULL, 2, NULL);
  }
}
