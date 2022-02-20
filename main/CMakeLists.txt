#添加源代码
file(GLOB  SOURCE_C_CPP_FILES  *.c  *.C  *.cpp  *.CPP)
set(COMPONENT_SRCS ${SOURCE_C_CPP_FILES})
set(COMPONENT_ADD_INCLUDEDIRS "." "./")

#默认RTOS框架中为启用tftp,在此处启用
set(COMPONENT_SRCS ${COMPONENT_SRCS} tftp/tftp_server.c)

register_component()
