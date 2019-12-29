#ifndef GENERICEVENTLOOP_HPP_
#define GENERICEVENTLOOP_HPP_

#include "esp_err.h"
#include "esp_event_loop.h"
#include "genericeventhandler.hpp"
#include <list>

/**
 * This class is used as a wrapper for all classes
 * that need to register on the event loop
 */
class EventLoop
{
public:
  ~EventLoop();

  void register_handler(EventHandler*);
  void unregister_handler(EventHandler*);

  esp_err_t handle(void*, system_event_t *);

  static EventLoop* get_instance();

private:
  EventLoop();
  static EventLoop* instance_;
  std::list<EventHandler*> handlers_;
};

#endif