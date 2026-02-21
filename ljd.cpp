#include "main.h"
#include "ljd.h"
#include <iostream>

using namespace std;

struct Error {
	const string message;
	const wstring &filePath;
	const char *function;
	const char *source;
	const uint32_t line;
};

wostream &operator<<(wostream &wout, const Error &error) {
	wout << "ljd: Error running ";
	wout << error.function << ". ";
	wout << "File: " << error.filePath << ", ";
	wout << "Source: ";
	wout << error.source << ": " << error.line << ". ";
	wout << error.message.c_str();
	return wout;
}

void assert(
	const bool assertion, 
	const string message, 
	const wstring &filePath, 
	const char *function, 
	const char *source, 
	const uint32_t line
) {
	if (!assertion) {
		throw Error {
			.message = message,
			.filePath = filePath,
			.function = function,
			.source = source,
			.line = line
		};
	}
}

string byte_to_string(const uint8_t &byte) {
	char string[] = "0x00";
	uint8_t digit;
	for (uint8_t i = 2; i--;) {
		digit = (byte >> i * 4) & 0xF;
		string[3 - i] = digit >= 0xA ? 'A' + digit - 0xA : '0' + digit;
	}
	return string;
}

extern "C" {
	LJD_API void ljd_file_to_file(
		const wchar_t *input, 
		const wchar_t *output
	) {
		FileBytecode bytecode{input};
		Ast ast{bytecode, false, false};
		FileLua lua{bytecode, ast, output, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			wcout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.identifier << endl;
		}
	}

	LJD_API void ljd_bytes_to_file(
		const wchar_t *name,
		const char *array,
		size_t inSize,
		const wchar_t *outPath
	) {
		MemoryBytecode bytecode{name, array, inSize};
		Ast ast{bytecode, false, false};
		FileLua lua{bytecode, ast, outPath, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			wcout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.identifier << endl;
		}
	}
	
	/**
	 * GENERIC_WRITE, 
	 * NULL, 
	 * NULL, 
	 * CREATE_ALWAYS, 
	 * FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
	 * NULL
	 */
	LJD_API void ljd_bytes_to_file_append(
		HANDLE file, 
		const wchar_t *name, 
		const char *array, 
		size_t inSize
	) {
		MemoryBytecode bytecode{name, array, inSize};
		Ast ast{bytecode, false, false};
		FileLuaAppend lua{bytecode, ast, file, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			wcout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.identifier << endl;
		}
	}

	LJD_API void ljd_bytes_to_buffer(
		const wchar_t *name, 
		const char *array, 
		size_t inSize, 
		CodeCallback callback
	) {
		MemoryBytecode bytecode{name, array, inSize};
		Ast ast{bytecode, false, false};
		MemoryLua lua{bytecode, ast, false, true};

		try {
			bytecode();
			ast();
			lua();
			const std::string &buf = lua.get_buffer();
			callback(buf.data(), buf.size());
		} catch (const Error &error) {
			wcout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.identifier << endl;
		}
	}
}
