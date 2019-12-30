#include <list>
#include "event_handlers.hpp"

namespace EventHandlers
{
  // List of handlers
  std::list<EventHandler*> handlers;

  void add(EventHandler *handler)
  {
    handlers.push_back(handler);
  }

  void remove(EventHandler *handler)
  {
    handlers.remove(handler);
  }

  esp_err_t handle_event(void* ctx, system_event_t *event)
  {
    esp_err_t err;

    for(auto &handler : handlers)
    {
      err = handler->handle_event(ctx, event);
      if (err != ESP_OK)
      {
        return err;
      }
    }

    return ESP_OK;
  }
}
