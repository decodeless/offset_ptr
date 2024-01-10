# pknowles offset_ptr

Tiny header-only library for an address space independent data structure. The
humble offset pointer is a relative address from its own address in memory
rather than an absolute address. As long as the thing it points to is in the
same address space it will still work. Quite useful if you're...

- Sharing memory between processes
- Memory mapping a file from disk, or even just reading big chunks
- Copying complex data structures to the GPU
- Duplicating the raw memory of a data structure, e.g. a low overhead deep copy

A tiny offset span class is provided too, which is just an offset pointer and
size. It `offset_span` is very similar to `std::span` but with a relative address.

**Example: offset_ptr**

```
#include <nodecode/offset_ptr.hpp>

using namespace nodecode;

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

```
#include <nodecode/offset_span.hpp>

using namespace nodecode;

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

- The offset pointer behaves like a regular C pointer
- Initializes to `nullptr` (perhaps controversial - the `glm` library does not)
- The difference is internally it stores an address offset from itself/`this`
- `nullptr` is encoded as the value `1` to allow pointing to itself
- Does not
  [propagate_const](https://en.cppreference.com/w/cpp/experimental/propagate_const)
  and arguably should, for something designed for file mapping

## Natvis

For visual studio users (and somewhat vscode
https://github.com/microsoft/vscode-cpptools/issues/10917), natvis files are
included to allow debug inspection of pointers and spans.

## Contributing

Issues and pull requests are most welcome, thank you! Note the
[DCO](CONTRIBUTING) and MIT [LICENSE](LICENSE).
