#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
COMPONENT_ADD_INCLUDEDIRS := include

COMPONENT_OBJS = main.o  src/init.o  src/wifi_station.o src/appcmd.o src/mqttapp.o  

COMPONENT_SRCDIRS := src ./

COMPONENT_SUBMODULES += main

