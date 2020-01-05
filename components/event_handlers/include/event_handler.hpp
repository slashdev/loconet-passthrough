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
  virtual void handle_event(esp_event_base_t base, int32_t event_id, void* data)=0;
};

#endif // EVENT_HANDLER_HPP
