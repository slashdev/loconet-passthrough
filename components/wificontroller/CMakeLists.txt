set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  "wificontroller.cpp" )

set(COMPONENT_REQUIRES 
        freertos
    )

register_component()