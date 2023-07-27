# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread

# Directories
SRC_DIR := src
LIB_DIR := lib
OBJ_DIR := obj
BIN_DIR := bin

# File names
LIB_NAME := libfilesearch.a
APP_NAME := file_search_app

# Source and object files
LIB_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB_SOURCES))
APP_SOURCE := main.cpp
APP_OBJECT := $(OBJ_DIR)/main.o

# Targets
all: $(BIN_DIR)/$(APP_NAME)

$(BIN_DIR)/$(APP_NAME): $(APP_OBJECT) $(LIB_DIR)/$(LIB_NAME)
	$(CXX) $(CXXFLAGS) -L$(LIB_DIR) -o $@ $< -lfilesearch

$(APP_OBJECT): $(APP_SOURCE)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_DIR)/$(LIB_NAME): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

.PHONY: all clean
