#include "..\main.h"

MemoryBytecode::MemoryBytecode(const NameBuilder *identifierBuilder, const char *array, size_t size)
    : Bytecode(identifierBuilder), array{(uint8_t *)array}
{
    fileSize = size;
    bytesUnread = size;
}

void MemoryBytecode::read_buffer(const uint32_t &byteCount)
{
    uint64_t base = fileSize - bytesUnread;
    std::copy(array + base, array + base + byteCount, fileBuffer.begin());
}
