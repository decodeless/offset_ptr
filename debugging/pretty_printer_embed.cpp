#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>

// Tiny program to dump python source code for a .debug_gdb_scripts section into
// a binary file that can then be consume by objcopy. It just adds some header
// bits and a null terminator.
// Usage: ./exe <python_script.py> <output.bin>
// See https://sourceware.org/gdb/current/onlinedocs/gdb.html/dotdebug_005fgdb_005fscripts-section.html
// SECTION_SCRIPT_ID_PYTHON_FILE = 1
// SECTION_SCRIPT_ID_SCHEME_FILE = 3
// SECTION_SCRIPT_ID_PYTHON_TEXT = 4 <--- source code
// SECTION_SCRIPT_ID_SCHEME_TEXT = 6
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: ./embed <input.py> <output.bin>\n";
        return EXIT_FAILURE;
    }

    std::ifstream src(argv[1]);
    if (!src.good()) {
        std::cerr << "Error: could not open input file '" << argv[1] << "'\n";
        return EXIT_FAILURE;
    }

    std::ofstream dst(argv[2], std::ios::binary);
    if (!dst.good()) {
        std::cerr << "Error: could not open output file '" << argv[2] << "'\n";
        return EXIT_FAILURE;
    }

    std::string name = std::filesystem::path(argv[1]).filename().string();
    dst.write("\x04", 1); // "python text" identifier byte
    dst << name << "\n";  // first line is the name of the script
    dst << src.rdbuf();   // inline the script
    dst.write("\0", 1);   // null terminated
    return EXIT_SUCCESS;
}