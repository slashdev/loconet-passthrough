#ifndef UDPCONTROLLER_H_
#define UDPCONTROLLER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include <list>

#include "udpcontroller_incoming_message_handler.hpp"

#ifndef UDPCONTROLLER_QUEUE_SIZE
#define UDPCONTROLLER_QUEUE_SIZE 5
#endif

#ifndef UDPCONTROLLER_PORT
#define UDPCONTROLLER_PORT 6000
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
     */
    class Controller
    {
    public:
      Controller();

      /*
       * Enqueues a message on the send buffer
       */
      BaseType_t enqueue_message(uint8_t *, size_t);

      /*
       * Sets the UDP port used for communication
       */
      Controller* set_udpport(uint16_t);

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
       * A single loop of the Controller. It should be embedded in a FreeRTOS Task, 
       * e.g. via:
       * <code>
       * void controller_task( void* pvParameter )
       * {
       *   TickType_t xDelay = pdMS_TO_TICKS( 500 );
           for(;;)
           {
             udpController->work_loop();    
             vTaskDelay(xDelay);
           }
         }
       * </code>
       */
      void work_loop();

      /**
       * Adds a message handler to the controller to see if there are any
       * incoming messages
       */
      void register_handler(MessageHandler*);

      /**
       * Removes a handler
       */
      void unregister_handler(MessageHandler*);

    private:
      /**
       * The internal queue used for sending messages
       */
      QueueHandle_t xQueue_;

      /**
       * The port number used for UDP traffic
       */
      uint16_t udpport_ = UDPCONTROLLER_PORT;

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
       * Function to return the broadcast address
       */
      sockaddr_in get_broadcast_address();

      /**
       * Create a new socket that can be used for broadcasting
       * Also, keep_alive is set, so that it stays open for listening
       */
      int create_socket();

      /**
       * List of all Listeners for the observer pattern
       */
      std::list<MessageHandler*> listeners_;

      /**
       * Takes the first element of the queue en sends it via UDP
       */
      void process_next_message();

      /**
       * Tries to read a single message from the socket
       */
      void listen();
      
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
