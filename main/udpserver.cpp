#include "udpserver.hpp"
#include "esp_log.h"
#include "event_handlers.hpp"

UDPServer::UDPServer()
{
	service_ = new communication::udp::Service();
	EventHandlers::add(this);
}

UDPServer::UDPServer(uint16_t portnumber) : UDPServer()
{
	port(portnumber);
}

UDPServer::~UDPServer()
{
	stop();
}

uint16_t UDPServer::port()
{
	return service_->udpport();
}

void UDPServer::port(uint16_t portnumber)
{
	service_->udpport(portnumber);
}

void UDPServer::start()
{
	service_->start();
}

void UDPServer::stop()
{
	service_->stop();
}

communication::udp::Service* UDPServer::service()
{
	return service_;
}

esp_err_t UDPServer::handle_event(void *ctx, system_event_t *event)
{
	switch(event->event_id)
	{
		case SYSTEM_EVENT_STA_GOT_IP:
			start();
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
			stop();
			break;
		default:
			break;
	}
	return ESP_OK;
}

void UDPServer::on_message_received(uint8_t *data, size_t length)
{
	(void) data;
	ESP_LOGI("UDPServer", "I received a message of size: %d", length);
}

void UDPServer::task(void* pvParameter)
{
	TickType_t xDelay = pdMS_TO_TICKS( 500 );

	for(;;)
	{
		service_->process_next_message();
    service_->listen();
    vTaskDelay(xDelay);
	}
}
