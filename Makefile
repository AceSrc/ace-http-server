NAME = http
INC_DIR += ./include /usr/include
BUILD_DIR ?= ./build
OBJ_DIR ?= $(BUILD_DIR)/obj
BINARY ?= $(BUILD_DIR)/$(NAME)

include Makefile.git

.DEFAULT_GOAL = app

# Compilation flags
CC = g++
LD = g++
INCLUDES = $(addprefix -I, $(INC_DIR))
CFLAGS += -O2 -MMD -Wall -ggdb $(INCLUDES) -DDEBUG --std=c++11

LIB_DIR += -L/usr/lib
LIBS += 
LINKFLAGS = $(LIB_DIR) $(LIBS) -lpthread

# Files to be compiled
SRCS = $(shell find -L src/ -name "*.cpp")
OBJS = $(SRCS:src/%.cpp=$(OBJ_DIR)/%.o)

# Compilation patterns
$(OBJ_DIR)/%.o: src/%.cpp
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

# Depencies
-include $(OBJS:.o=.d)
	
# Rules
.PHONY: app run clean commit reset hardreset tst install toy
tst:
	@echo $(LINKFLAGS)
app: $(BINARY)

# Command to execute RMCV
RUN_ARGS ?=
RMCV_EXEC = $(BINARY) $(RUN_ARGS)

# Generate executable program
$(BINARY): $(OBJS)
	@echo + LD $@
	@$(LD) -O2 -o $@ $^ $(LINKFLAGS)

# Run program
run: $(BINARY)
	@$(RMCV_EXEC)

# Git commit
commit: 
	$(call git_commit, "git_commit", $(msg))

reset: 
	$(shell git reset HEAD^)

hardreset: 
	$(shell git reset --hard HEAD^)

# Clean files
clean: 
	rm -rf $(BUILD_DIR)

toy:
	@echo running flex...
	@flex -o ./toy/token.cpp ./toy/token.l
	@echo running bison...
	@bison -d -o ./toy/parser.cpp ./toy/parser.y
	@echo compiling...
	@g++ ./toy/token.cpp ./toy/parser.cpp ./toy/main.cpp ./toy/fastcgi.cpp -std=c++17 -o ./toy/toy-server
	@echo running...
	@./toy/toy-server
