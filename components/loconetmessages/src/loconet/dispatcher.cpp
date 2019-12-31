#include "dispatcher.hpp"

#include "freertos/queue.h"

#include "message/messages.hpp"
#include <list>
// for the memcpy
#include <cstring>


/*
     * Struct only used internally to represent a message
     */
    typedef struct {
      uint8_t *data;
      size_t size;
    } loconet_dispatcher_message_t;


namespace loconet
{
	namespace Dispatcher
	{

    QueueHandle_t xQueue = xQueueCreate(LOCONET_DISPATCHER_QUEUE_SIZE, sizeof( loconet_dispatcher_message_t * ) );

		std::list<DispatchHandler*> handlers;

		void add(DispatchHandler* handler)
		{
			handlers.push_back(handler);
		}

		void remove(DispatchHandler* handler)
		{
			handlers.remove(handler);
		}

		BaseType_t enqueue(message::Message* msg)
		{
			loconet_dispatcher_message_t *p = (loconet_dispatcher_message_t*) malloc (sizeof(loconet_dispatcher_message_t));
			p->data = (uint8_t*) malloc(sizeof(uint8_t) * msg->length());
      p->size = msg->length();

      std::memcpy(p->data, msg->bytes(), sizeof(uint8_t) * p->size);

			return xQueueSendToBack(xQueue, (void *) &p, 0);
		}

		void task(void* pvParameter)
		{
			TickType_t xDelay = LOCONET_DISPATCHER_TASK_DELAY;
			BaseType_t xStatus;

			for(;;)
			{
				loconet_dispatcher_message_t *msg;
				xStatus = xQueueReceive( xQueue, &msg, 0 );
	      if (xStatus == pdPASS)
	      {
		     	for(auto &h : handlers)
					{
						// Give each handler its own copy
						uint8_t* data = (uint8_t*) malloc(sizeof(uint8_t) * msg->size);
        		std::memcpy(data, msg->data, sizeof(uint8_t) * msg->size);
						h->handle_message(data, msg->size);
						free(data);
					}
	        // Free the memory of msg
	        free(msg->data);
	        free(msg);
	      }
	      vTaskDelay(xDelay);
	    }
		}


	}
}
