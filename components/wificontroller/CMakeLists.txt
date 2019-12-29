set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  
        "wifibase.cpp"
        "wifiap.cpp" 
        "wifistation.cpp"
)

set(COMPONENT_REQUIRES 
        genericeventhandler
    )

register_component()