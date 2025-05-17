cmake_minimum_required(VERSION 3.16)
set(NAME norman)
project(${NAME} LANGUAGES C ASM)
set(CMAKE_C_FLAGS "-O0")

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

set(SOURCES
        src/syscall-arm64.S
        src/system.c
        src/errno.c
        src/stdio.c
        src/mem.c
        src/string.c
        tests/syscalls.c
)

# set(OBJECT_FILES
#         ${BUILD_DIR}/syscall-arm64.o
#         ${BUILD_DIR}/system.o
#         ${BUILD_DIR}/errno.o
#         ${BUILD_DIR}/stdio.o
#         ${BUILD_DIR}/mem.o
#         ${BUILD_DIR}/string.o
#         ${BUILD_DIR}/syscalls.o
# )

set(TARGET demo)

add_executable(${TARGET}
    ${SOURCES}
)
set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
)

set_target_properties(${TARGET} PROPERTIES C_STANDARD 99)
set_target_properties(${TARGET} PROPERTIES C_STANDARD_REQUIRED ON)
set_target_properties(${TARGET} PROPERTIES CMAKE_WERROR_ON_UNUSABLE_WARNINGS TRUE)

target_compile_options(${TARGET} PRIVATE -nostdinc -nobuiltininc -fno-builtin -ffreestanding)
