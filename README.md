# Loconet Passthrough

This is an application for the ESP32 WROOM push Loconet messages over UDP. Any
messages sent over UDP (on the correct port) will be passed to Loconet.

When a message is passed that expects a response as next message on Loconet it
will keep back all other messages until that response is received or a timeout
expires waiting for that message.
