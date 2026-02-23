#pragma once

class Bytecode {
public:

	class Prototype;
	struct Constant;
	struct NumberConstant;
	struct TableConstant;
	struct TableNode;
	struct VariableInfo;
	struct Instruction;
	#include "prototype.h"
	#include "constants.h"
	#include "instructions.h"

	virtual ~Bytecode();

	void operator()();

	const NameBuilder *identifierBuilder;

	struct {
		uint8_t version = 0;
		uint8_t flags = 0;
		std::string chunkname;
	} header;

	const Prototype* main = nullptr;
	uint64_t prototypesTotalSize = 0;

protected:
	static constexpr uint8_t MIN_PROTO_SIZE = 11;
	static constexpr uint8_t MIN_FILE_SIZE = MIN_PROTO_SIZE + 7;
	
	Bytecode(const NameBuilder *);

	std::vector<uint8_t> fileBuffer;
	uint64_t fileSize = 0;
	uint64_t bytesUnread = 0;
	HANDLE file = INVALID_HANDLE_VALUE;

	virtual void open_file() {  }
	virtual void close_file() {  }
	virtual void read_buffer(const uint32_t& byteCount) = 0;

private:
	void read_header();
	void read_prototypes();
	void read_file(const uint32_t& byteCount);
	uint32_t read_uleb128();
	bool buffer_next_block();
	
	std::vector<Prototype*> prototypes;
};

class MemoryBytecode : public Bytecode {
public:
	MemoryBytecode(const NameBuilder *, const char *, size_t);

protected:
	void read_buffer(const uint32_t& byteCount) override;

private:
	const uint8_t * const array;
};

class FileBytecode : public Bytecode {
public:
	FileBytecode(const NameBuilder *, const wchar_t *);

protected:
	void open_file() override;
	void close_file() override;
	void read_buffer(const uint32_t& byteCount) override;

private:
	const wchar_t *filePath;
};
