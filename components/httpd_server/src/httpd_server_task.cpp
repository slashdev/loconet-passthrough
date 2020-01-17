#include "httpd_server_task.hpp"
#include "esp_event.h"

// Stack size used for the Webserver handling task
uint32_t server_stack_size = 16000;

// Priority of the webserver task
UBaseType_t server_priority = tskIDLE_PRIORITY + 2;

static httpd::Server *server = new httpd::Server();

void httpd_server_add_handler(httpd::RequestHandler *handler)
{
  server->add(handler);
}

void httpd_server_remove_handler(httpd::RequestHandler *handler)
{
  server->remove(handler);
}

void httpd_server_task(void* args)
{
  for(;;)
  {
    server->thread();
  }
}

TaskHandle_t handle = NULL;

void httpd_server_start()
{
  server->start();
  if (handle == NULL)
  {
    xTaskCreate(httpd_server_task,
      "Web Server - session handling",
      server_stack_size, NULL, server_priority,
      &handle );
  }
}

void httpd_server_stop()
{
  if (handle != NULL)
  {
    vTaskDelete(handle);
    handle = NULL;
  }
  server->stop();
}

extern "C" {
  void handle_event(void *args, esp_event_base_t base, int32_t event, void *data)
  {
    if (    (base == IP_EVENT && event == IP_EVENT_STA_GOT_IP)
         || (base == WIFI_EVENT && event == WIFI_EVENT_AP_START) )
    {
      httpd_server_start();
    }

    if ( (base == WIFI_EVENT) && ( event == WIFI_EVENT_STA_DISCONNECTED || event == WIFI_EVENT_AP_STOP ) )
    {
      httpd_server_stop();
    }
  }

  void httpd_server_initialize(uint32_t stacksize, UBaseType_t priority)
  {
    server_stack_size = stacksize;
    server_priority = priority;

    // Register the events to start and stop the server
    ESP_ERROR_CHECK(esp_event_handler_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, handle_event, NULL
    ));
    ESP_ERROR_CHECK(esp_event_handler_register(
      IP_EVENT, ESP_EVENT_ANY_ID, handle_event, NULL
    ));
  }
}
