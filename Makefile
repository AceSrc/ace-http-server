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
LINKFLAGS = $(LIB_DIR) $(LIBS)

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
.PHONY: app run clean commit reset hardreset tst install
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

