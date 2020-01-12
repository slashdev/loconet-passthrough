set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCS
  "src/wifihandler/handler.cpp"
)

set(COMPONENT_REQUIRES
  httpd_server
  wifi_connector
  log
)

register_component()
