set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  "udpservice.cpp")

set(COMPONENT_REQUIRES
        freertos
        lwip
    )

register_component()
