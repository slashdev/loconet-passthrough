set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  "udpcontroller.cpp")

set(COMPONENT_REQUIRES 
        freertos
        log
        lwip
    )

register_component()