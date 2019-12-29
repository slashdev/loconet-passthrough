#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "genericeventloop.hpp"

#include "wifiswitcher.hpp"

extern "C" {

    static esp_err_t event_handler(void *ctx, system_event_t *event)
    {
        return EventLoop::get_instance()->handle(ctx, event);
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

        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );
        tcpip_adapter_init();

        WifiSwitcher::ap()->set_ssid("sd_loconet");
        WifiSwitcher::ap()->set_password("Aa123456");

        WifiSwitcher::station()->set_ssid("defaultSsid");
        WifiSwitcher::station()->set_password("defaultPassword");

        WifiSwitcher::start();
    }
}