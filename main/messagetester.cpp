#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "loconet/messages.hpp"
#include "loconet/messages/parser.hpp"
#include "loconet/receiver.hpp"
#include "loconet/inboundhandler.hpp"
#include "loconet/dispatcher.hpp"
#include "loconet/dispatchhandler.hpp"

size_t before = 0;

class SimpleInboundHandler : public loconet::InboundHandler
{
public:
  SimpleInboundHandler()
  {
    loconet::Receiver::add(this);
  }

  void handle_message(loconet::messages::Message* msg)
  {
    ESP_LOGI("HANDLER", "I got message: OPCODE: %d, LENGTH: %d", msg->opcode(), msg->length());
    loconet::Dispatcher::enqueue(msg);
  }
};

class SimpleDispatchHandler : public loconet::DispatchHandler
{
public:
  SimpleDispatchHandler()
  {
    loconet::Dispatcher::add(this);
  }
  void handle_message(uint8_t* data, size_t length)
  {
    ESP_LOGI("DISPATCH", "I dispatch a message of %d bytes", length);
    loconet::messages::Message* msg = loconet::messages::Parser::parse(data, length);
    if (msg == NULL)
    {
      ESP_LOGW("DISPATCH", "Wrong message type!");
      for(size_t i = 0 ; i < length; i++)
      {
        ESP_LOGI("DISPATCH", "        data[%d] = %d", i, data[i]);
      }
    }
    else
    {
      ESP_LOGI("DISPATCH", "Message has OPCODE: %d", msg->opcode());
    }
    delete msg;
  }
};

SimpleInboundHandler* inboundhandler = new SimpleInboundHandler();
SimpleDispatchHandler* dispatchhandler = new SimpleDispatchHandler();

extern "C" {

	void task_receiver(void *pvParameter)
	{
		loconet::Receiver::task(pvParameter);
	}

  void task_dispatcher(void *pvParameter)
  {
    loconet::Dispatcher::task(pvParameter);
  }

	void task_sender(void *pvParameter)
	{
		TickType_t xDelay = pdMS_TO_TICKS( 5000 );
		uint8_t *msg = (uint8_t*) malloc(sizeof(uint8_t)*2);
		msg[0] = 0x81; // Busy
		msg[1] = 0x7E; // Checksum of Busy

		for(;;)
		{
			loconet::Receiver::enqueue(msg, 2);
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
		before = xPortGetFreeHeapSize();

		xTaskCreate(task_receiver, "Loconet receiver", 8000, NULL, 2, NULL);
    xTaskCreate(task_dispatcher, "Loconet dispatcher", 8000, NULL, 2, NULL);


		xTaskCreate(task_sender, "Simple message sender", 8000, NULL, 2, NULL);

		xTaskCreate(memory_usage, "Memory manager", 4000, NULL, 2, NULL);

	}


	void app_main_correct_memory_usage()
  {


  	TickType_t xDelay = pdMS_TO_TICKS( 1000 );
  	for(;;)
  	{
  		ESP_LOGI("MAIN", "LOOP");
  		size_t before = xPortGetFreeHeapSize();
  		ESP_LOGI("MAIN", "Memory: %d", before);
  		loconet::messages::Busy* busy = new loconet::messages::Busy();
  		ESP_LOGI("MAIN", "Size: %d", busy->length());
  		delete busy;
  		size_t after = xPortGetFreeHeapSize();
  		ESP_LOGI("MAIN", "Memory: %d", after);
  		if (before < after)
  		{
  			ESP_LOGW("MAIN", "Difference           : %d", after - before);
  		}
  		else
  		{
  			ESP_LOGI("MAIN", "Difference           : %d", after - before);
  		}
  		vTaskDelay(xDelay);
  	}
  }

  /*
   * Note that there is no DELETE on the loconet Message,
   * running this shows a memory leak of 24 bytes per cycle
   */
  void app_main_memory_leak()
  {


  	TickType_t xDelay = pdMS_TO_TICKS( 1000 );
  	for(;;)
  	{
  		ESP_LOGI("MAIN", "LOOP");
  		size_t before = xPortGetFreeHeapSize();
  		ESP_LOGI("MAIN", "Memory: %d", before);
  		loconet::messages::Busy* busy = new loconet::messages::Busy();
  		ESP_LOGI("MAIN", "Size: %d", busy->length());
  		size_t after = xPortGetFreeHeapSize();
  		ESP_LOGI("MAIN", "Memory: %d", after);
  		if (before < after)
  		{
  			ESP_LOGW("MAIN", "Difference           : %d", after - before);
  		}
  		else
  		{
  			ESP_LOGI("MAIN", "Difference           : %d", after - before);
  		}
  		vTaskDelay(xDelay);
  	}
  }
}
