set(COMPONENT_ADD_INCLUDEDIRS loconet)
set(COMPONENT_SRCS
			"loconet/message/message.cpp"
			"loconet/message/busy.cpp"
			"loconet/message/inputreport.cpp"
			"loconet/message/messageparser.cpp"
)

set(COMPONENT_REQUIRES

)

register_component()
