#定义需要编译的目录

#当前目录的绝对路径
export BUILD_ROOT = $(shell pwd)

#头文件的绝对路径
export INCLUDE_PATH = $(BUILD_ROOT)/_include

#需要编译的目录
BUILD_DIR = $(BUILD_ROOT)/signal/ \
			   $(BUILD_ROOT)/logs/ \
			   $(BUILD_ROOT)/net/ \
			   $(BUILD_ROOT)/proc/ \
			   $(BUILD_ROOT)/app/ 
			   

#编译是否生成调试信息
export DEBUG = true