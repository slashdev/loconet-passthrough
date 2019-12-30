set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCS
			"src/loconet/message/message.cpp"
			"src/loconet/message/busy.cpp"
			"src/loconet/message/inputreport.cpp"
			"src/loconet/message/messageparser.cpp"
)

set(COMPONENT_REQUIRES
			freertos
)

register_component()
