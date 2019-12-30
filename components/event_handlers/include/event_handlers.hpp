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
  void add(EventHandler*);
  void remove(EventHandler*);

  esp_err_t handle_event(void*, system_event_t*);
}

#endif // EVENT_HANDLERS_HPP
