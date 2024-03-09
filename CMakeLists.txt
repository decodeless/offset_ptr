# Copyright (c) 2024 Pyarelal Knowles, MIT License

cmake_minimum_required(VERSION 3.20)

project(decodeless_offset_ptr)

set(CMAKE_CXX_STANDARD 20)

add_library(decodeless_offset_ptr INTERFACE)
target_include_directories(decodeless_offset_ptr INTERFACE include)

if(MSVC_IDE)
  target_sources(decodeless_offset_ptr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/decodeless/offset_ptr.natvis
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/include/decodeless/offset_span.natvis)
endif()

add_library(decodeless::offset_ptr ALIAS decodeless_offset_ptr)

if(BUILD_TESTING)
  option(BUILD_DECODELESS_TESTING "Enable decodeless testing" ON)
  if(BUILD_DECODELESS_TESTING)
    enable_testing()
    add_subdirectory(test)
  endif()
endif()
