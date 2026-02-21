#include "..\main.h"

FileLua::FileLua(
    const Bytecode &bytecode,
    const Ast &ast,
    const std::wstring &filePath,
    const bool &minimizeDiffs,
    const bool &unrestrictedAscii
) : Lua(filePath, bytecode, ast, minimizeDiffs, unrestrictedAscii) {  }

void FileLua::create_file() {
    file = CreateFileW(identifier.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    assert(file != INVALID_HANDLE_VALUE, "Unable to create file", identifier, DEBUG_INFO);
}

void FileLua::write_file() {
    flush_buffer();
}

void FileLua::close_file() {
    if (file != INVALID_HANDLE_VALUE) {
        CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
    }
}
