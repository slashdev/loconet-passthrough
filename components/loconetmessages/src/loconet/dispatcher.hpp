#ifndef LOCONET_DISPATCHER_HPP_
#define LOCONET_DISPATCHER_HPP_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "message/message.hpp"
#include "dispatchhandler.hpp"



// Normally, set the delay to 250 MS
#ifndef LOCONET_DISPATCHER_TASK_DELAY
#define LOCONET_DISPATCHER_TASK_DELAY pdMS_TO_TICKS( 250 );
#endif

#ifndef LOCONET_DISPATCHER_QUEUE_SIZE
#define LOCONET_DISPATCHER_QUEUE_SIZE 15
#endif

namespace loconet
{
	namespace Dispatcher
	{
		void add(DispatchHandler*);
		void remove(DispatchHandler*);

		BaseType_t enqueue(message::Message*);

		void task(void*);
	}
}


#endif
