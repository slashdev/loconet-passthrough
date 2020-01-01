#include "receiver.hpp"

#include "freertos/queue.h"

#include "messages/parser.hpp"
#include <list>

namespace loconet
{
	namespace Receiver
	{
		std::list<InboundHandler*> handlers;
		QueueHandle_t xQueue = xQueueCreate(LOCONET_RECEIVER_QUEUE_SIZE, sizeof( messages::Message * ) );

		void add(InboundHandler* handler)
		{
			handlers.push_back(handler);
		}

		void remove(InboundHandler* handler)
		{
			handlers.remove(handler);
		}


		BaseType_t enqueue(uint8_t *data, size_t length)
		{
			messages::Message* msg = loconet::messages::Parser::parse(data, length);

			if (msg != NULL)
			{
				return xQueueSendToBack(xQueue, (void *) &msg, 0);
			}
			// Not succeeded, so send a fail
			return pdFAIL;
		}


		void task(void* pvParameter)
		{
			TickType_t xDelay = LOCONET_RECEIVER_TASK_DELAY;
			BaseType_t xStatus;

			for(;;)
			{
				messages::Message *msg;
				xStatus = xQueueReceive( xQueue, &msg, 0 );
	      if (xStatus == pdPASS)
	      {
		     	for(auto &h : handlers)
					{
						h->handle_message(msg);
					}
	        // Free the memory of msg
	        delete msg;
	      }
	      vTaskDelay(xDelay);
	    }
		}

	}
}
