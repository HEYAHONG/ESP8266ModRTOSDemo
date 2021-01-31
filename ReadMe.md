# 说明

本工程为使用ESP8266的RTOS_SDK进行编程的Demo。完成一些基础功能以快速开发新应用。

采用SDK：[ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) （使用分支为master，日期：20210131）

## 功能

- 使用保存的WIFI信息作为WIFI STATION连接AP，并可通过串口/smartconfig修改WIFI信息。
- 同步时间
- 使用TCP连接MQTT。修改mqttapp中的事件处理函数即可实现新应用。

# 编译

本工程中不包含任何SDK的源代码文件，因此首先应该正确安装并使用[ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) 。本工程中的编译方式同SDK中example中的工程相同（安装完成后可尝试编译SDK中的example中的工程，成功后再使用cd命令切换到本工程目录编译）。

环境安装正确后可采用以下方式编译（由SDK提供）：

- 直接在工程目录使用make命令编译烧录
- 使用CMAKE生成工程文件，再根据工程类型（CMake支持的工程类型，下面均为个人习惯的类型）进行编译：
  - 默认状态下生成可使用make进行编译的工程。
  - 生成codeblocks工程文件，再使用codeblocks编译（编译之前需确定环境变量设置正确）

根据平台不同，CMake还可支持nijia/Eclipse CDT等类型。





