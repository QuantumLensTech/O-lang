cmake_minimum_required(VERSION 3.15)
project(O_Language VERSION 1.0 LANGUAGES CXX)

# ═══════════════════════════════════════════════════════════════════════════
# C++ Standard
# ═══════════════════════════════════════════════════════════════════════════

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ═══════════════════════════════════════════════════════════════════════════
# Header-Only Library
# ═══════════════════════════════════════════════════════════════════════════

add_library(o INTERFACE)
target_include_directories(o INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ═══════════════════════════════════════════════════════════════════════════
# Examples
# ═══════════════════════════════════════════════════════════════════════════

option(O_BUILD_EXAMPLES "Build O examples" ON)

if(O_BUILD_EXAMPLES)
    add_executable(hello_multistate examples/basic/hello_multistate.cpp)
    target_link_libraries(hello_multistate PRIVATE o)
endif()

# ═══════════════════════════════════════════════════════════════════════════
# Installation
# ═══════════════════════════════════════════════════════════════════════════

install(DIRECTORY include/o DESTINATION include)
install(TARGETS o EXPORT OTargets)
install(EXPORT OTargets
    FILE OConfig.cmake
    NAMESPACE O::
    DESTINATION lib/cmake/O
)
