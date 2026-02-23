#include "..\main.h"

FileLuaAppend::FileLuaAppend(
    const Bytecode &bytecode,
    const Ast &ast,
    HANDLE hFile,
    const bool &minimizeDiffs,
    const bool &unrestrictedAscii
) : Lua(bytecode.identifierBuilder, bytecode, ast, minimizeDiffs, unrestrictedAscii) {
    file = hFile;
}

void FileLuaAppend::write_file() {
    write(NEW_LINE);
    flush_buffer();
}
