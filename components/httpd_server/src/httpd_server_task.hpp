#ifndef HTTPD_SERVER_TASK_HPP_
#define HTTPD_SERVER_TASK_HPP_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "httpd/server.hpp"
#include "httpd/request_handler.hpp"

extern void httpd_server_add_handler(httpd::RequestHandler *);
extern void httpd_server_remove_handler(httpd::RequestHandler *);


extern "C"
{
  void httpd_server_initialize(uint32_t, UBaseType_t);
}


#endif
