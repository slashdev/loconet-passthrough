#ifndef LOCONET_RECEIVER_H_
#define LOCONET_RECEIVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "inboundhandler.hpp"

// Normally, set the delay to 250 MS
#ifndef LOCONET_RECEIVER_TASK_DELAY
#define LOCONET_RECEIVER_TASK_DELAY pdMS_TO_TICKS( 250 );
#endif

#ifndef LOCONET_RECEIVER_QUEUE_SIZE
#define LOCONET_RECEIVER_QUEUE_SIZE 15
#endif


namespace loconet
{
	namespace Receiver
	{
		void add(InboundHandler*);
		void remove(InboundHandler*);

		BaseType_t enqueue(uint8_t *, size_t);

		void task(void*);
	}
}


#endif
