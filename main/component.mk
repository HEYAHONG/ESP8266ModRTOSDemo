#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
COMPONENT_ADD_INCLUDEDIRS := ./
COMPONENT_SRCDIRS :=  ./

COMPONENT_ADD_INCLUDEDIRS += appstack OneNetToken
COMPONENT_SRCDIRS +=  appstack OneNetToken

#默认RTOS框架中为启用tftp,在此处启用
COMPONENT_SRCDIRS += tftp

#添加 SimpleMQTTGateWayStack
COMPONENT_SRCDIRS +=  SimpleMQTTGateWayStack/lib/
COMPONENT_ADD_INCLUDEDIRS += SimpleMQTTGateWayStack/lib/include

#添加PAHO_MQTT
COMPONENT_SRCDIRS +=  SimpleMQTTGateWayStack/lib/3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/
COMPONENT_ADD_INCLUDEDIRS += SimpleMQTTGateWayStack/lib/3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/

#添加RC
PATH := $(PROJECT_PATH)/main/rc/:$(PATH)
FSGEN := $(shell which $(PROJECT_PATH)/main/rc/fsgen)
G++PATH := $(shell which g++)
ifndef FSGEN
    $(warning fsgen is not found,try to compile it)
    ifndef G++PATH
        $(error g++ is not installed)
    else
RC_fs:
		$(G++PATH) -static -static-libgcc -static $(PROJECT_PATH)/main/rc/fsgen.cpp -o $(PROJECT_PATH)/main/rc/fsgen
		$(PROJECT_PATH)/main/rc/fsgen  $(PROJECT_PATH)/main/rc/fs $(PROJECT_PATH)/main/rc/RC_fs.c
    endif
else
RC_fs:
		$(PROJECT_PATH)/main/rc/fsgen $(PROJECT_PATH)/main/rc/fs $(PROJECT_PATH)/main/rc/RC_fs.c
endif
.PHONY:RC_fs

build:RC_fs


COMPONENT_SRCDIRS +=  rc/
COMPONENT_OBJEXCLUDE += rc/fsgen.o
COMPONENT_ADD_INCLUDEDIRS += rc/

COMPONENT_SUBMODULES += main


