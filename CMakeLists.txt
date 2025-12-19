cmake_minimum_required(VERSION 3.14)
project(OLanguage VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# PROJECT CONFIGURATION
# ============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# COMPILER FLAGS
# ============================================================================

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wconversion
        -Wshadow
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /W4
        /WX
    )
endif()

# ============================================================================
# HEADER-ONLY LIBRARY
# ============================================================================

add_library(o INTERFACE)
target_include_directories(o INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# C++17 requirement
target_compile_features(o INTERFACE cxx_std_17)

# ============================================================================
# EXAMPLES
# ============================================================================

option(O_BUILD_EXAMPLES "Build examples" ON)

if(O_BUILD_EXAMPLES)
    add_executable(example_basic examples/basic.cpp)
    target_link_libraries(example_basic PRIVATE o)
    
    # Add more examples as they are created
    # add_executable(example_quantum examples/quantum.cpp)
    # target_link_libraries(example_quantum PRIVATE o)
endif()

# ============================================================================
# TESTS
# ============================================================================

option(O_BUILD_TESTS "Build tests" ON)

if(O_BUILD_TESTS)
    enable_testing()
    
    # Find or download GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
    
    # Test executables
    add_executable(test_o_state tests/test_o_state.cpp)
    target_link_libraries(test_o_state PRIVATE o gtest gtest_main)
    
    add_executable(test_logic tests/test_logic.cpp)
    target_link_libraries(test_logic PRIVATE o gtest gtest_main)
    
    add_executable(test_channel tests/test_channel.cpp)
    target_link_libraries(test_channel PRIVATE o gtest gtest_main)
    
    # Register tests with CTest
    add_test(NAME O_State COMMAND test_o_state)
    add_test(NAME Logic COMMAND test_logic)
    add_test(NAME Channel COMMAND test_channel)
endif()

# ============================================================================
# BENCHMARKS
# ============================================================================

option(O_BUILD_BENCHMARKS "Build benchmarks" OFF)

if(O_BUILD_BENCHMARKS)
    # Find or download Google Benchmark
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG        v1.8.3
    )
    FetchContent_MakeAvailable(benchmark)
    
    add_executable(bench_o_state benchmarks/bench_o_state.cpp)
    target_link_libraries(bench_o_state PRIVATE o benchmark::benchmark)
endif()

# ============================================================================
# INSTALLATION
# ============================================================================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install headers
install(DIRECTORY include/o
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

# Install targets
install(TARGETS o
        EXPORT OLanguageTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Generate and install CMake config files
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/OLanguageConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/OLanguageConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/OLanguageConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OLanguage
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OLanguageConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OLanguageConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OLanguage
)

install(EXPORT OLanguageTargets
        FILE OLanguageTargets.cmake
        NAMESPACE O::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OLanguage
)

# ============================================================================
# PACKAGING
# ============================================================================

set(CPACK_PACKAGE_NAME "OLanguage")
set(CPACK_PACKAGE_VENDOR "Jean-Christophe Ané")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "O — Universal Multi-State Extension of C++")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(CPack)

# ============================================================================
# PRINT CONFIGURATION
# ============================================================================

message(STATUS "")
message(STATUS "O Language Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Examples: ${O_BUILD_EXAMPLES}")
message(STATUS "  Build Tests: ${O_BUILD_TESTS}")
message(STATUS "  Build Benchmarks: ${O_BUILD_BENCHMARKS}")
message(STATUS "")
