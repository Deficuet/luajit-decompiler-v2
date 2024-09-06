#include "main.h"
#include <iostream>

#define PY_EXPORT __declspec(dllexport)

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
			cout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.filePath << endl;
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
			cout << error << endl;
		} catch (...) {
			wcout << "Unknown exception in file: " << bytecode.filePath << endl;
		}
	}

	PY_EXPORT void close_src_file(HANDLE file) {
		if (file == INVALID_HANDLE_VALUE) {
			cout << "close_seg_file: invalid handle" << endl;
			return;
		} else {
			CloseHandle(file);
		}
	}
}
