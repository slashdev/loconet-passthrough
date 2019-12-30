#ifndef UDPCONTROLLER_H_
#define UDPCONTROLLER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include <list>

#include "udpservicemessagehandler.hpp"

#ifndef UDPSERVICE_QUEUE_SIZE
#define UDPSERVICE_QUEUE_SIZE 5
#endif

#ifndef UDPSERVICE_PORT
#define UDPSERVICE_PORT 6000
#endif


// namespace slashdev::loconet::communication::udp
namespace communication
{
  namespace udp
  {


    /*
     * Struct only used internally to represent a message
     */
    typedef struct {
      uint8_t *data;
      size_t size;
    } udpmessage_t;


    /**
     * @brief UdpController sending and receiving messages via broadcasting
     * e.g. via:
     * <code>
     * void controller_task( void* pvParameter )
     * {
     *   TickType_t xDelay = pdMS_TO_TICKS( 500 );
     *   for(;;)
     *   {
     *     udpController->process_next_message();
     *     udpController->listen();
     *     vTaskDelay(xDelay);
     *   }
     * }
     * </code>
     */

    class Service
    {
    public:
      Service();
      ~Service();

      /*
       * Enqueues a message on the send buffer
       */
      BaseType_t enqueue(uint8_t *, size_t);

      /*
       * Returns the current UDP Port
       */
      uint16_t udpport();

      /*
       * Sets the UDP port used for communication.
       * Registered Ports only (see IANA Allocation Guidelines for TCP and UDP Port Numbers).
       * If invalid port is used, then UDPSERVICE_PORT is used.
       */
      void udpport(uint16_t);

      /*
       * Starts the Controller. This function creates a socket
       * that is used by the listener and sender.
       * If all goes well, it sets running_ to true.
       */
      void start();

      /**
       * Stops the controller. It closes and removes the socket.
       */
      void stop();

      /**
       * Returns whether the service is started
       */
      bool is_running();

       /**
       * Takes the first element of the queue en sends it via UDP
       */
      void process_next_message();

      /**
       * Tries to read a single message from the socket
       */
      void listen();

      /**
       * Adds a message handler to the controller to see if there are any
       * incoming messages
       */
      void add(MessageHandler*);

      /**
       * Removes a handler
       */
      void remove(MessageHandler*);

    private:
      /**
       * The internal queue used for sending messages
       */
      QueueHandle_t xQueue_;

      /**
       * The socket
       */
      int socket_ = -1;

      /**
       * To keep track whether the system is running
       * In principle, the following assertion should hold:
       *   running === (socket_ > 0)
       */
      bool running_ = false;

      /**
       * Default address for broadcasting.
       */
      sockaddr_in broadcast_address_ = {};

      /**
       * Function to return the broadcast address
       */
      sockaddr_in broadcast_address();

      /**
       * Create a new socket that can be used for broadcasting
       * Also, keep_alive is set, so that it stays open for listening
       */
      int create_socket();

      /**
       * List of all Handlers for the observer pattern
       */
      std::list<MessageHandler*> handlers_;

      /**
       * Notifies all listeners if a message is sent.
       * It gives each listener its own copy of the message,
       * after which it frees the memory again.
       */
      void notify_message_received(udpmessage_t);

    }; // end of class

  } // End of namespace udp
} // End of namespace communication

#endif
