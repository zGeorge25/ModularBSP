cmake_minimum_required(VERSION 3.10)
project(SatelliteBSP VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to select backend: MOCK (default) or TARGET
set(BSP_BACKEND "MOCK" CACHE STRING "Select BSP Backend (MOCK or TARGET)")

# Include directories
include_directories(src/bsp/include)

# Common sources
set(BSP_SOURCES
    src/main.cpp
)

# Backend selection
if(BSP_BACKEND STREQUAL "MOCK")
    message(STATUS "Building for MOCK backend (Host)")
    add_definitions(-DBSP_BACKEND_MOCK)
    list(APPEND BSP_SOURCES src/bsp/backend/mock/gpio_mock.cpp)
elseif(BSP_BACKEND STREQUAL "TARGET")
    message(STATUS "Building for TARGET backend (ATSAMV71)")
    add_definitions(-DBSP_BACKEND_TARGET)
    list(APPEND BSP_SOURCES src/bsp/backend/target/gpio_target.cpp)
else()
    message(FATAL_ERROR "Invalid BSP_BACKEND. Use MOCK or TARGET.")
endif()

# Main executable
add_executable(satellite_bsp
    ${BSP_SOURCES}
)
