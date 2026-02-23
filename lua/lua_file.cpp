#include "..\main.h"

FileLua::FileLua(
    const Bytecode &bytecode,
    const Ast &ast,
    const wchar_t *filePath,
    const bool &minimizeDiffs,
    const bool &unrestrictedAscii
) : Lua(bytecode.identifierBuilder, bytecode, ast, minimizeDiffs, unrestrictedAscii), filePath{filePath} {  }

void FileLua::create_file() {
    file = CreateFileW(filePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    assert(file != INVALID_HANDLE_VALUE, "Unable to create file", identifierBuilder, DEBUG_INFO);
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
