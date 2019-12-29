#ifndef UDPCONTROLLER_LISTENER_H_
#define UDPCONTROLLER_LISTENER_H_

namespace communication
{
  namespace udp
  {

    class MessageHandler
    {
    public:
      virtual void on_message_received(uint8_t*, size_t);
    };
    
  } // End of namespace udp
} // End of namespace communication

#endif