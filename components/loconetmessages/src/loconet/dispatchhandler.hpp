#ifndef LOCONET_DISPATCH_HANDLER_HPP_
#define LOCONET_DISPATCH_HANDLER_HPP_

namespace loconet
{
	class DispatchHandler
	{
	public:
		virtual void handle_message(uint8_t*, size_t) = 0;
	};
}

#endif
