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
    if (request_ != NULL)
    {
      delete request_;
    }
    if (response_ != NULL)
    {
      delete response_;
    }

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

  int Session::socket()
  {
    return socket_;
  }

  bool Session::process()
  {
    if (socket_ < 0)
    {
      ESP_LOGW(TAG, "No socket!");
      return false;
    }

    char buffer[HTTD_SERVER_MAX_REQUEST_LENGTH];

    int len = recv(socket_, buffer, sizeof(buffer) - 1, 0);
    if (len > 0)
    {
      // We received a message! First terminate the string :-)
      buffer[len] = 0;

      ESP_LOGI(TAG, "I received %d bytes", len);

      request_ = new Request(buffer);
      response_ = new Response();

      return true;
    }
    return false;
}

  void Session::reply()
  {
    if (socket_ < 0)
    {
      return;
    }

    if (response_ == NULL)
    {
      ESP_LOGI(TAG, "Response is NULL, cannot send it");
    }
    std::string message = response_->message();

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
