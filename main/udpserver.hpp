#ifndef UDPSERVER_HPP_
#define UDPSERVER_HPP_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "udpservice.hpp"
#include "udpservicemessagehandler.hpp"
#include "event_handler.hpp"

class UDPServer : public EventHandler, public communication::udp::MessageHandler
{
public:
	UDPServer();
	UDPServer(uint16_t port);

	~UDPServer();

	uint16_t port();
	void port(uint16_t);

	void start();
	void stop();

	esp_err_t handle_event(void *, system_event_t*);
	void on_message_received(uint8_t*, size_t);

	void task(void* pvParameter);


	communication::udp::Service* service();

private:
	communication::udp::Service* service_;
};


#endif
