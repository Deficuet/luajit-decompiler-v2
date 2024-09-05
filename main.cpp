#include "main.h"
#include <iostream>

#define PY_EXPORT __declspec(dllexport)

struct Error {
	const std::string &message;
	const std::wstring &filePath;
	const std::string &function;
	const std::string &source;
	const std::string &line;
};

std::wostream &operator<<(std::wostream &wout, const Error &error) {
	wout << "Error running ";
	wout << error.function.c_str() << ". ";
	wout << "Source: ";
	wout << error.source.c_str() << ": " << error.line.c_str() << ". ";
	wout << error.message.c_str();
	return wout;
}

void assert(
	const bool &assertion, 
	const std::string &message, 
	const std::wstring &filePath, 
	const std::string &function, 
	const std::string &source, 
	const uint32_t &line
) {
	if (!assertion) {
		throw Error {
			.message = message,
			.filePath = filePath,
			.function = function,
			.source = source,
			.line = std::to_string(line)
		};
	}
}

std::string byte_to_string(const uint8_t &byte) {
	char string[] = "0x00";
	uint8_t digit;
	for (uint8_t i = 2; i--;) {
		digit = (byte >> i * 4) & 0xF;
		string[3 - i] = digit >= 0xA ? 'A' + digit - 0xA : '0' + digit;
	}
	return string;
}

extern "C" {
	PY_EXPORT void file_to_file(
		const wchar_t *input, 
		const wchar_t *output
	) {
		Bytecode bytecode{input};
		Ast ast{bytecode, false, false};
		Lua lua{bytecode, ast, output, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			std::wcout << error << std::endl;
		} catch (...) {
			std::wcout << "Unknown exception in file: " << bytecode.filePath << std::endl;
		}
	}

	PY_EXPORT HANDLE open_src_file(const wchar_t *filePath) {
		return CreateFileW(filePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	}

	PY_EXPORT void bytes_to_file_append(
		HANDLE file, 
		const wchar_t *name, 
		const char *array, 
		size_t size
	) {
		Bytecode bytecode{name, array, size};
		Ast ast{bytecode, false, false};
		Lua lua{bytecode, ast, file, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			std::wcout << error << std::endl;
		} catch (...) {
			std::wcout << "Unknown exception in file: " << bytecode.filePath << std::endl;
		}
	}

	PY_EXPORT void close_src_file(HANDLE file) {
		if (file == INVALID_HANDLE_VALUE) {
			std::cout << "close_seg_file: invalid handle" << std::endl;
			return;
		} else {
			CloseHandle(file);
		}
	}
}
