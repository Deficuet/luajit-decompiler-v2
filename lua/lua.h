#pragma once

class Lua {
public:
	virtual ~Lua();

	void operator()();

	const std::wstring identifier;

protected:
	static constexpr char UTF8_BOM[] = "\xEF\xBB\xBF";
	static constexpr char NEW_LINE[] = "\r\n";

	Lua(const std::wstring &, const Bytecode &, const Ast &, const bool &, const bool &);

	HANDLE file = INVALID_HANDLE_VALUE;
	std::string writeBuffer;

	virtual void create_file() {  }
	virtual void write_file() {  }
	virtual void close_file() {  }

	void write(const std::string &string);
	void flush_buffer();

private:
	void write_header();
	void write_block(const Ast::Function &function, const std::vector<Ast::Statement *> &block);
	void write_expression(const Ast::Expression &expression, const bool &useParentheses);
	void write_prefix_expression(const Ast::Expression &expression, const bool &isLineStart);
	void write_variable(const Ast::Variable &variable, const bool &isLineStart);
	void write_function_call(const Ast::FunctionCall &functionCall, const bool &isLineStart);
	void write_assignment(const std::vector<Ast::Variable> &variables, const std::vector<Ast::Expression *> &expressions, const std::string &seperator, const bool &isLineStart);
	void write_expression_list(const std::vector<Ast::Expression *> &expressions, const Ast::Expression *const &multres);
	void write_function_definition(const Ast::Function &function, const bool &isMethod);
	void write_number(const double &number);
	void write_string(const std::string &string);
	uint8_t get_operator_precedence(const Ast::Expression &expression);
	template <typename... Strings>
	void write(const std::string &string, const Strings &...strings);
	void write_indent();

	const Bytecode &bytecode;
	const Ast &ast;
	const bool minimizeDiffs;
	const bool unrestrictedAscii;
	uint32_t indentLevel = 0;
	uint64_t prototypeDataLeft = 0;
};

class FileLua : public Lua {
public:
	FileLua(const Bytecode &, const Ast &, const std::wstring &, const bool &, const bool &);

protected:
	void create_file() override;
	void write_file() override;
	void close_file() override;
};

class FileLuaAppend : public Lua {
public:
	FileLuaAppend(const Bytecode &, const Ast &, HANDLE, const bool &, const bool &);

protected:
	void write_file() override;
};

class MemoryLua : public Lua {
public:
	MemoryLua(const Bytecode &, const Ast &, const bool &, const bool &);
	const std::string& get_buffer() const { return writeBuffer; }
};
