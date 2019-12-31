#include "messages.hpp"

// Required for malloc
#include <string>

namespace loconet
{
	namespace message
	{

		Message* parse(uint8_t *data, size_t length)
		{
			if (length < 2)
			{
				return NULL;
			}

			Message* msg = NULL;

			switch(data[0])
			{
				case BUSY:
					msg = new Busy();
					break;
				case INPUT_REP:
					if (length == 4)
					{
						msg = new InputReport(data[1], data[2]);
					}
					break;
				default:
					msg = NULL;
					break;
			}
			if (msg != NULL)
			{
				// Check the checksums
				if (data[length-1] == msg->calculate_checksum())
				{
					return msg;
				}
				// We do not need msg
				delete msg;
			}
			return NULL;
		}

	}
}
