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

#include "udpserver.hpp"

UDPServer* udpServer = new UDPServer(6000);

extern "C" {

    void udpservertask(void* pvParameters )
    {
      udpServer->task(pvParameters);
    }


    static esp_err_t handle_event(void *ctx, system_event_t *event)
    {
        return EventHandlers::handle_event(ctx, event);
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

        WifiSwitcher::station()->set_ssid("defaultSsid");
        WifiSwitcher::station()->set_password("defaultPassword");

        WifiSwitcher::start();

        // Run the UDPServer task on priority 2
        xTaskCreate(udpservertask, "UDP Server on port 6000", 8000, NULL, 2, NULL);
    }
}
