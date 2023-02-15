#添加源代码
file(GLOB  SOURCE_C_CPP_FILES  *.c    *.cpp  appstack/*.c appstack/*.cpp)
set(COMPONENT_SRCS ${SOURCE_C_CPP_FILES})
set(COMPONENT_ADD_INCLUDEDIRS "." "./" "appstack")

#默认RTOS框架中为启用tftp,在此处启用
set(COMPONENT_SRCS ${COMPONENT_SRCS} tftp/tftp_server.c)

register_component()

#添加SimpleMQTTGateWayStack库
add_subdirectory(SimpleMQTTGateWayStack/lib EXCLUDE_FROM_ALL)
target_link_libraries(${COMPONENT_LIB} SMGS)

#添加RC库
add_subdirectory(rc EXCLUDE_FROM_ALL)
target_link_libraries(${COMPONENT_LIB} RC)

