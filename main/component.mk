#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
COMPONENT_ADD_INCLUDEDIRS := ./
COMPONENT_SRCDIRS :=  ./

#默认RTOS框架中为启用tftp,在此处启用
COMPONENT_SRCDIRS += tftp

COMPONENT_SUBMODULES += main

