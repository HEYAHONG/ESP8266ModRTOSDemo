# 注意

- 推荐直接使用make指令编译,而不使用CMake编译。

# 说明

本工程为使用ESP8266的RTOS_SDK进行编程的Demo。完成一些基础功能以快速开发新应用。

采用SDK：[ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) 

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

## 资源文件

类似于桌面程序的资源文件。源代码实现的目录为 [main/rc](main/rc/)。

在固件编写中，很多时候需要大量的固定数据，直接手工嵌入到C文件里比较麻烦。

通过读取文件转换到对应C文件可大大节省时间，可添加常用的文件（如各种证书）或者不适宜放在可读写的文件系统中的文件(如需要在格式化中保留或者初始参数)。转换程序源代码为[main/rc/fsgen.cpp](main/rc/fsgen.cpp)。

使用步骤如下:

- 将待添加的文件放入 main/rc/fs目录下。

- 使用文件名调用以下函数(需包含相应头文件RC.h):

  ```c++
  //通过名称获取资源大小
  size_t RCGetSize(const char * name);
  
  //通过名称获取资源指针
  const unsigned char * RCGetHandle(const char * name);
  ```

## SPIFFS

 spiffs是一个用于嵌入式目标上的SPI NOR flash设备的文件系统。挂载spiffs后可直接以文件的形式进行数据的存储(可读可写)。

### 预置的镜像

可预先将一些文件放入文件系统,可直接在挂载后访问(可读可写)。

目录:[spiffs_image](spiffs_image)

### 挂载

默认情况下,挂载的目录为/spiffs,具体参见 [main/init.c](main/init.c)
### 镜像生成工具

采用了mkspiffs作为镜像生成工具且使用源代码编译,故需要安装make、gcc、g++等工具。

注意:修改spiffs的Kconfig参数后需要检验是否能够正常读取,有些参数不能正常工作。



# 编译

## 编译环境安装

注意:仅支持github.com下载的代码，其他方式下载的代码需要完全按照官方的说明安装。

### Linux / WSL

执行工程目录下的 bootstrap.sh，当提示 初始化完成时 即表示编译环境安装完成。

注意: bootstrap.sh主要用于进入编译环境,执行bootstrap.sh需保证当前环境能正常使用ESP8266_RTOS_SDK编译例程。每次打开终端后，如需进行工程编译,就需要执行 bootstrap.sh,若未关闭终端,无需重复执行 bootstrap.sh 。

### 其它

进入 [ESP8266_RTOS_SDK](ESP8266_RTOS_SDK) 目录,安装官方的说明安装。

## 编译说明

环境安装正确后可采用以下方式编译（由SDK提供）：

- 直接在工程目录使用make命令编译烧录
- 使用CMAKE生成工程文件，再根据工程类型（CMake支持的工程类型，下面均为个人习惯的类型）进行编译：
  - 默认状态下生成可使用make进行编译的工程。
  - 生成codeblocks工程文件，再使用codeblocks编译（编译之前需确定环境变量设置正确）

根据平台不同，CMake还可支持nijia/Eclipse CDT等类型。





