import gdb
import re


# Encapsulates a list of children. Calls .child() on the pretty printer only
# when requested. This is a workaround for poor performance when inspecting
# large arrays in the debugger.
class LazyChildren:
    def __init__(self, obj, length):
        self._obj = obj
        self._length = length

    def __getitem__(self, index):
        if not 0 <= index < self._length:
            raise IndexError("Index out of range")
        return self._obj.child(index)

    def __len__(self):
        return self._length


class OffsetSpanPrinter:
    """Pretty printer for decodeless::offset_span<T>"""
    def __init__(self, val):
        self.T = val.type.strip_typedefs().unqualified().template_argument(0)
        self.length = val["m_size"]

        ptr = val["m_data"]
        base = ptr.address
        offset = ptr["m_offset"]
        if offset == 1:
            base = gdb.Value(0)
            offset = gdb.Value(0)
        self.pointer = (base.cast(gdb.lookup_type("char").pointer()) + offset).cast(self.T.pointer())

    def child(self, i):
        return (f"[{i}]", (self.pointer + i).dereference())

    def children(self):
        return LazyChildren(self, self.length)

    def __len__(self):
        return self.length

    def to_string(self):
        return f'offset_span<{self.T}> of length {self.length}'

    def display_hint(self):
        return 'array'


offset_span_lookup_pattern = re.compile("^decodeless::offset_span<.*>$")


def offset_span_lookup(val):
    global offset_span_lookup_pattern
    lookup_tag = val.type.strip_typedefs().unqualified().tag
    if lookup_tag is None:
        return None
    if offset_span_lookup_pattern.match(lookup_tag):
        return OffsetSpanPrinter(val)
    return None


gdb.pretty_printers.append(offset_span_lookup)
