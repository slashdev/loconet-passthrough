set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  
        "src/webserver.cpp"
        "src/urihandler.cpp"
        "src/gethandler.cpp"
        "src/posthandler.cpp"
        "src/stringhandler.cpp"
)

set(COMPONENT_REQUIRES 
        log
        esp_http_server
    )

register_component()