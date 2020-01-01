#include "busy.hpp"

namespace loconet
{
	namespace messages
	{
		Busy::Busy() : Message(OPCode::BUSY, 2)
		{
		}
	}
}
