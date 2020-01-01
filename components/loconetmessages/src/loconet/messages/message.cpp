#include "message.hpp"

namespace loconet
{
	namespace messages
	{
		Message::Message(opcode_t opc, size_t length)
		{
			opcode_ = opc;
			length_ = length;
			data_ = (uint8_t*) calloc(length_, sizeof(uint8_t));
			data_[0] = opc;
		}

		Message::~Message()
		{
			free(data_);
		}

		opcode_t Message::opcode()
		{
			return opcode_;
		}

		size_t Message::length()
		{
			return length_;
		}

		uint8_t* Message::bytes()
		{
			// Ensure that there is a checksum, hence, try to return it
			// (and we don't care about the value, just that is there)
			checksum();
			return data_;
		}

		uint8_t Message::checksum()
		{
			if(data_[length_ - 1] == 0)
			{
				data_[length_ -1 ] = calculate_checksum();
			}

			return data_[length_ -1];
		}

		uint8_t Message::calculate_checksum()
		{
			// calculate the checksum: XOR each byte with checksum
			uint8_t checksum = 0xFF;
			for(size_t i = 0 ; i < length_ - 1 ; i++)
			{
				checksum ^= data_[i];
			}

			return checksum;
		}

	}
}
