# decodeless::offset_ptr

`offset_ptr` is a self-relative pointer, encapsulated with C++:

```cpp
T* operator->() const { return (T*)((char*)this + m_offset); }
```

This is a tiny header-only C++ library. The humble offset pointer is a relative
address from its own address in memory rather than an absolute address. It is
address space independent. As long as the thing it points to is mapped or copied
within the same address range it will still work. Quite useful if you're...

- Saving a data structure to disk
- Better yet, memory mapping a file from disk
- Sharing memory between processes
- Copying complex data structures to the GPU
- Duplicating the raw memory of a data structure, e.g. a low overhead deep copy

A tiny offset span class is provided too, which is just an offset pointer and
size. `offset_span` is very similar to `std::span` but with a relative address.

Part of the [`decodeless`](https://github.com/decodeless) collection of utility
libraries for conveniently reading and writing files via memory mapping.

**Example: offset_ptr**

```cpp
#include <decodeless/offset_ptr.hpp>

using namespace decodeless;

...

struct File {
    offset_ptr<int> ptr;
    int data;
};

File file;
file.data = 42;
file.ptr =
    &file.data; // intialize the offset pointer from a regular pointer

// Duplicate the memory. file2's ptr then implicitly points to file2's data
File file2;
std::memcpy(&file2, &file, sizeof(file));
EXPECT_TRUE(file2.ptr == &file2.data);
EXPECT_TRUE(*file2.ptr == 42);

// Pointer data is mutable
*file2.ptr = 21;
EXPECT_TRUE(file2.data == 21);

// Prove we weren't accidentally modifying the original file
EXPECT_TRUE(file.data == 42);
```

**Example: offset_span**

```cpp
#include <decodeless/offset_span.hpp>

using namespace decodeless;

...

struct File {
    offset_span<int> values;
    int data[32];
};

File file;
file.values = file.data;

EXPECT_TRUE(file.values.data() == file.data);
EXPECT_TRUE(file.values.size() == std::size(file.data));
```

## Notes

- The offset pointer behaves like a regular C pointer - you can assign to it,
  dereference etc., and it'll just work
- Initializes to `nullptr` (perhaps controversial - the `glm` library does not zero-initialize)
- The difference is internally it stores an address offset from itself/`this`
- `nullptr` is encoded as the value `1` to allow pointing to itself
- Does not
  [propagate_const](https://en.cppreference.com/w/cpp/experimental/propagate_const)
  and arguably should, for something designed for file mapping

## CMake Integration

This is a header-only library with no dependencies other than C++20. CMake
integration is provided. You don't have to use CMake, but debug visualizers are
integrated if you do. You can use one of the following:

**add_subdirectory**

Add the library as a submodule (`git add submodule
https://github.com/decodeless/offset_ptr.git`), then in cmake:

```cmake
add_subdirectory(path/to/offset_ptr)
target_link_libraries(myproject PRIVATE decodeless::offset_ptr)
```

**FetchContent**

CMake will download the library to the build directory at configure time.

```cmake
include(FetchContent)
FetchContent_Declare(
    decodeless_offset_ptr
    GIT_REPOSITORY https://github.com/decodeless/offset_ptr.git
    GIT_TAG release_tag
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(decodeless_offset_ptr)

target_link_libraries(myproject PRIVATE decodeless::offset_ptr)
```

**find_package**

If using in a library, a config file is provided for
`find_package(... CONFIG ...)`, which trivially
includes `CMakeLists.txt`. See
[decodeless_writer](https://github.com/decodeless/writer/blob/main/CMakeLists.txt)
for an example.

```cmake
find_package(decodeless_offset_ptr REQUIRED CONFIG PATHS ...)

target_link_libraries(myproject PRIVATE decodeless::offset_ptr)
```

## Debug Visualization

If you're using CMake, debugging should be seamless on windows without any setup
and near-seamless on linux (see `auto-load safe-path` below).

Without debug visualizers you'd just see `m_offset` in bytes when hovering over
an `offset_ptr`. You *could* add a watch or print
`*(MyType*)((char*)&offsetPtr + offsetPtr.m_offset)` ... lol. Instead, it'd
be nicer if a debugger showed `offset_ptr` like it was a regular pointer and
`offset_span` like a regular array. That's what the visualizers in
[./debugging](debugging/) do.

**Natvis**

For visual studio users (and somewhat
[vscode](https://github.com/microsoft/vscode-cpptools/issues/10917)), natvis
files are included to allow debug inspection of pointers and spans.

**Pretty Printing**

For gdb, python pretty printers are embedded in
[`.debug_gdb_scripts`](https://www.heuristic42.com/blog/64) sections during
compilation.

You may need to set an [auto-load
safe-path](https://sourceware.org/gdb/current/onlinedocs/gdb.html/dotdebug_005fgdb_005fscripts-section.html)
before gdb will read the inlined scripts. Add this to `~/.gdbinit`:

```set auto-load safe-path <binary directory or parent>```

Or add the following to vscode's `launch.json`:

```json
"setupCommands": [
    {
        "text": "set auto-load safe-path .",
        "description": "enable loading pretty printers",
        "ignoreFailures": false
    }
],
```

## Contributing

Issues and pull requests are most welcome, thank you! Note the
[DCO](CONTRIBUTING) and MIT [LICENSE](LICENSE).
