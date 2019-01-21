CXX ?= g++

INCLUDES := 
INCLUDES += $(subst -I,-isystem,$(shell pkg-config --cflags cairo))
INCLUDES += $(subst -I,-isystem,$(shell pkg-config --cflags sdl2))

CFLAGS   := -std=c++17 -Wall -Wextra -Werror -pedantic -g
D_CFLAGS := -Og -DDEBUG
R_CFLAGS := -O3 -DNDEBUG

LFLAGS   := 
LFLAGS   += $(shell pkg-config --libs cairo)
LFLAGS   += $(shell pkg-config --libs sdl2)
D_LFLAGS :=
R_LFLAGS :=

SRC_PATH   := src

debug:   export BUILD_PATH := build/debug
debug:   export BIN_PATH   := bin/debug
release: export BUILD_PATH := build/release
release: export BIN_PATH   := bin/release

SOURCES  = $(shell find $(SRC_PATH) -name *.cpp)
OBJECTS  = $(SOURCES:$(SRC_PATH)/%.cpp=$(BUILD_PATH)/%.o)
DEPS     = $(OBJECTS:%.o=%.d)
BIN      = $(BIN_PATH)/six

debug:   export C_FLAGS := $(CFLAGS) $(D_CFLAGS)
debug:   export L_FLAGS := $(LFLAGS) $(D_LFLAGS)
release: export C_FLAGS := $(CFLAGS) $(R_CFLAGS)
release: export L_FLAGS := $(LFLAGS) $(R_LFLAGS)

debug: dirs
	$(MAKE) -k --no-print-directory all

release: dirs
	$(MAKE) -k --no-print-directory all

all: $(BIN)
	ln -sf $(BIN) six

.PHONY: dirs
dirs:
	mkdir -p $(dir $(OBJECTS))
	mkdir -p $(dir $(BIN))

.PHONY: clean
clean:
	rm -rf build bin
	rm -rf six

-include $(DEPS)

$(BIN): $(OBJECTS)
	$(CXX) -o $@ $^ $(L_FLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) $(C_FLAGS) $(INCLUDES) -MMD -o $@ -c $<
