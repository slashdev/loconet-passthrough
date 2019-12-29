#include "genericeventloop.hpp"

EventLoop::EventLoop()
{
}

EventLoop::~EventLoop()
{
  handlers_.clear();
}


void EventLoop::register_handler(EventHandler* handler)
{
  handlers_.push_back(handler);
}

void EventLoop::unregister_handler(EventHandler* handler)
{
  handlers_.remove(handler);
}

esp_err_t EventLoop::handle(void* ctx, system_event_t * event)
{
  for(auto &h : handlers_)
  {
    esp_err_t err = h->event_handler(ctx, event);
    if (err != ESP_OK)
    {
      return err;
    }
  }
  return ESP_OK;
}


// Singleton pattern
EventLoop* EventLoop::instance_ = NULL;

EventLoop* EventLoop::get_instance()
{
  if (instance_ == NULL)
  {
    instance_ = new EventLoop();
  }
  return instance_;
}
