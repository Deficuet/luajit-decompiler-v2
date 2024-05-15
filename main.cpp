#include "main.h"

#include <iostream>

struct Error {
	const std::string message;
	const std::string filePath;
	const std::string function;
	const std::string source;
	const std::string line;
};

std::ostream& operator<<(std::ostream &out, const Error &error) {
	out << "Error running ";
	out << error.function << ". ";
	out << "Source: ";
	out << error.source << ": " << error.line << ". ";
	out << error.message << std::endl;
	return out;
}

#define PY_FUNC __declspec(dllexport)

extern "C" {
	PY_FUNC void decompile(const char *input, const char *output) {
		std::string inPath{input};
		std::string outPath{output};

		Bytecode bytecode{inPath};
		Ast ast{bytecode, false, false};
		Lua lua{bytecode, ast, outPath, false, false};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			std::cout << error;
		} catch (...) {
			std::cout << "Unknown exception in file: " << bytecode.filePath << std::endl;
		}
	}
}

void assert(const bool& assertion, const std::string& message, const std::string& filePath, const std::string& function, const std::string& source, const uint32_t& line) {
	if (!assertion) throw Error{
		.message = message,
		.filePath = filePath,
		.function = function,
		.source = source,
		.line = std::to_string(line)
	};
}

std::string byte_to_string(const uint8_t& byte) {
	char string[] = "0x00";
	uint8_t digit;
	
	for (uint8_t i = 2; i--;) {
		digit = (byte >> i * 4) & 0xF;
		string[3 - i] = digit >= 0xA ? 'A' + digit - 0xA : '0' + digit;
	}

	return string;
}
