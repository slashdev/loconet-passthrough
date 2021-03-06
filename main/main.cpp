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

#include "wifisettings.h"

#include "httpd_server_task.hpp"
#include "wificonfigurator/handler.hpp"

extern "C" {

    void app_main()
    {
        esp_err_t ret = nvs_flash_init();
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

        httpd_server_initialize(16000, 2);
        httpd_server_add_handler(new wificonfigurator::Handler());
    }
}
