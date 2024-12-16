cmake_minimum_required(VERSION 3.12)

project(MiningSimulation)

set(CMAKE_CXX_STANDARD 17)  # Set C++ standard to C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensure C++ standard is strictly adhered to 
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions

option(BUILD_TESTS "Build unit test source code" OFF)

if(BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()
endif()

# Add simulation library
add_library(MiningSimulation SHARED
    include/Simulation.h
)
set_target_properties(MiningSimulation PROPERTIES LINKER_LANGUAGE CXX)

include_directories(include)

add_executable(mining_simulation "src/main.cpp")

add_subdirectory(extern/spdlog)

target_link_libraries(mining_simulation PRIVATE spdlog::spdlog)

if(BUILD_TESTS)
    # Find all .cpp files in the test source directory
    file(GLOB TEST_SOURCES "test/*.cpp")

    # Add the executable for the unit test using all of the collected source files
    add_executable(unit_tests ${TEST_SOURCES})

    add_test(NAME MiningSimulationTests COMMAND unit_tests)
    set_tests_properties(MiningSimulationTests PROPERTIES LABELS "MiningSimulation")

    # Add GoogleTest submodule to the project
    add_subdirectory(extern/googletest)

    # Link GoogleTest and library to test executable
    target_link_libraries(unit_tests
        gtest_main
        MiningSimulation
        spdlog::spdlog
    )

    # Add test directory to run all tests
    gtest_discover_tests(unit_tests)

    # Specify include directories
    target_include_directories(unit_tests PUBLIC test)
endif()

# Specify include directories
target_include_directories(MiningSimulation PUBLIC include)

# Install the library and headers
install(TARGETS MiningSimulation DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)


