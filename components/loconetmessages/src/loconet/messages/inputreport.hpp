#ifndef LOCONET_MESSAGE_INPUT_REPORT_HPP_
#define LOCONET_MESSAGE_INPUT_REPORT_HPP_

#include "message.hpp"

namespace loconet
{
	namespace messages
	{
		class InputReport : public Message
		{
		public:
			InputReport(uint16_t);
			InputReport(uint16_t, bool);
			InputReport(uint8_t, uint8_t);

			/*
			 * The address, max 10 bits
			 */
			uint16_t address();
			void address(uint16_t);
			void address(uint8_t, uint8_t);
			void address(uint8_t, uint8_t, bool);

			/*
			 * input = 1 => high, input = 0 => low
			 */
			bool input();
			void input(bool);
			void input(uint8_t);

		private:
			uint16_t address_;
		};
	}
}


#endif
