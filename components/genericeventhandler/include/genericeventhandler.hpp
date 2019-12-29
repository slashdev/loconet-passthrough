#ifndef GENERICEVENTHANDLER_HPP_
#define GENERICEVENTHANDLER_HPP_

#include "esp_err.h"
#include "esp_event_loop.h"

/**
 * This class is used as a wrapper for all classes
 * that need to register on the event loop
 */
class EventHandler
{
public:
  virtual esp_err_t event_handler(void *, system_event_t *);
};

#endif