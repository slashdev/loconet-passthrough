#include "server.hpp"
#include "esp_log.h"

namespace httpd
{

  Server::Server() : Server(80)
  {
  }

  Server::~Server()
  {
    handlers_.clear();

    for(auto &s: sessions_)
    {
      delete s;
    }
    sessions_.clear();
  }

  Server::Server(uint16_t port)
  {
    dest_addr_ = {};
    dest_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_.sin_family = AF_INET;
    dest_addr_.sin_port = htons(port);
  }

  void Server::add(URIHandler* handler)
  {
    handlers_.push_back(handler);
  }

  void Server::remove(URIHandler* handler)
  {
    handlers_.remove(handler);
  }

  esp_err_t Server::stop()
  {
    close(socket_);
    socket_ = -1;

    return ESP_OK;
  }

  esp_err_t Server::start()
  {

    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;


    socket_ = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (socket_ < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", socket_);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Socket created");

    // Enable SO_REUSEADDR to allow binding to the same address and port when restarting the server
    int enable = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    int err = bind(socket_, (struct sockaddr *)&dest_addr_, sizeof(dest_addr_));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", err);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", dest_addr_.sin_port);

    err = listen(socket_, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", err);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Socket listening");

    return ESP_OK;
  }

  esp_err_t Server::process_sessions()
  {
    if (socket_ < 0)
    {
      ESP_LOGE(TAG, "NO SOCKET");
      return ESP_FAIL;
    }

    // ESP_LOGI(TAG, "On the stack: %d", sessions_.size());

    while(!sessions_.empty())
    {
      Session* session = sessions_.front();
      sessions_.pop_front();

      // ESP_LOGI(TAG, "Working on session: %d", session->socket());

      // session->process();

      bool handled = false;
      for(auto &handler: handlers_)
      {
        if (handler->accept(session->request()))
        {
          handler->handle(session->request(), session->response());
          handled = true;
          break;
        }
      }
      if (!handled)
      {
        // Set the error to 404 or NOT IMPLEMENTED
        switch(session->request()->method())
        {
          case method::GET:
          case method::HEAD:
          case method::POST:
            session->response()->status(status::NOT_FOUND);
            break;
          case method::OTHER:
            session->response()->status(status::NOT_IMPLEMENTED);
            break;
          default:
            session->response()->status(status::METHOD_NOT_ALLOWED);
            break;
        }
        session->response()->construct_default_message(
            "method: " + Request::method_to_str(session->request()->method())
            + " not possible on URI: " + session->request()->uri()
        );
      }

      session->reply();
      delete session;
    }

    return ESP_OK;
  }

  esp_err_t Server::process_accept()
  {
    if (socket_ < 0)
    {
      ESP_LOGD(TAG, "NO SOCKET");
      return ESP_FAIL;
    }

    // It's full, we don't accept any new
    if (sessions_.size() > HTTPD_SESSION_QUEUE_SIZE)
    {
      ESP_LOGW(TAG, "sessions full");
      return ESP_OK;
    }

    // ESP_LOGI(TAG, "accept new connection");
    // ESP_LOGI(TAG, "Sessions on the stack: %d", sessions_.size());
    sockaddr_in addr_from;
    socklen_t addr_from_len = sizeof(addr_from);

    // THIS LINE FAILS
    int conn = accept(socket_, (sockaddr *)&addr_from, &addr_from_len);

    if (conn < 0) {
      ESP_LOGW(TAG, "Error in accept (%d)", conn);
      return ESP_FAIL;
    }
    // ESP_LOGI(TAG, "newfd = %d", conn);

    timeval tv;
    // Set recv timeout of this fd
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    // Snd the same time out for sending
    setsockopt(conn, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));

    Session* session = new Session(conn, addr_from);

    if (session->valid())
    {
      sessions_.push_back(session);
    }
    else
    {
      delete session;
    }

    return ESP_OK;
  }

  esp_err_t Server::thread()
  {
    process_sessions();
    process_accept();
    return ESP_OK;
  }

}
