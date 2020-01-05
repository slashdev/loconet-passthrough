#ifndef EVENT_HANDLERS_HPP
#define EVENT_HANDLERS_HPP

#include "esp_err.h"
#include "esp_event.h"
#include "event_handler.hpp"

/**
 * This class is used as a wrapper for all classes
 * that need to register a handler on the event loop
 */
namespace EventHandlers
{
  void init();
  void add(esp_event_base_t, EventHandler*);
  void remove(esp_event_base_t, EventHandler*);
}

#endif // EVENT_HANDLERS_HPP
