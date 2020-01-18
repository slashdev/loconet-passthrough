set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCS
  "src/httpd/server.cpp"
  "src/httpd/session.cpp"
  "src/httpd/request.cpp"
  "src/httpd/response.cpp"
  "src/httpd/util/uri.cpp"
  "src/httpd_server_task.cpp"
)

set(COMPONENT_REQUIRES
  log
  lwip
)

register_component()
