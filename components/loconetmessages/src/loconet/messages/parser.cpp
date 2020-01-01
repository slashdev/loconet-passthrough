#include "parser.hpp"

// Required for malloc
#include <string>

namespace loconet
{
	namespace messages
	{
  	bool valid_checksum(uint8_t *data, size_t length)
  	{
			// calculate the checksum: XOR each byte with checksum
			uint8_t checksum = 0xFF;
			for(size_t i = 0 ; i < length - 1 ; i++)
			{
				checksum ^= data[i];
			}
			return (checksum == data[length-1]);
  	}


		Message* parse(uint8_t *data, size_t length)
		{
			if (length < 2)
			{
				return NULL;
			}
			if (!valid_checksum(data, length))
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
			return msg;
		}
  }
}
