# Copyright (c) 2024 Pyarelal Knowles, MIT License

cmake_minimum_required(VERSION 3.20)

project(offset_ptr)

set(CMAKE_CXX_STANDARD 20)

add_library(offset_ptr INTERFACE)
target_include_directories(offset_ptr INTERFACE include)

if (MSVC_IDE)
  target_sources(offset_ptr INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nodecode/offset_ptr.natvis
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nodecode/offset_span.natvis
  )
endif()

if(BUILD_TESTING)
  option(BUILD_OFFSET_PTR_TESTING "Enable offset_ptr testing" ON)
  if(BUILD_OFFSET_PTR_TESTING)
    enable_testing()
    add_subdirectory(test)
  endif()
endif()
