#.PHONY:all clean 

#判断是否需要生成调试信息
ifeq ($(DEBUG), true)
GCC = g++ -g
VISION = DEBUG
else
GCC = g++
VISION = RELEASE
endif

#扫描当前目录文件----*c
SRCS = $(wildcard *.cpp)
#定义生成目标文件的名字
OBJS = $(SRCS:.cpp=.o) 
#定义生成的依赖文件的名字
DEPS = $(SRCS:.cpp=.d)
#生成可执行文件BIN的位置和名字
BIN := $(addprefix $(BUILD_ROOT)/,$(BIN))
#定义存放目标文件的目录
LINK_OBJ_DIR = $(BUILD_ROOT)/app/link_obj
#定义存放依赖文件的目录
DEP_DIR = $(BUILD_ROOT)/app/dep


$(shell mkdir -p $(LINK_OBJ_DIR))
$(shell mkdir -p $(DEP_DIR))


#给生成目标文件加上路径
OBJS := $(addprefix $(LINK_OBJ_DIR)/, $(OBJS))
#给依赖文件加上路劲
DEPS := $(addprefix $(DEP_DIR)/, $(DEPS))

#找到目录中的所有.o文件（编译出来的）
LINK_OBJ = $(wildcard $(LINK_OBJ_DIR)/*.o)
#因为构建依赖关系时app目录下这个.o文件还没构建出来，所以LINK_OBJ是缺少这个.o的，我们 要把这个.o文件加进来
LINK_OBJ += $(OBJS)



#入口
all: $(DEPS) $(OBJS) $(BIN)

ifneq ("$(wildcard $(DEPS))","")   #如果不为空,$(wildcard)是函数【获取匹配模式文件名】，这里 用于比较是否为""
include $(DEPS)  
endif


#生成目标文件
$(BIN):$(LINK_OBJ)
	@echo "-----------------------------$(VISION)-------------------------------------------"
	$(GCC) -o $@ $^

#%.o:%.cpp
$(LINK_OBJ_DIR)/%.o:%.cpp
	@echo "构建目标文件"
	$(GCC) -I$(INCLUDE_PATH) -o $@ -c $(filter %.cpp,$^)

#%.d:%.cpp
$(DEP_DIR)/%.d:%.cpp
	@echo "构建依赖文件"
	echo -n $(LINK_OBJ_DIR)/ > $@
	g++ -I$(INCLUDE_PATH) -MM $^ >> $@