#include "main.h"
#include "ljd.h"
#include <iostream>

using namespace std;

struct Error {
	const string_view message;
	const NameBuilder *nameBuilder;
	const char *function;
	const char *source;
	const uint32_t line;
};

wostream &operator<<(wostream &wout, const Error &error) {
	size_t nameLength;
	const wchar_t *name = error.nameBuilder->callback(error.nameBuilder->userData, &nameLength);

	wout << "ljd: Error running ";
	wout << error.function << ". ";
	wout << "File: " << std::wstring_view(name, nameLength) << ", ";
	wout << "Source: ";
	wout << error.source << ": " << error.line << ". ";
	wout << error.message.data();
	return wout;
}

void assert(
	const bool assertion, 
	const string &message, 
	const NameBuilder *nameBuilder, 
	const char *function, 
	const char *source, 
	const uint32_t line
) {
	if (!assertion) {
		throw Error {
			.message = message,
			.nameBuilder = nameBuilder,
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

static const wchar_t *wstring_as_is_builder(void *userData, size_t *lengthPtr) {
	auto filePath = static_cast<const wchar_t *>(userData);
	if (lengthPtr) {
		*lengthPtr = wcslen(filePath);
	}
	return filePath;
}

extern "C" {
	LJD_API void ljd_file_to_file(
		const wchar_t *input, 
		const wchar_t *output
	) {
		NameBuilder nameBuilder{.callback = wstring_as_is_builder, .userData = (void *)input};

		FileBytecode bytecode{&nameBuilder, input};
		Ast ast{bytecode, false, false};
		FileLua lua{bytecode, ast, output, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			wcout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.identifierBuilder << endl;
		}
	}

	LJD_API void ljd_bytes_to_file(
		const char *array,
		size_t inSize,
		const NameBuilder *nameBuilder,
		const wchar_t *outPath
	) {
		MemoryBytecode bytecode{nameBuilder, array, inSize};
		Ast ast{bytecode, false, false};
		FileLua lua{bytecode, ast, outPath, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			wcout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.identifierBuilder << endl;
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
		const char *array, 
		size_t inSize,
		const NameBuilder *nameBuilder
	) {
		MemoryBytecode bytecode{nameBuilder, array, inSize};
		Ast ast{bytecode, false, false};
		FileLuaAppend lua{bytecode, ast, file, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			wcout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.identifierBuilder << endl;
		}
	}

	LJD_API void ljd_bytes_to_buffer(
		const char *array, 
		size_t inSize, 
		const NameBuilder *nameBuilder,
		CodeCallback callback
	) {
		MemoryBytecode bytecode{nameBuilder, array, inSize};
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
			wcout << "Unknown exception in file: " << bytecode.identifierBuilder << endl;
		}
	}
}
