#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
COMPONENT_ADD_INCLUDEDIRS := include

COMPONENT_OBJS = main.o  src/init.o  src/wifi_station.o src/appcmd.o src/mqttapp.o src/tftpd.o


#默认RTOS框架中为启用tftp,在此处启用
COMPONENT_OBJS += tftp/tftp_server.o

COMPONENT_SRCDIRS := src ./

#默认RTOS框架中为启用tftp,在此处启用
COMPONENT_SRCDIRS += tftp

COMPONENT_SUBMODULES += main

