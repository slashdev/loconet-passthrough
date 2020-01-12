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
#include "wifihandler/handler.hpp"

#include "wifisettings.h"

#ifndef DEFAULT_WIFI_SSID
#define DEFAULT_WIFI_SSID "defaultSSID"
#endif

#ifndef DEFAULT_WIFI_PASSWORD
#define DEFAULT_WIFI_PASSWORD "defaultPassword"
#endif

size_t before = 0;



static httpd::Server *webserver = new httpd::Server();
static httpd::handlers::wifi::Handler *wifiHandler = new httpd::handlers::wifi::Handler(webserver);

void webserver_task(void* args)
{
  for(;;)
  {
    webserver->thread();
  }
}

TaskHandle_t webserverTaskHandler = NULL;

void webserver_start()
{
  webserver->start();
  if (webserverTaskHandler == NULL)
  {
    xTaskCreate(webserver_task, "Web Server - session handling", 16000, NULL, 2, &webserverTaskHandler );
  }
}

void webserver_stop()
{
  if (webserverTaskHandler != NULL)
  {
    vTaskDelete(webserverTaskHandler);
    webserverTaskHandler = NULL;
  }
  webserver->stop();
}


extern "C" {

  void handle_main_event(void *args, esp_event_base_t base, int32_t event, void *data)
  {
    if (    (base == IP_EVENT && event == IP_EVENT_STA_GOT_IP)
         || (base == WIFI_EVENT && event == WIFI_EVENT_AP_START) )
    {
      webserver_start();
    }

    if ( (base == WIFI_EVENT) && ( event == WIFI_EVENT_STA_DISCONNECTED || event == WIFI_EVENT_AP_STOP ) )
    {
      webserver_stop();
    }
  }

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

    EventHandlers::init();

    ESP_ERROR_CHECK(esp_event_handler_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, handle_main_event, NULL
    ));
    ESP_ERROR_CHECK(esp_event_handler_register(
      IP_EVENT, ESP_EVENT_ANY_ID, handle_main_event, NULL
    ));

    tcpip_adapter_init();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    WifiConnector::ap()->ssid("sd_loconet");
    WifiConnector::ap()->password("Aa123456");

    WifiConnector::station()->ssid(DEFAULT_WIFI_SSID);
    WifiConnector::station()->password(DEFAULT_WIFI_PASSWORD);

    WifiConnector::start();

    // Create a task for the webserver
    // And directly suspend it
    // vTaskSuspend(webserverTaskHandler);
    // xTaskCreate(webserver_task, "Web Server - session handling", 16000, NULL, 2, NULL);
    // xTaskCreate(webserver_session_processing_task, "Web Server - session handling", 16000, NULL, 2, NULL);
    // xTaskCreate(webserver_accept_connections_task, "Web Server - accept connections", 8000, NULL, 2, NULL);
    xTaskCreate(memory_usage, "Memory printer", 4000, NULL, 2, NULL);
  }
}
