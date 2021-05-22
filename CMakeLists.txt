# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.8)

if(WIN32)
    set(AWSSDK_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\cmake\\AWSSDK")
    set(aws-cpp-sdk-core_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\cmake\\aws-cpp-sdk-core")
    set(aws-crt-cpp_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-crt-cpp\\cmake")
    set(aws-c-http_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-http\\cmake")
    set(aws-c-io_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-io\\cmake")
    set(aws-c-common_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-common\\cmake")
    set(aws-c-cal_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-cal\\cmake")
    set(aws-c-compression_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-compression\\cmake")
    set(aws-c-mqtt_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-mqtt\\cmake")
    set(aws-c-auth_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-auth\\cmake")
    set(aws-c-event-stream_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-event-stream\\cmake")
    set(aws-c-s3_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-c-s3\\cmake")
    set(aws-checksums_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\aws-checksums\\cmake")
    set(aws-cpp-sdk-s3_DIR "C:\\Program Files (x86)\\aws-cpp-sdk-all\\lib\\cmake\\aws-cpp-sdk-s3")
endif()

# Set this project's name.
project("encoding-app")

# Set the C++ standard to use to build this target.
set(CMAKE_CXX_STANDARD 11)

# Build shared libraries by default.
if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

# Find the AWS SDK for C++ package.
find_package(AWSSDK REQUIRED COMPONENTS s3)
set(SERVICE s3)

# If the compiler is some version of Microsoft Visual C++, or another compiler simulating C++, 
# and building as shared libraries, then dynamically link to those shared libraries.
if(MSVC AND BUILD_SHARED_LIBS)
    add_definitions(-DUSE_IMPORT_EXPORT)
    # Copy relevant AWS SDK for C++ libraries into the current binary directory for running and debugging.
    list(APPEND SERVICE_LIST s3)
    AWSSDK_CPY_DYN_LIBS(SERVICE_LIST "" ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Add the code example-specific header files.
file(GLOB AWSDOC_S3_HEADERS
    "include/awsdoc/s3/*.h"
)

# Add the code example-specific source files.
file(GLOB AWSDOC_S3_SOURCE
    "*.cpp"
)

# Check whether the target system is Windows, including Win64.
if(WIN32)
    # Check whether the compiler is some version of Microsoft Visual C++, or another compiler simulating C++.
    if(MSVC)
        source_group("Header Files\\awsdoc\\s3" FILES ${AWSDOC_S3_HEADERS})
        source_group("Source Files" FILES ${AWSDOC_S3_SOURCE})
    endif(MSVC)
endif()

foreach(file ${AWSDOC_S3_SOURCE})
    get_filename_component(EXAMPLE ${file} NAME_WE)

    # Build the code example executables.
    set(EXAMPLE_EXE run_${EXAMPLE})

    add_executable(${EXAMPLE_EXE} ${AWSDOC_S3_HEADERS} ${file})

    if(MSVC AND BUILD_SHARED_LIBS)
        target_compile_definitions(${EXAMPLE_EXE} PUBLIC "USE_IMPORT_EXPORT")
        target_compile_definitions(${EXAMPLE_EXE} PRIVATE "AWSDOC_S3_EXPORTS")
    endif()

    target_include_directories(${EXAMPLE_EXE} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
    target_link_libraries(${EXAMPLE_EXE} ${AWSSDK_LINK_LIBRARIES} 
        ${AWSSDK_PLATFORM_DEPS})

endforeach()
