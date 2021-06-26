set(COMPONENT_SRCS "main.c" "src/init.c" "src/wifi_station.c" "src/mqttapp.c" "src/appcmd.c" "src/tftpd.c")
set(COMPONENT_ADD_INCLUDEDIRS "." "include")

#默认RTOS框架中为启用tftp,在此处启用
set(COMPONENT_SRCS ${COMPONENT_SRCS} tftp/tftp_server.c)

register_component()
