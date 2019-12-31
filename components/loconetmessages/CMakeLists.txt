set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCS
			"src/loconet/message/message.cpp"
			"src/loconet/message/busy.cpp"
			"src/loconet/message/inputreport.cpp"
			"src/loconet/message/messageparser.cpp"
			"src/loconet/receiver.cpp"
			"src/loconet/dispatcher.cpp"
)

set(COMPONENT_REQUIRES
			freertos
)

register_component()
