cmake_minimum_required(VERSION 3.19)
project(pear-desktop)

# platform checks
if (UNIX AND !APPLE)
    set(LINUX true)
elseif (WIN32 AND !MSVC)
    message(FATAL_ERROR "Win32 only supported with MSVC tools")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PEAR_COMPILE_FEATURES cxx_std_20)
set(PEAR_COMPILER_DEFINITIONS -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

if (APPLE)
    list(APPEND PEAR_COMPILER_DEFINITIONS -DPEAR_MAC)
elseif (LINUX)
    list(APPEND PEAR_COMPILER_DEFINITIONS -DPEAR_LINUX)
elseif (WIN32)
    list(APPEND PEAR_COMPILER_DEFINITIONS -DPEAR_WIN -DUNICODE -DWIN32_LEAN_AND_MEAN)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    list(APPEND PEAR_COMPILER_DEFINITIONS -DPEAR_DEBUG)
endif ()

# definitions cannot be added per target with target_compile_definitions
# because it won't be applied to a target's target_include_directories headers
# add_definitions here will be added to all targets and their include directories
add_definitions(${PEAR_COMPILER_DEFINITIONS})

add_subdirectory(bin/pear_cli)
add_subdirectory(modules/net_tcp)
add_subdirectory(modules/screen_capture)
