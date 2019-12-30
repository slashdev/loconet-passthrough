#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include "esp_err.h"
#include "esp_event.h"

/**
 * This class is used as a wrapper for all classes
 * that need to register on the event loop
 */
class EventHandler
{
public:
  virtual esp_err_t handle_event(void *, system_event_t*)=0;
};

#endif // EVENT_HANDLER_HPP
