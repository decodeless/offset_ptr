# Copyright (c) 2024-2025 Pyarelal Knowles, MIT License

cmake_minimum_required(VERSION 3.20)

project(decodeless_offset_ptr)

set(CMAKE_CXX_STANDARD 20)

add_library(decodeless_offset_ptr INTERFACE)
target_include_directories(decodeless_offset_ptr INTERFACE include)

if(MSVC_IDE)
  # Natvis for debugging in visual studio
  target_sources(
    decodeless_offset_ptr
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/debugging/offset_ptr.natvis"
              "${CMAKE_CURRENT_SOURCE_DIR}/debugging/offset_span.natvis")
else()
  # Python pretty printers for linux debugging
  option(DECODELESS_EMBED_PRETTY_PRINTERS
        "Embed python pretty printers in .debug_gdb_scripts sections" ON)
  if(DECODELESS_EMBED_PRETTY_PRINTERS)
    include(debugging/pretty_printer_embed.cmake)
    decodeless_embed_pretty_printers(
      decodeless_offset_ptr
      "${CMAKE_CURRENT_SOURCE_DIR}/debugging/offset_ptr_pretty_printer.py"
      "${CMAKE_CURRENT_SOURCE_DIR}/debugging/offset_span_pretty_printer.py")
  endif()
endif()

add_library(decodeless::offset_ptr ALIAS decodeless_offset_ptr)

if(BUILD_TESTING)
  option(BUILD_DECODELESS_TESTING "Enable decodeless testing" ON)
  if(BUILD_DECODELESS_TESTING)
    enable_testing()
    add_subdirectory(test)
  endif()
endif()
