#!/bin/bash

#检查工具是否存在,$1为待检查的工具名。
function CheckTool
{
	[  -n "$1"  ] ||
	{
		echo -e  "\033[41;37mCheckTool 参数错误!!\033[40;37m";
		return 255;
	};
	ToolPath=`which $1`;
	[ -e "$ToolPath" ] ||
	{
		 echo -e "\033[41;37m$1 不存在，请先安装此工具\033[40;37m";
		 return 255;
	};
	return 0;
}

#检查必要的工具
CheckTool git
[ $? -eq 0 ] || exit;
CheckTool install
[ $? -eq 0 ] || exit;
CheckTool patch
[ $? -eq 0 ] || exit;
CheckTool find
[ $? -eq 0 ] || exit;
CheckTool dirname
[ $? -eq 0 ] || exit;
CheckTool readlink
[ $? -eq 0 ] || exit;
CheckTool ln
[ $? -eq 0 ] || exit;
CheckTool sed
[ $? -eq 0 ] || exit;
CheckTool python
[ $? -eq 0 ] || exit;
#设置ROOT_PATH变量

self_path=""

# shellcheck disable=SC2128  # ignore array expansion warning
if [ -n "${BASH_SOURCE-}" ]
then
self_path="${BASH_SOURCE}"
elif [ -n "${ZSH_VERSION-}" ]
then
self_path="${(%):-%x}"
else
echo -e "\033[41;37m不能获取工作目录\033[40;37m";
return 1
fi

# shellcheck disable=SC2169,SC2169,SC2039  # unreachable with 'dash'
if [[ "$OSTYPE" == "darwin"* ]]; then
# convert possibly relative path to absolute
script_dir="$(realpath_int "${self_path}")"
# resolve any ../ references to make the path shorter
script_dir="$(cd "${script_dir}" || exit 1; pwd)"
else
# convert to full path and get the directory name of that
script_name="$(readlink -f "${self_path}")"
script_dir="$(dirname "${script_name}")"
fi

ROOT_PATH="${script_dir}";
SDK_PATH=${ROOT_PATH}/ESP8266_RTOS_SDK/

echo -e  "\033[44;37m当前目录:${ROOT_PATH}\033[40;37m";

python -m pip install -U pip || python  ${ROOT_PATH}/get-pip.py
python -m pip install -U virtualenv

echo -e  "\033[44;37m正在初始化SDK环境\033[40;37m";
# 更换下载地址
export IDF_GITHUB_ASSETS="dl.espressif.com/github_assets"
cd ${SDK_PATH}
function InstallFailure
{
	echo -e "初始化失败,请手动安装SDK开发环境。"
	exit;
}
./install.sh
[ $? -eq 0 ] || InstallFailure
source ./export.sh
cd ${ROOT_PATH}

echo -e  "\033[44;37mSDK环境初始化完毕!\033[40;37m";
${SHELL}