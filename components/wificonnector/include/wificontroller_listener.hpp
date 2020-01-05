#ifndef WIFICONNECTOR_LISTENER_H_
#define WIFICONNECTOR_LISTENER_H_

namespace communication
{
  namespace wifi
  {
    class ConnectionListener
    {
    public:
        virtual void on_connect();
        virtual void on_disconnect();
        virtual void on_start_as_ap();
    };

  } // End of namespace wifi
} // End of namespace communication
#endif