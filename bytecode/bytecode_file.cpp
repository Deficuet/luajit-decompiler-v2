#include "..\main.h"

FileBytecode::FileBytecode(const std::wstring &identifier) : Bytecode(identifier) {  }

void FileBytecode::open_file() {
    assert(file == INVALID_HANDLE_VALUE, "File already open", identifier, DEBUG_INFO);
    file = CreateFileW(identifier.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    assert(file != INVALID_HANDLE_VALUE, "Failed to open file", identifier, DEBUG_INFO);
    LARGE_INTEGER size;
    assert(GetFileSizeEx(file, &size), "Failed to get file size", identifier, DEBUG_INFO);
    fileSize = size.QuadPart;
    assert(fileSize >= MIN_FILE_SIZE, "File is too small or empty", identifier, DEBUG_INFO);
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
    assert(ReadFile(file, fileBuffer.data(), byteCount, &bytesRead, NULL) && bytesRead == byteCount, "Failed to read file", identifier, DEBUG_INFO);
}
