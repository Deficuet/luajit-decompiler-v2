#include "..\main.h"

FileBytecode::FileBytecode(const NameBuilder *identifierBuilder, const wchar_t *filePath)
    : Bytecode(identifierBuilder), filePath{filePath} {  }

void FileBytecode::open_file() {
    assert(file == INVALID_HANDLE_VALUE, "File already open", identifierBuilder, DEBUG_INFO);
    file = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    assert(file != INVALID_HANDLE_VALUE, "Failed to open file", identifierBuilder, DEBUG_INFO);
    LARGE_INTEGER size;
    assert(GetFileSizeEx(file, &size), "Failed to get file size", identifierBuilder, DEBUG_INFO);
    fileSize = size.QuadPart;
    assert(fileSize >= MIN_FILE_SIZE, "File is too small or empty", identifierBuilder, DEBUG_INFO);
    bytesUnread = fileSize;
}

void FileBytecode::close_file() {
    if (file != INVALID_HANDLE_VALUE) {
        CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
    }
}

void FileBytecode::read_buffer(const uint32_t& byteCount) {
    DWORD bytesRead = 0;
    assert(ReadFile(file, fileBuffer.data(), byteCount, &bytesRead, NULL) && bytesRead == byteCount, "Failed to read file", identifierBuilder, DEBUG_INFO);
}
