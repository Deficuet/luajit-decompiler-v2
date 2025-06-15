#include "main.h"
#include "ljd.h"
#include <iostream>

using namespace std;

struct Error {
	const string &message;
	const wstring &filePath;
	const string &function;
	const string &source;
	const string &line;
};

ostream &operator<<(ostream &out, const wstring &str) {
	int cp = GetACP();
	const wchar_t *ws = str.c_str();
	int size = WideCharToMultiByte(cp, NULL, ws, -1, NULL, 0, NULL, NULL);
	char *buf = new char[size]{ 0 };
	WideCharToMultiByte(cp, NULL, ws, -1, buf, size, NULL, NULL);
	cout << buf;
	delete[] buf;
	return out;
}

ostream &operator<<(ostream &out, const Error &error) {
	out << "ljd: Error running ";
	out << error.function << ". ";
	out << "File: " << error.filePath << ", ";
	out << "Source: ";
	out << error.source << ": " << error.line << ". ";
	out << error.message;
	return out;
}

void assert(
	const bool &assertion, 
	const string &message, 
	const wstring &filePath, 
	const string &function, 
	const string &source, 
	const uint32_t &line
) {
	if (!assertion) {
		throw Error {
			.message = message,
			.filePath = filePath,
			.function = function,
			.source = source,
			.line = to_string(line)
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
	EXPORT void ljd_file_to_file(
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
			cout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.filePath << endl;
		}
	}

	EXPORT void ljd_bytes_to_file(
		const wchar_t *name,
		const char *array,
		size_t inSize,
		const wchar_t *outPath
	) {
		Bytecode bytecode{name, array, inSize};
		Ast ast{bytecode, false, false};
		Lua lua{bytecode, ast, outPath, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			cout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.filePath << endl;
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
	EXPORT void ljd_bytes_to_file_append(
		HANDLE file, 
		const wchar_t *name, 
		const char *array, 
		size_t inSize
	) {
		Bytecode bytecode{name, array, inSize};
		Ast ast{bytecode, false, false};
		Lua lua{bytecode, ast, file, false, true};

		try {
			bytecode();
			ast();
			lua();
		} catch (const Error &error) {
			cout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.filePath << endl;
		}
	}
}
