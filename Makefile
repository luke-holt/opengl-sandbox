CC = clang
CXX = clang++

INCLUDE_DIR = inc
SOURCE_DIR = src
BUILD_DIR = build
OBJECT_DIR = $(BUILD_DIR)/obj

WARNINGS = -Wall

DEBUG_FLAG = -g

CFLAGS = $(WARNINGS) $(addprefix "-I", $(INCLUDE_DIR)) $(DEBUG_FLAG)
CXXFLAGS = $(WARNINGS) $(addprefix "-I", $(INCLUDE_DIR)) $(DEBUG_FLAG)
LDFLAGS = -lglfw -lX11 -lGL -lm $(DEBUG_FLAG)

COMMON_SOURCES = glad.c shader.c util.c
COMMON_OBJECT_NAMES = $(patsubst %.c, %.o, $(COMMON_SOURCES))
COMMON_OBJECTS = $(addprefix $(OBJECT_DIR)/, $(COMMON_OBJECT_NAMES))

TARGETS = triangle shaders textures
TARGET_PATHS = $(addprefix $(BUILD_DIR)/, $(TARGETS))

# Compiling
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $^"
	@$(CC) $(CFLAGS) -c $^ -o $@

triangle: $(COMMON_OBJECTS) $(OBJECT_DIR)/triangle.o
	@mkdir -p $(BUILD_DIR)
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^

shaders: $(COMMON_OBJECTS) $(OBJECT_DIR)/shaders.o
	@mkdir -p $(BUILD_DIR)
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^

textures: $(COMMON_OBJECTS) $(OBJECT_DIR)/textures.o
	@mkdir -p $(BUILD_DIR)
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^

.PHONY: clean

clean:
	@rm $(TARGET_PATHS) $(OBJECT_DIR)/*

