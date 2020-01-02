#include "session.hpp"

#include "esp_log.h"

namespace httpd
{

	Session::Session(int socket, sockaddr_in from)
	{
		socket_ = socket;
		from_ = from;
	}

	Session::~Session()
	{
		ESP_LOGI(TAG, "Destructor called");
		if (socket_ > 0)
		{
			close(socket_);
		}
		from_ = {};
	}


	void Session::process()
	{
		if (socket_ < 0)
		{
			ESP_LOGW(TAG, "No socket!");
			return;
		}

		char buffer[HTTD_SERVER_MAX_REQUEST_LENGTH];

	 	int len = recv(socket_, buffer, sizeof(buffer) - 1, 0);
	 	if (len > 0)
	 	{
	 		// We received a message! First terminate the string :-)
	 		buffer[len] = 0;

	 		// Split the buffer into a header and the rest

	 		const char* message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";


	    int err = send(socket_, message, strlen(message), 0);
	    if (err < 0) {
	        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
	    }
	    close(socket_);
	 	}
	}


}
