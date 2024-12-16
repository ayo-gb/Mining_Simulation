# Project settings
BUILD_DIR = build
RESULTS_DIR = results
PROJECT_NAME = MiningSimulation
CMAKE_FLAGS_RELEASE = -DCMAKE_BUILD_TYPE=Release
CMAKE_FLAGS_DEBUG = -DCMAKE_BUILD_TYPE=Debug
TEST_FLAG_ON = -DBUILD_TESTS=ON
TEST_FLAG_OFF = -DBUILD_TESTS=OFF

# Default target: build the project
all: build

# Build the project
build:
	@echo "Building $(PROJECT_NAME)..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS_RELEASE) $(TEST_FLAG_OFF) .. && make

# Run unit tests (only if enabled)
test:
	@echo "Building $(PROJECT_NAME) and Running Unit Tests..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS_DEBUG) $(TEST_FLAG_ON) .. && make && ctest

# Clean up the build directory
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(RESULTS_DIR)