
set(CMAKE_CXX_FLAGS " -ffunction-sections -fdata-sections  -Os  ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "  -Wl,–gc-sections  -Os ${CMAKE_EXE_LINKER_FLAGS}")
#添加源代码
file(GLOB  SOURCE_C_CPP_FILES  *.c *.h    *.cpp *.hpp  appstack/*.c appstack/*.h  appstack/*.cpp appstack/*.hpp OneNet/*.c OneNet/*.h OneNet/*.cpp OneNet/*.hpp )
set(COMPONENT_SRCS ${SOURCE_C_CPP_FILES})
set(COMPONENT_ADD_INCLUDEDIRS "." "./" "appstack" "OneNet")

#默认RTOS框架中为启用tftp,在此处启用
set(COMPONENT_SRCS ${COMPONENT_SRCS} tftp/tftp_server.c)

register_component()

#添加SimpleMQTTGateWayStack库
add_subdirectory(SimpleMQTTGateWayStack/lib EXCLUDE_FROM_ALL)
target_link_libraries(${COMPONENT_LIB} SMGS)

#添加RC库
add_subdirectory(rc EXCLUDE_FROM_ALL)
target_link_libraries(${COMPONENT_LIB} RC)

#添加jsoncpp库
add_subdirectory(jsoncpp EXCLUDE_FROM_ALL)
target_link_libraries(${COMPONENT_LIB} jsoncpp)

#设置属性
set_property(TARGET ${COMPONENT_LIB} PROPERTY C_STANDARD 11)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)

