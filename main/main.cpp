#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "genericeventloop.hpp"

extern "C" {

    static esp_err_t event_handler(void *ctx, system_event_t *event)
    {
        return EventLoop::get_instance()->handle(ctx, event);
    }

    void app_main()
    {
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );
        tcpip_adapter_init();
    }
}