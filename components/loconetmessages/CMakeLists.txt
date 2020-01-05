set(COMPONENT_ADD_INCLUDEDIRS src)
set(COMPONENT_SRCS
			"src/loconet/messages/message.cpp"
			"src/loconet/messages/busy.cpp"
			"src/loconet/messages/inputreport.cpp"
			"src/loconet/messages/parser.cpp"
			"src/loconet/receiver.cpp"
			"src/loconet/dispatcher.cpp"
            "src/loconet/messages/util.cpp"
)

set(COMPONENT_REQUIRES
			freertos
			log
)

register_component()
