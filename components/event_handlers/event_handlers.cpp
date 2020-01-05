#include <list>
#include <unordered_map>
#include "event_handlers.hpp"

namespace EventHandlers
{

  std::unordered_map<esp_event_base_t, std::list<EventHandler*>> handlers;
  std::list<EventHandler*> allHandlers;

  void handle_event(void* arg, esp_event_base_t base, int32_t id,  void* data)
  {
    for(auto &handler : allHandlers)
    {
      handler->handle_event(base, id, data);
    }
    if (handlers.find(base) != handlers.end())
    {
      for(auto &handler : handlers[base])
      {
        handler->handle_event(base, id, data);
      }
    }
  }

  void add(esp_event_base_t event_base, EventHandler *handler)
  {
    if (event_base == ESP_EVENT_ANY_BASE)
    {
      if (allHandlers.empty())
      {
        // If the list is empty, it was not yet registered.
        esp_event_handler_register(
            ESP_EVENT_ANY_BASE,
            ESP_EVENT_ANY_ID,
            handle_event, NULL
        );
      }
      allHandlers.push_back(handler);
      return;
    }

    if (handlers.find(event_base) == handlers.end())
    {
      // handler does not exist. Let's register
      esp_event_handler_register(
        event_base,
        ESP_EVENT_ANY_ID,
        handle_event, NULL);
    }
    handlers[event_base].push_back(handler);
  }

  void remove(esp_event_base_t event_base, EventHandler *handler)
  {
    if (event_base == ESP_EVENT_ANY_BASE)
    {
      allHandlers.remove(handler);
      if (allHandlers.empty())
      {
        esp_event_handler_unregister(
          ESP_EVENT_ANY_BASE,
          ESP_EVENT_ANY_ID,
          handle_event);
      }
      return;
    }
    if (handlers.find(event_base) == handlers.end())
    {
      // handler was never registered for this event base!
      return;
    }
    handlers[event_base].remove(handler);
    if (handlers[event_base].empty())
    {
      esp_event_handler_unregister(
        event_base,
        ESP_EVENT_ANY_ID,
        handle_event);
    }
  }

}
