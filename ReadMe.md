# 注意

- 推荐直接使用make指令编译,而不使用CMake编译。

# 说明

本工程为使用ESP8266的RTOS_SDK进行编程的Demo。完成一些基础功能以快速开发新应用。

采用SDK：[ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) （使用分支为master，日期：20220220）

## 源代码下载

由于本源代码包含第三方源代码,故直接下载可能有部分源代码缺失，需要通过以下方法解决:

- 在进行git clone 使用--recurse-submodules参数。

- 若已通过git clone下载,则在源代码目录中执行以下命令下载子模块:

  ```bash
   git submodule update --init --recursive
  ```

## 功能

- 使用保存的WIFI信息作为WIFI STATION连接AP，并可通过串口/smartconfig修改WIFI信息。
- 同步时间
- 使用TCP连接MQTT。

# 编译

## 编译环境安装

注意:仅支持github.com下载的代码，其他方式下载的代码需要完全按照官方的说明安装。

### Linux / WSL

执行工程目录下的 bootstrap.sh，当提示 初始化完成时 即表示编译环境安装完成。

### 其它

进入 [ESP8266_RTOS_SDK](ESP8266_RTOS_SDK) 目录,安装官方的说明安装。

## 编译说明

环境安装正确后可采用以下方式编译（由SDK提供）：

- 直接在工程目录使用make命令编译烧录
- 使用CMAKE生成工程文件，再根据工程类型（CMake支持的工程类型，下面均为个人习惯的类型）进行编译：
  - 默认状态下生成可使用make进行编译的工程。
  - 生成codeblocks工程文件，再使用codeblocks编译（编译之前需确定环境变量设置正确）

根据平台不同，CMake还可支持nijia/Eclipse CDT等类型。





