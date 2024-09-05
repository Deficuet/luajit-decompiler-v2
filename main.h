/*
Requirements:
  Visual Studio
  C++20
  Windows API
  Default char is unsigned (/J)
*/

#ifndef _CHAR_UNSIGNED
#error Default char is not unsigned!
#endif

#pragma comment(linker, "/stack:268435456")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <bit>
#include <cmath>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <windows.h>
#include <fileapi.h>

#define DEBUG_INFO __FUNCTION__, __FILE__, __LINE__

constexpr uint64_t DOUBLE_SIGN = 0x8000000000000000;
constexpr uint64_t DOUBLE_EXPONENT = 0x7FF0000000000000;
constexpr uint64_t DOUBLE_FRACTION = 0x000FFFFFFFFFFFFF;
constexpr uint64_t DOUBLE_SPECIAL = DOUBLE_EXPONENT;
constexpr uint64_t DOUBLE_NEGATIVE_ZERO = DOUBLE_SIGN;

void assert(const bool &assertion, const std::string &message, const std::wstring &filePath, const std::string &function, const std::string &source, const uint32_t &line);
std::string byte_to_string(const uint8_t &byte);

class Bytecode;
class Ast;
class Lua;

#include "bytecode\bytecode.h"
#include "ast\ast.h"
#include "lua\lua.h"
