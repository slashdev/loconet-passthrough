#ifndef HTTPD_BASE_CLASS_HPP_
#define HTTPD_BASE_CLASS_HPP_

#include "esp_log.h"

namespace httpd
{

  class BaseClass
  {
  public:
    void *operator new(size_t size)
    {
      ESP_LOGI("BASECLASS", "Constructor called");
      void *p = malloc(size);
      return p;
    }
    void operator delete(void* p)
    {
      ESP_LOGI("BASECLASS", "Destructor called");
      free(p);
    }
  };

}

#endif
