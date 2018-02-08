################# user config #################
#											  
#											  
###############################################
PROJECT_NAME=c_tool
TARGET=main
CXX=g++
CXX_TYPE= cpp c cc
HEADER_TYPE= h hpp
# 控制指令回显的选项
NECHO=@
# .o目标文件的目标路径
BUILD=./build
$(shell mkdir -p $(BUILD))
OBJ_DIR=$(BUILD)/obj
$(shell mkdir -p $(OBJ_DIR))
COMPILE_SHELL=compile_shell

# 静态库 和 动态库
STAITIC_LIBS=$(BUILD)/$(PROJECT_NAME).a
DYNAMIC_LIBS=$(BUILD)/lib$(PROJECT_NAME).so

# 寻找当前目录下所有含有.cpp和.c源文件的目录
CXX_FIND_ARG=$(shell /bin/bash $(COMPILE_SHELL)/f_find_type.sh $(CXX_TYPE))
HEADER_FIND_ARG=$(shell /bin/bash $(COMPILE_SHELL)/f_find_type.sh $(HEADER_TYPE))
VPATH=. $(shell find * -type d -exec bash -c "find {} -maxdepth 1 \( $(CXX_FIND_ARG) \) | grep -q ." \; -print)

# 寻找当前目录下所有含有.h和.hpp头文件的目录
INCLUDE_DIRS=. ./include $(shell find * -type d -exec bash -c "find {} -maxdepth 1 \( $(HEADER_FIND_ARG) \) | grep -q ." \; -print)
# 头文件的编译选项
INC=$(addprefix -I,$(INCLUDE_DIRS))
# 所有.cpp/.c/.cc源文件的相对路径
SRC_CXX=$(foreach t,$(CXX_TYPE),$(foreach dir,$(VPATH),$(wildcard $(dir)/*.$(t))))
# 目标文件的相对路径
#OBJS=$(addprefix $(OBJ_DIR)/,$(subst .cpp,.o,$(notdir $(SRC_CXX)))) \
#	$(addprefix $(OBJ_DIR)/,$(subst .c,.o,$(notdir $(SRC_C)))) \
#	$(addprefix $(OBJ_DIR)/,$(subst .cc,.o,$(notdir $(SRC_CC))))
OBJS=$(foreach t,$(CXX_TYPE),$(addprefix $(OBJ_DIR)/,$(subst .$(t),.o,$(notdir $(foreach dir,$(VPATH),$(wildcard $(dir)/*.$(t)))))))

################ compile config ###############
#											  
#											  
###############################################
CXXFLAGS=-ggdb -fPIC -Wall -pedantic $(INC)
TARGETFLAGS=$(CXXFLAGS) -L. -L$(BUILD) $(DYNAMIC_LIBS)

################# compile rule ################
#											  
#											  
###############################################
all: message object static_libs dynamic_libs target

message:
	@echo ">>>>>>>>>> message <<<<<<<<<<\n"
	@echo "\033[31m<CXX_FIND_ARG>\033[0m : "$(CXX_FIND_ARG)
	@echo "\033[31m<HEADER_FIND_ARG>\033[0m : "$(HEADER_FIND_ARG)
	@echo "\033[31m<VPATH>\033[0m : "$(VPATH)
	@echo "\033[31m<CXXFLAGS>\033[0m : "$(CXXFLAGS)
	@echo "\033[31m<SRC_CXX>\033[0m : "$(SRC_CXX)
	@echo "\033[31m<OBJS>\033[0m : "$(OBJS)
	@echo "\n>>>>>>>>>> compiling <<<<<<<<<<\n"

target: $(TARGET)

object: $(OBJS)

static_libs: $(STAITIC_LIBS)

dynamic_libs: $(DYNAMIC_LIBS)

$(STAITIC_LIBS): $(OBJS)
	$(NECHO)$(AR) $(ARFLAGS) $@ $(OBJS)

$(DYNAMIC_LIBS): $(OBJS)
	@echo $(CXX)": 正在创建" $(DYNAMIC_LIBS)
	$(NECHO)$(CXX) -shared -o $@ $(OBJS)
	@echo "\033[32m<so> - " $(DYNAMIC_LIBS)"\033[0m"

$(TARGET): $(OBJ_DIR)/$(TARGET).o
	$(NECHO)$(CXX) -o $@ $^ $(TARGETFLAGS) 
	@echo "\033[32m<target> - " $(TARGET)"\033[0m"

$(OBJ_DIR)/%.o: %.cpp
	$(NECHO)$(CXX) -o $@ -c $< $(CXXFLAGS)
	@echo "\033[32m<o> - " $@"\033[0m"

$(OBJ_DIR)/%.o: %.c
	$(NECHO)$(CXX) -o $@ -c $< $(CXXFLAGS)
	@echo "\033[32m<o> - " $@"\033[0m"

$(OBJ_DIR)/%.o: %.cc
	$(NECHO)$(CXX) -o $@ -c $< $(CXXFLAGS)
	@echo "\033[32m<o> - " $@"\033[0m"

clean:
	$(NECHO)-rm -rf $(BUILD) $(TARGET)
	@echo ">>>>>>>>>> clean <<<<<<<<<<"
