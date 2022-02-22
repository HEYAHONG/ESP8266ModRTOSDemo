#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
COMPONENT_ADD_INCLUDEDIRS := ./
COMPONENT_SRCDIRS :=  ./

COMPONENT_ADD_INCLUDEDIRS += appstack
COMPONENT_SRCDIRS +=  appstack

#默认RTOS框架中为启用tftp,在此处启用
COMPONENT_SRCDIRS += tftp

#添加 SimpleMQTTGateWayStack
COMPONENT_SRCDIRS +=  SimpleMQTTGateWayStack/lib/
COMPONENT_ADD_INCLUDEDIRS += SimpleMQTTGateWayStack/lib/include

#添加PAHO_MQTT
COMPONENT_SRCDIRS +=  SimpleMQTTGateWayStack/lib/3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/
COMPONENT_ADD_INCLUDEDIRS += SimpleMQTTGateWayStack/lib/3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/

COMPONENT_SUBMODULES += main

