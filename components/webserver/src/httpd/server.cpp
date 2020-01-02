#include "server.hpp"
#include "esp_log.h"


namespace httpd
{

  Server::Server() : Server(80)
  {
  }

  Server::~Server()
  {
    // TODO: If there are items on the queue, delete them all
    if (uxQueueMessagesWaiting(sessionQueue_) > 0)
    {
      Session* session;
      BaseType_t xStatus;

      xStatus = xQueueReceive( sessionQueue_, &session, 0 );
      if (xStatus == pdPASS)
      {
        delete session;
      }
    }

    vQueueDelete(sessionQueue_);
  }

  Server::Server(uint16_t port)
  {
    dest_addr_ = {};
    dest_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_.sin_family = AF_INET;
    dest_addr_.sin_port = htons(port);

    sessionQueue_ = xQueueCreate(HTTPD_SESSION_QUEUE_SIZE, sizeof( Session * ) );
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

  esp_err_t Server::process_session()
  {
    if (socket_ < 0)
    {
      ESP_LOGD("HTTPD", "NO SOCKET");
      return ESP_FAIL;
    }
    Session* session;
    BaseType_t xStatus;

    xStatus = xQueueReceive( sessionQueue_, &session, 0 );
    if (xStatus == pdPASS)
    {
      session->process();
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

    sockaddr_in addr_from;
    socklen_t addr_from_len = sizeof(addr_from);
    int new_fd = accept(socket_, (sockaddr *)&addr_from, &addr_from_len);

    if (new_fd < 0) {
      ESP_LOGW(TAG, "Error in accept (%d)", new_fd);
      return ESP_FAIL;
    }
    ESP_LOGD(TAG, "newfd = %d", new_fd);

    timeval tv;
    // Set recv timeout of this fd
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(new_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    // Snd the same time out for sending
    setsockopt(new_fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));

    Session* session = new Session(new_fd, addr_from);

    return xQueueSendToBack(sessionQueue_, (void *) &session, 0);

    return ESP_OK;
  }


}
