#include "parser.hpp"

#include "util.hpp"
// Required for malloc
#include <string>

namespace loconet
{
	namespace messages
	{
		Message* parse(uint8_t *data, size_t length)
		{
			if (length < 2)
			{
				return NULL;
			}
			if (calculate_checksum(data, length) != 0)
			{
				return NULL;
			}

			Message* msg = NULL;

			switch(data[0])
			{
				case OPCode::BUSY:
					msg = new Busy();
					break;
				case OPCode::INPUT_REP:
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
