#include "session.hpp"

#include "esp_log.h"

namespace httpd
{

	Session::Session(int socket, sockaddr_in from)
	{
		socket_ = socket;
		from_ = from;

		process();
	}

	Session::~Session()
	{
		// ESP_LOGW(TAG, "I am being destroyed");
		// Kill the request
		if (request_ != NULL) delete request_;

		// Kill the response
		if (response_ != NULL) delete response_;

		if (socket_ > 0)
		{
			close(socket_);
		}
		from_ = {};
	}

	Request* Session::request()
	{
		return request_;
	}

	Response* Session::response()
	{
		return response_;
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

	 		// ESP_LOGI(TAG, "I received: \n%s", buffer);

	 		request_ = new Request(buffer);

	 		// Split the buffer into a header and the rest
	 		response_ = new Response();
	 	}
	}


	void Session::reply()
	{
		if (socket_ < 0)
		{
			return;
		}

		// const char* message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		if (response_ == NULL)
		{
			response_ = new Response();
			response_->status(status::NOT_FOUND);
			response_->body(Response::response_phrase(status::NOT_FOUND));
		}
		std::string message = response_->message();

		// ESP_LOGI(TAG, "Sending message:\n%s", message.c_str());

    int err = send(socket_, message.c_str(), message.length(), 0);

    if (err < 0)
    {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
    }
    shutdown(socket_, 0);
    close(socket_);
    socket_ = -1;
	}
}
