# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ESP8266Mod)


set(SPIFFS_PARTITION_NAME "storage")
partition_table_get_partition_info(SPIFFS_PARTITION_SIZE "--partition-name ${SPIFFS_PARTITION_NAME}" "size")
partition_table_get_partition_info(SPIFFS_PARTITION_OFFSET "--partition-name ${SPIFFS_PARTITION_NAME}" "offset")
idf_build_get_property(SPIFFS_BUILD_DIR BUILD_DIR)
idf_build_get_property(SPIFFS_SDKCONFIGHEADER_DIR SDKCONFIG_HEADER)
set(SPIFFS_IDF_PATH $ENV{IDF_PATH})
set(SPIFFS_IMAGE_NAME "${SPIFFS_BUILD_DIR}/spiffs_image.bin")
set(SPIFFS_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spiffs_image")

find_program(MAKE NAMES mingw32-make make)
if(MAKE-NOTFOUND)
	message(FATAL_ERROR "make is not found,please install it!")
endif()

find_program(CMAKE NAMES cmake)

add_custom_target(mkspiffs
			COMMAND ${CMAKE} -E copy ${SPIFFS_SDKCONFIGHEADER_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/mkspiffs/include/sdkconfig.h
			COMMAND ${CMAKE} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tools/mkspiffs.makefile ${CMAKE_CURRENT_SOURCE_DIR}/tools/mkspiffs/Makefile
			COMMAND ${CMAKE} -E copy ${SPIFFS_IDF_PATH}/components/spiffs/spiffs/*  ${CMAKE_CURRENT_SOURCE_DIR}/tools/mkspiffs/spiffs/
			COMMAND ${MAKE} -C ${CMAKE_CURRENT_SOURCE_DIR}/tools/mkspiffs/ clean all
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/mkspiffs/
			DEPENDS partition_table
			)

add_custom_target(genspiffsimage ALL
			COMMAND PATH=${CMAKE_CURRENT_SOURCE_DIR}/tools/mkspiffs/ mkspiffs -s ${SPIFFS_PARTITION_SIZE} -c ${SPIFFS_IMAGE_DIR} ${SPIFFS_IMAGE_NAME}
			DEPENDS mkspiffs
			)

set_property(GLOBAL APPEND_STRING PROPERTY ESPTOOL_WRITE_FLASH_ARGS "${SPIFFS_PARTITION_OFFSET}  ${SPIFFS_IMAGE_NAME} ")
esptool_py_flash_project_args(genspiffsimage ${SPIFFS_PARTITION_OFFSET}  ${SPIFFS_IMAGE_NAME} FLASH_IN_PROJECT)
