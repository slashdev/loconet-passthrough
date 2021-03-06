#include <list>
#include <unordered_map>
#include "esp_err.h"

#include "event_handlers.hpp"

namespace EventHandlers
{

  /*
   * There are two types of handlers:
   * - handlers registered for a specific event_base
   * - generic events that listen to any event_base
   *
   * The map stores the former, genericHandlers the latter.
   */
  std::unordered_map<esp_event_base_t, std::list<EventHandler*>> handlers;
  std::list<EventHandler*> genericHandlers;

  void handle_event(void* arg, esp_event_base_t base, int32_t id,  void* data)
  {
    for(const auto &handler : genericHandlers)
    {
      handler->handle_event(base, id, data);
    }

    for(const auto &handler : handlers[base])
    {
      handler->handle_event(base, id, data);
    }
  }

  void init()
  {
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Register ourselves as event handler to
    // any event in the system.
    ESP_ERROR_CHECK(esp_event_handler_register(
        ESP_EVENT_ANY_BASE,
        ESP_EVENT_ANY_ID,
        handle_event, NULL
    ));
  }


  void add(esp_event_base_t event_base, EventHandler *handler)
  {
    if (event_base == ESP_EVENT_ANY_BASE)
    {
      genericHandlers.push_back(handler);
      return;
    }

    handlers[event_base].push_back(handler);
  }

  void remove(esp_event_base_t event_base, EventHandler *handler)
  {
    if (event_base == ESP_EVENT_ANY_BASE)
    {
      genericHandlers.remove(handler);
      return;
    }

    handlers[event_base].remove(handler);
  }
}
