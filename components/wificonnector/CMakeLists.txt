set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  
        "wifibase.cpp"
        "wifiap.cpp" 
        "wifistation.cpp"
)

set(COMPONENT_REQUIRES 
        event_handlers
    )

register_component()