#ifndef LOCONET_MESSAGE_BUSY_HPP_
#define LOCONET_MESSAGE_BUSY_HPP_

#include "message.hpp"


namespace loconet
{
	namespace messages
	{
		class Busy : public Message
		{
		public:
			Busy();
		};
	}
}


#endif


