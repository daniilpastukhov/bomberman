CXX = g++
# path #
SRC_PATH = src
BUILD_PATH = build
LOGIN = bin
BIN_PATH = $(LOGIN)
# executable #
BIN_NAME = bomberman
# extensions #
SRC_EXT = cpp
# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)
# flags #
COMPILE_FLAGS = -std=c++14 -O2 -Wall -pedantic -g -c
# Space-separated pkg-config libraries used by this project
LIBS = -lncurses
.PHONY: default_target
default_target: all
.PHONY: all
all: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
all: dirs
	@$(MAKE) build
	@$(MAKE) doc
.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)
.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)
	@$(RM) -r doc
# checks the executable and symlinks to the output
.PHONY: doc
	doxygen Doxyfile
.PHONY: build
build: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)
# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@ $(LIBS)
# Add dependency files, if they exist
-include $(DEPS)
# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) -MP -MMD -c $< -o $@
.PHONY: run
run:
	$(LOGIN)/$(BIN_NAME)
.PHONY: doc
doc:
	doxygen Doxyfile
.PHONY: compile
compile: dirs
	@$(MAKE) build

