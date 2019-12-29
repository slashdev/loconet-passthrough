#include "keyvaluestore.hpp"

#include "esp_err.h"
#include "esp_log.h"

#include <cstring>

std::unordered_map<std::string, KeyValueStore*> KeyValueStore::stores_;
bool KeyValueStore::initialized_ = false;

void KeyValueStore::initialize()
{
  // Initialize
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
  {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  initialized_ = (ret == ESP_OK);
}

KeyValueStore::KeyValueStore(const char* ns)
{
  if (!KeyValueStore::initialized_) {
    KeyValueStore::initialize();
  }

  ns_ = ns;
  handle_ = 0;

  esp_err_t err = nvs_open(ns_, NVS_READWRITE, &handle_);
  ESP_LOGW("keyValueStore", "Opening gave code: %d (%s)", err, esp_err_to_name(err));
  if (err != ESP_OK)
  {
    ESP_LOGW("keyValueStore", "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    handle_ = 0;
  }
}

KeyValueStore::~KeyValueStore()
{
  ESP_LOGW("KeyValueStore", "Closing the store of %s", ns_);
  nvs_close(handle_);
  handle_ = 0;
}

uint8_t KeyValueStore::get_u8(const char* key)
{
  uint8_t result = 0;

  if (handle_ && check_key(key))
  {
    esp_err_t err = nvs_get_u8(handle_, key, &result);
    if (err != ESP_OK) return 0;
  }
  return result;
}

uint16_t KeyValueStore::get_u16(const char* key)
{
  uint16_t result = 0;

  if (handle_ && check_key(key))
  {
    esp_err_t err = nvs_get_u16(handle_, key, &result);
    if (err != ESP_OK) return 0;
  }
  return result;
}

uint32_t KeyValueStore::get_u32(const char* key)
{
  uint32_t result = 0;

  if (handle_)
  {
    esp_err_t err = nvs_get_u32(handle_, key, &result);
    if (err != ESP_OK) return 0;
  }
  return result;
}

std::string KeyValueStore::get_str(const char* key)
{
  if (handle_)
  {
    size_t length;
    esp_err_t err = nvs_get_str(handle_, key, NULL, &length);
    if (err == ESP_OK)
    {
      ESP_LOGI("KeyValueStore", "I have a string of length %d", length);
      char* outp = (char*) malloc(sizeof(char) * length);
      err = nvs_get_str(handle_, key, outp, &length);
      // If it is a 0-terminator, remove it
      if (outp[length-1] == 0) length--;
      
      std::string str = std::string(outp, length);
      free(outp);
      if (err == ESP_OK)
      {
        return str;
      }
    }
  }
  return "";
}

bool KeyValueStore::store(const char* key, uint8_t value)
{
  if (handle_)
  {
    esp_err_t err = nvs_set_u8(handle_, key, value);
    if (err != ESP_OK)
    {
      ESP_LOGW("keyValueStore", "Error (%s) saving key %s with value %d", esp_err_to_name(err), key, value);
      return false;
    }
    return true;
  }
  ESP_LOGW("KeyValueStore", "Handle not set!");
  return false;
}

bool KeyValueStore::store(const char* key, uint16_t value)
{
  if (handle_)
  {
    esp_err_t err = nvs_set_u16(handle_, key, value);
    if (err != ESP_OK)
    {
      ESP_LOGW("keyValueStore", "Error (%s) saving key %s with value %d", esp_err_to_name(err), key, value);
      return false;
    }
    ESP_LOGW("KeyValueStore", "Handle not set!");
    return true;  
  }
  return false;
}

bool KeyValueStore::store(const char* key, uint32_t value)
{
  if (handle_)
  {
    esp_err_t err = nvs_set_u32(handle_, key, value);
    if (err != ESP_OK)
    {
      ESP_LOGW("keyValueStore", "Error (%s) saving key %s with value %d", esp_err_to_name(err), key, value);
      return false;
    }
    return true;  
  }
  ESP_LOGW("KeyValueStore", "Handle not set!");
  return false;
}

bool KeyValueStore::store(const char* key, std::string value)
{
  if (handle_)
  {
    esp_err_t err = nvs_set_str(handle_, key, value.c_str());
    if (err != ESP_OK)
    {
      ESP_LOGW("keyValueStore", "Error (%s) saving key %s with value %s", esp_err_to_name(err), key, value.c_str());
      return false;
    }
    return true;  
  }
  ESP_LOGW("KeyValueStore", "Handle not set!");
  return false;
}

bool KeyValueStore::store(const char* key, const char* value)
{
  if (handle_)
  {
    esp_err_t err = nvs_set_str(handle_, key, value);
    if (err != ESP_OK)
    {
      ESP_LOGW("keyValueStore", "Error (%s) saving key %s with value %s", esp_err_to_name(err), key, value);
      return false;
    }
    return true;  
  }
  ESP_LOGW("KeyValueStore", "Handle not set!");
  return false;
}

bool KeyValueStore::commit()
{
  if (handle_)
  {
    esp_err_t err = nvs_commit(handle_);
    if (err != ESP_OK)
    {
      ESP_LOGW("keyValueStore", "Error in commit of ns '%s': %s", ns_, esp_err_to_name(err));
      return false;
    }
    return true;
  }
  ESP_LOGW("KeyValueStore", "Handle not set for commit!");
  return false;
}

bool KeyValueStore::check_key(const char* key)
{
  return (std::strlen(key) < 16);
}

bool KeyValueStore::check_key(std::string key)
{
  return (key.length() < 16);
}

KeyValueStore* KeyValueStore::ns(std::string name)
{
  auto item = stores_.find(name);
  if (item == stores_.end())
  {
    ESP_LOGI("KeyValueStore", "Create store for: '%s'", name.c_str());
    KeyValueStore* s = new KeyValueStore(name.c_str());
    stores_.insert({name, s});
    return s;
  }
  else
  {
    return item->second; 
  }
}