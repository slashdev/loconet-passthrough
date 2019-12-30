#include "udpservice.hpp"
// Required for memcpy
#include <cstring>
#include "tcpip_adapter.h"


namespace communication
{
  namespace udp
  {

    Service::Service()
    {
      xQueue_ = xQueueCreate(UDPSERVICE_QUEUE_SIZE, sizeof( udpmessage_t * ) );

      broadcast_address_ = {};
      broadcast_address_.sin_family         = AF_INET,
      broadcast_address_.sin_port           = htons(UDPSERVICE_PORT);
      broadcast_address_.sin_addr.s_addr    = inet_addr("255.255.255.255");
      broadcast_address_.sin_len            = sizeof(broadcast_address_);

    }

    Service::~Service()
    {
      stop();
      vQueueDelete(xQueue_);
      handlers_.clear();
    }

    void Service::start()
    {
      if (socket_ < 0) {
        socket_ = this->create_socket();
        if (socket_ >= 0) {
          running_ = true;
        }
      }
    }

    void Service::stop()
    {
      if (socket_ > 0)
      {
        close(socket_);
        socket_ = -1;
      }
      running_ = false;
    }


    uint16_t Service::udpport()
    {
      return broadcast_address_.sin_port;
    }

    void Service::udpport(uint16_t port)
    {
      broadcast_address_.sin_port = htons( (port < 1024 || port > 49151) ? UDPSERVICE_PORT : port );
      broadcast_address_.sin_len = sizeof( broadcast_address_ );
    }

    BaseType_t Service::enqueue(uint8_t *data, size_t length)
    {
      // We create a copy of the data that is passed to us
      // so that we can refer to it from the queue
      // REMEMBER: the process_next_message function is in charge of
      // freeing this space!
      udpmessage_t* msg = (udpmessage_t*) malloc( sizeof(udpmessage_t));
      msg->data = (uint8_t*) malloc(sizeof(uint8_t) * length);
      msg->size = length;

      std::memcpy(msg->data, data, sizeof(uint8_t) * length);

      return xQueueSendToBack(xQueue_, (void *) &msg, 0);
    }

    sockaddr_in Service::broadcast_address()
    {
      return broadcast_address_;
    }

    void Service::process_next_message()
    {
      if (socket_ < 0)
      {
        return;
      }

      udpmessage_t* msg;
      BaseType_t xStatus;

      xStatus = xQueueReceive(xQueue_, &msg, 0);
      if (xStatus == pdPASS)
      {

        sockaddr_in send_addr = this->broadcast_address();
        sendto(socket_, msg->data, msg->size, 0, (const struct sockaddr*) & send_addr, sizeof(send_addr) );
        // Free the memory of msg
        free(msg->data);
        free(msg);
      }
    }

    void Service::listen()
    {
      char rx_buffer[128];
      if (running_)
      {
        struct sockaddr_in6 sourceAddr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(sourceAddr);

        // Notice that we pass the flat MSG_DONTWAIT to ensure that this
        // function is non blocking!
        int len = recvfrom(socket_, rx_buffer, sizeof(rx_buffer) - 1, MSG_DONTWAIT, (struct sockaddr *)&sourceAddr, &socklen);

        // If the len is non-positive, then it indicates an error in receiving
        // the message
        if (len > 0)
        {
          udpmessage_t msg = {};
          msg.size = len;
          msg.data = (uint8_t*) malloc (sizeof(uint8_t)*len);
          std::memcpy(msg.data, rx_buffer, sizeof(uint8_t) * len);

          this->notify_message_received(msg);

          // As we do a malloc, we also need to free it again!
          free(msg.data);
        }
      }
    }

    int Service::create_socket()
    {
      struct sockaddr_in saddr = { };
      int sock = -1;
      int err = 0;

      sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
      if (sock < 0)
      {
          return -1;
      }

      // Make it a broadcast
      int broadcast = 1;
      err = setsockopt( sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast) );
      int keepalive = 1;
      setsockopt( sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive) );

      // Bind the socket to any address
      saddr.sin_family = PF_INET;
      saddr.sin_port = broadcast_address().sin_port;
      saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      err = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
      if (err < 0)
      {
        close(sock);
        return -2;
      }

      return sock;
    }


    void Service::add(MessageHandler* handler)
    {
      handlers_.push_back(handler);
    }

    void Service::remove(MessageHandler* handler)
    {
      handlers_.remove(handler);
    }

    void Service::notify_message_received(udpmessage_t msg)
    {
      // Each listener gets its own data. After the listener
      // finished, the data is removed
      for(auto const&l : handlers_)
      {
        uint8_t* data = (uint8_t*) malloc(sizeof(uint8_t) * msg.size);
        std::memcpy(data, msg.data, sizeof(uint8_t) * msg.size);
        l->on_message_received(data, msg.size);
        free(data);
      }
    }
  } // End of namespace udp
} // End of namespace communication
