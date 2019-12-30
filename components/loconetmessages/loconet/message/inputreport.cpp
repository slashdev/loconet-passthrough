#include "inputreport.hpp"


namespace loconet
{
	namespace message
	{
		InputReport::InputReport(uint16_t addr, bool ishigh) : Message(INPUT_REP, 4)
		{
			address(addr);
			input(ishigh);
		}

		InputReport::InputReport(uint8_t byte1, uint8_t byte2) : Message(INPUT_REP, 4)
		{
			address(byte1, byte2);
			input(byte2);
		}

		InputReport::InputReport() : InputReport(0, false)
		{
		}

		uint16_t InputReport::address()
		{
			return address_;
		}

		void InputReport::address(uint16_t addr)
		{
			address_ = addr;

			// I is used as odd/even bit
  		uint8_t odd = addr & 0x01;

  		bool state = input();

  		// Subtract one and then bit shift right to divide by two
  		addr--;
  		addr >>= 1;

  		// Set low bits of the address
  		data_[1] = addr & 0x7F;
  		// Set high bits of the address, keep track of the state
  		data_[2] = ((addr >> 7) & 0x0F)
			    | (((odd + 1) % 2) << 5)
			    | (state << 4)
			    | 0x40;

  		// Invalidate the checksum, so that it is recalculated upon request
			data_[3] = 0;
		}

		void InputReport::address(uint8_t byte1, uint8_t byte2)
		{
			address(byte1, byte2, true);
		}

		void InputReport::address(uint8_t byte1, uint8_t byte2, bool is4kaddress)
		{
			// Note that we can directly set these, as these are set to 0 in the constructor
			data_[1] = byte1;
			data_[2] = byte2;
			// Set the checksum to 0, so that it is recalculated upon request.
			data_[3] = 0;

			address_ = byte1 | ((byte2 & 0x0F)<<7);

			// Multiply by two
  		address_ <<= 1;

  		if (is4kaddress)
  		{
  			// We always need to add 1 to the address, and 2 if its odd
    		// If bit 5 is a 1, then the number is even
    		address_ += (byte2 & 0x20) ? 2 : 1;
  		}
  		else
  		{
  			// We just need to add one to the address
  			address_ += 1;
  		}
		}

		bool InputReport::input()
		{
			// Return the fourth bit of the 3rd byte
			return (data_[2] & 0x10);
		}

		void InputReport::input(bool ishigh)
		{
			if (ishigh)
			{
				// Set the fourth bit
				data_[2] |= (1UL << 4);
			}
			else
			{
				// Clear the fourth bit
				data_[2] &= ~(1UL << 4);
			}

			// Reset the checksum, so that it is recalculated
			data_[3] = 0;
		}

		void InputReport::input(uint8_t byte)
		{
			bool val = (byte & 0x10);
			input(val);
		}
	}
}
