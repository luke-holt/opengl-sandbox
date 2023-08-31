CC = clang
CXX = clang++

INCLUDE_DIR = inc
SOURCE_DIR = src
BUILD_DIR = build
OBJECT_DIR = $(BUILD_DIR)/obj

CFLAGS = -Wall $(addprefix "-I", $(INCLUDE_DIR))
CXXFLAGS = -Wall $(addprefix "-I", $(INCLUDE_DIR))
LDFLAGS = -lglfw -lX11 -lGL

COMMON_SOURCES = glad.c
COMMON_OBJECT_NAMES = $(patsubst %.c, %.o, $(COMMON_SOURCES))
COMMON_OBJECTS = $(addprefix $(OBJECT_DIR)/, $(COMMON_OBJECT_NAMES))

TARGETS = triangle

# Compiling
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $^"
	@$(CC) $(CFLAGS) -c $^ -o $@

triangle: $(COMMON_OBJECTS) $(OBJECT_DIR)/triangle.o
	@mkdir -p $(BUILD_DIR)
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^

.PHONY: clean

clean:
	@rm $(BUILD_DIR)/$(TARGETS) $(OBJECT_DIR)/*

