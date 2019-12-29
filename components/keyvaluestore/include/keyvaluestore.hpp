#ifndef KEYVALUESTORE_HPP_
#define KEYVALUESTORE_HPP_

#include <string>
#include <unordered_map>

extern "C" {
  #include "nvs_flash.h"
}

class KeyValueStore
{
public:
  static void initialize();

  ~KeyValueStore();

  uint8_t get_u8(const char*);
  uint16_t get_u16(const char*);
  uint32_t get_u32(const char*);
  std::string get_str(const char*);

  bool store(const char*, uint8_t);
  bool store(const char*, uint16_t);
  bool store(const char*, uint32_t);
  bool store(const char*, std::string);
  bool store(const char*, const char*);

  bool commit();


  static KeyValueStore* ns(std::string);

private:
  KeyValueStore(const char*);

  const char* ns_;
  nvs_handle handle_;
  bool check_key(std::string);
  bool check_key(const char*);

  static bool initialized_;
  static std::unordered_map<std::string, KeyValueStore*> stores_; 
};

#endif