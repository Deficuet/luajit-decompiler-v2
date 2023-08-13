class Lua {
public:

	Lua(const Ast& ast, const std::string& filePath);
	~Lua();

	void operator()();

	const std::string filePath;

private:

	void create_file();
	void close_file();
	void write_file();

	const Ast& ast;
	HANDLE file = INVALID_HANDLE_VALUE;
	std::string writeBuffer;
};