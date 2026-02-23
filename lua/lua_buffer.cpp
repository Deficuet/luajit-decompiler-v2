#include "..\main.h"

MemoryLua::MemoryLua(const Bytecode &bytecode, const Ast &ast, const bool &minimizeDiffs, const bool &unrestrictedAscii)
    : Lua(bytecode.identifierBuilder, bytecode, ast, minimizeDiffs, unrestrictedAscii) {  }
