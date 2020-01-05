set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS
        "wifi_base.cpp"
        "wifi_ap.cpp"
        "wifi_station.cpp"
        "wifi_connector.cpp"
)

set(COMPONENT_REQUIRES
        event_handlers
    )

register_component()
