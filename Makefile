# --- Variables ---

CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -I./src -I./include -I$(LIB_DIR)
LDFLAGS =

# Project naming and directories
TARGET = LOTEB
SRC_DIR = src
BIN_DIR = build/bin
OBJ_DIR = build/obj
LIB_DIR = build/lib
SAVES_DIR = $(BIN_DIR)/saves

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

.PHONY: all
all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJECTS) | $(BIN_DIR) $(SAVES_DIR)
	@echo "-> Linking $(TARGET) into $(BIN_DIR)/"
	$(CC) $(LDFLAGS) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) $(LIB_DIR)
	@echo "-> Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Targets 
$(BIN_DIR):
	@echo "Creating directory: $(BIN_DIR)"
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@echo "Creating directory: $(OBJ_DIR)"
	@mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	@echo "Creating directory: $(LIB_DIR)"
	@mkdir -p $(LIB_DIR)

$(SAVES_DIR):
	@echo "Creating directory: $(SAVES_DIR)"
	@mkdir -p $(SAVES_DIR)

# Clean target: removes the entire build directory
.PHONY: clean
clean:
	@echo "Cleaning up generated files (removing build/ directory)..."
	@rm -rf build/

# Rebuild target: clean and then build
.PHONY: rebuild
rebuild: clean all