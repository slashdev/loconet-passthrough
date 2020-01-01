set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCS
        "src/httpd/server.cpp"
        "src/httpd/session.cpp"
)

set(COMPONENT_REQUIRES
        log
        lwip
    )

register_component()
