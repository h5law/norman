cmake_minimum_required(VERSION 3.16)
set(NAME norman)
project(${NAME} LANGUAGES C ASM)

set(CLANG_CLANG clang)
set(CMAKE_C_COMPILER ${CLANG_CLANG})
set(CMAKE_ASM_COMPILER ${CLANG_CLANG})
# if(CLANG_CLANG)
#         message(STATUS "Found clang at: ${CLANG_CLANG}")
# else()
#         message(FATAL_ERROR "Clang not found.  Please ensure it's installed and in
# your PATH.")
# endif()

# Unset environment variables
unset(ENV{CFLAGS})
unset(ENV{LDFLAGS})
unset(ENV{LIBRARY_PATH})
unset(ENV{C_INCLUDE_PATH})

# Clear any cached compiler flags
set(CMAKE_C_FLAGS "" CACHE STRING "C compiler flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "Linker flags" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS "" CACHE STRING "Shared linker flags" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS "" CACHE STRING "Module linker flags" FORCE)

# Prevent CMake from automatically adding include or library paths
set(CMAKE_SYSTEM_INCLUDE_PATH "")
set(CMAKE_SYSTEM_LIBRARY_PATH "")

# Ensure no external libraries or includes are added implicitly
set(CMAKE_NO_SYSTEM_FROM_IMPORTED TRUE)

# Optionally, set the compiler to operate in a minimal mode
add_compile_options(-nostdinc -nobuiltininc -nostdlib -fno-builtin -ffreestanding -fno-stack-protector)

# Build type
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug)
endif()

# Architecture string.
set(ARCH "${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "Override CPU architecture")
string(REGEX REPLACE "aarch" "arm" KERNEL_ARCH "${ARCH}")

if("${KERNEL_ARCH}" STREQUAL "arm64")
        set(ARCH "arm64")
else()
        message(FATAL_ERROR "Unsupported ARCH: ${KERNEL_ARCH}")
endif()

# Set directory locations (allowing us to move directories easily)
set(BUILD_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}-${ARCH})
file(MAKE_DIRECTORY ${BUILD_DIR})

add_executable(demo
        src/syscall-arm64.S
        src/system.c
        src/errno.c
        src/stdio.c
        src/mem.c
        src/string.c
        tests/syscalls.c
)
