#include <list>
#include <unordered_map>
#include "event_handlers.hpp"


  /*
   * NOTE: Ensure the following code is added to ```app_main()```:
   * <code>
   *    ESP_ERROR_CHECK(esp_event_loop_create_default());
   * </code>
   */
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

  void add(esp_event_base_t event_base, EventHandler *handler)
  {
    if (event_base == ESP_EVENT_ANY_BASE)
    {
      if (genericHandlers.empty())
      {
        // If the list is empty, it was not yet registered.
        esp_event_handler_register(
            ESP_EVENT_ANY_BASE,
            ESP_EVENT_ANY_ID,
            handle_event, NULL
        );
      }
      genericHandlers.push_back(handler);
      return;
    }

    if (handlers[event_base].empty())
    {
      // Handler does not exist. Let's register
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
      genericHandlers.remove(handler);
      if (genericHandlers.empty())
      {
        esp_event_handler_unregister(
          ESP_EVENT_ANY_BASE,
          ESP_EVENT_ANY_ID,
          handle_event);
      }
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
