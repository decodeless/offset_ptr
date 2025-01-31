# Copyright (c) 2025 Pyarelal Knowles, MIT License

# Function to embed a python pretty printer in an object (.o) file and add it as
# a dependency of the given target. See https://www.heuristic42.com/blog/64/.
# Usage: decodeless_embed_pretty_printer(MyTarget printer_a.py printer_b.py)
function(decodeless_embed_pretty_printers target)
  foreach(source IN LISTS ARGN)
    # Generate a *.o filename
    get_filename_component(source_name "${source}" NAME_WLE)
    set(object "${CMAKE_CURRENT_BINARY_DIR}/${source_name}.o")

    # Create a .bin file with a .debug_gdb_scripts section and generate a .o file from it
    add_custom_command(
      OUTPUT "${object}"
      COMMAND
        "$<TARGET_FILE:decodeless_pretty_printer_embed>" "${source}" "${object}.bin" &&
        ${CMAKE_OBJCOPY} -I binary -O elf64-x86-64 --rename-section .data=.debug_gdb_scripts "${object}.bin" "${object}"
      DEPENDS decodeless_pretty_printer_embed "${source}"
      COMMENT "Embedding GDB pretty-printer ${source}")

    # Add the object file as a target dependency
    # TODO: avoid the custom target dance
    target_link_libraries(${target} INTERFACE "${object}")
    add_custom_target("generate_${source_name}" DEPENDS "${object}")
    add_dependencies(${target} "generate_${source_name}")
  endforeach()
endfunction()

# Adding the necessary headers to the .debug_gdb_scripts section is not easy
# with cmake. Much nicer to build a little C++ executable to do the generation
# with no other dependencies.
add_executable(decodeless_pretty_printer_embed "${CMAKE_CURRENT_LIST_DIR}/pretty_printer_embed.cpp")
