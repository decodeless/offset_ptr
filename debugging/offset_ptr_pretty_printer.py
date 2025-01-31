import gdb
import re


class OffsetPtrPrinter:
    """Pretty printer for decodeless::offset_ptr<T>"""
    def __init__(self, val):
        self.val = val
        self.T = self.val.type.strip_typedefs().unqualified().template_argument(0)

    def pointer(self):
        base, offset = self.val.address, self.val["m_offset"]

        # Handle nullptr magic offset value of '1'
        if offset == 1:
            base, offset = gdb.Value(0), gdb.Value(0)

        return (base.cast(gdb.lookup_type("char").pointer()) + offset).cast(self.T.pointer())

    def children(self):
        # TODO: rather than a virtual '*' member, can this be made to behave
        # more like a real pointer?
        return [("operator*()", self.pointer().dereference())]

    def to_string(self):
        return f'({self.T} *) {self.pointer()}'


offset_ptr_lookup_pattern = re.compile("^decodeless::offset_ptr<.*>$")


def offset_ptr_lookup(val):
    global offset_ptr_lookup_pattern
    lookup_tag = val.type.strip_typedefs().unqualified().tag
    if lookup_tag is None:
        return None
    if offset_ptr_lookup_pattern.match(lookup_tag):
        return OffsetPtrPrinter(val)
    return None


gdb.pretty_printers.append(offset_ptr_lookup)
