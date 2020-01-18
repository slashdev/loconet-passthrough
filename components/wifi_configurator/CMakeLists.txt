set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCS
  "src/wificonfigurator/handler.cpp"
)

set(COMPONENT_REQUIRES
  httpd_server
  log
)

register_component()
