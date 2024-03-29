#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp8266


include $(IDF_PATH)/make/project.mk


SPIFFS_BIN := $(BUILD_DIR_BASE)/spiffs_image.bin
SPIFFS_DIR := spiffs_image

all_binaries: $(SPIFFS_BIN)

.PHONY:$(SPIFFS_BIN)
$(SPIFFS_BIN) : mkspiffs
	@PATH=tools/mkspiffs mkspiffs -s ${SPIFFS_SIZE} -c ${SPIFFS_DIR} ${SPIFFS_BIN}

#默认采用gcc作为编译器
.PHONY:mkspiffs
mkspiffs: mkspiffs_config
	@make -C tools/mkspiffs CC=gcc CXX=g++  CFLAGS= CPPFLAGS= LDFLAGS= clean all


.PHONY:mkspiffs_config
mkspiffs_config: $(BUILD_DIR_BASE)/include/sdkconfig.h $(PARTITION_TABLE_BIN) 
	@cp -rf $(BUILD_DIR_BASE)/include/sdkconfig.h tools/mkspiffs/include/sdkconfig.h
	@cp -rf tools/mkspiffs.makefile tools/mkspiffs/Makefile
	@cp -rf ESP8266_RTOS_SDK/components/spiffs/spiffs/* tools/mkspiffs/spiffs/
	$(eval SPIFFS_OFFSET := $(shell $(GET_PART_INFO) --partition-table-file $(PARTITION_TABLE_BIN) --partition-table-offset $(PARTITION_TABLE_OFFSET) get_partition_info --partition-type data --partition-subtype spiffs --info offset))
	$(eval SPIFFS_SIZE := $(shell $(GET_PART_INFO) --partition-table-file $(PARTITION_TABLE_BIN) --partition-table-offset $(PARTITION_TABLE_OFFSET) get_partition_info --partition-type data --partition-subtype spiffs --info size))
	$(eval ESPTOOL_ALL_FLASH_ARGS += ${SPIFFS_OFFSET} ${SPIFFS_BIN} )

