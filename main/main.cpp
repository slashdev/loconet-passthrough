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

extern "C" {

    /*
    static esp_err_t handle_event(void *ctx, system_event_t *event)
    {
        return EventHandlers::handle_event(ctx, event);
    }
    */

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
