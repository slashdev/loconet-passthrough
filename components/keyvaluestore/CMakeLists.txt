set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  
    "keyvaluestore.cpp"
)

set(COMPONENT_REQUIRES 
    log
    nvs_flash
)

register_component()