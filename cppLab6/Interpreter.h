#pragma once
#include<iostream>
#include<map>
#include<vector>
class Interpreter
{
public: 

	Interpreter(std::ostream& out_stream);

	bool ReadFile(const std::string& fileName);

	void evaluate(const std::vector<std::string>& tokens);

	std::vector<std::string> TokenizeStr(const std::string& input);

	std::vector<std::string> statements;

	std::vector<std::string> tokens;
	int position = 0;
	const std::string ETX = "\u0003";
	std::map<std::string, int> symbolTable;
	int result = 0;

	void parse_stmt(const std::vector<std::string>& tokens);
	void parse_configstmt(const std::vector<std::string>& tokens);
	void parse_printstmt(const std::vector<std::string>& tokens);
	void parse_assgstmt(const std::vector<std::string>& tokens);
	int parse_MathExp(const std::vector<std::string>& tokens);
	int parse_SumExp(const std::vector<std::string>& tokens);
	int parse_ProductExp(const std::vector<std::string>& tokens);
	int parse_PrimaryExp(const std::vector<std::string>& tokens);

	std::string peek();
	std::string peekP();

	std::string peek(int steps);
	std::string peekP(int steps);

	void consume(const std::string& token);



};

