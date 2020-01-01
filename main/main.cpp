#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "event_handlers.hpp"

#include "wifiswitcher.hpp"

#include "httpd/server.hpp"

#include "wifisettings.h"


#ifndef DEFAULT_WIFI_SSID
#define DEFAULT_WIFI_SSID "defaultSSID"
#endif

#ifndef DEFAULT_WIFI_PASSWORD
#define DEFAULT_WIFI_PASSWORD "defaultPassword"
#endif



size_t before = 0;
communication::httpd::Server* webserver = new communication::httpd::Server();

extern "C" {

    static esp_err_t handle_event(void *ctx, system_event_t *event)
    {
        if (event->event_id == SYSTEM_EVENT_STA_GOT_IP || event->event_id == SYSTEM_EVENT_AP_START)
        {
            webserver->start();
        }
        return EventHandlers::handle_event(ctx, event);
    }

    void webserver_task(void* args)
    {
        TickType_t xDelay = pdMS_TO_TICKS( 500 );
        for(;;)
        {
            webserver->thread();
            vTaskDelay(xDelay);
        }

    }

    void memory_usage(void * pvParameter)
    {
        TickType_t xDelay = pdMS_TO_TICKS( 1000 );
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

        ESP_ERROR_CHECK(esp_event_loop_init(handle_event, NULL) );
        tcpip_adapter_init();

        WifiSwitcher::ap()->set_ssid("sd_loconet");
        WifiSwitcher::ap()->set_password("Aa123456");

        WifiSwitcher::station()->set_ssid(DEFAULT_WIFI_SSID);
        WifiSwitcher::station()->set_password(DEFAULT_WIFI_PASSWORD);

        WifiSwitcher::start();

        // Create a task for the webserver
        xTaskCreate(webserver_task, "Web Server", 8000, NULL, 2, NULL);
        xTaskCreate(memory_usage, "Memory printer", 4000, NULL, 2, NULL);
    }
}
