#include "Interpreter.h"
#include <fstream>
#include<string>
#include<regex>

Interpreter::Interpreter(std::ostream& out_stream)
{
}

bool Interpreter::ReadFile(const std::string& fileName)
{
	std::string line;
	std::ifstream myfile(fileName);

	if (myfile.is_open())
	{

		while (std::getline(myfile, line))
		{
			statements.push_back(line);
		}
		myfile.close();
		for (std::string stmt : statements) {
			TokenizeStr(stmt);
		}
		return true;
	}
	else {
		std::cout << "Unable to open file";
		return false;
	}
}

std::vector<std::string> Interpreter::TokenizeStr(const std::string& input)
{
	std::string token;

	for (int i = 0; i < input.size(); ++i) {
		char ch = input[i];
	
		if (isspace(ch)) 
		{
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		}
		else {
			token += ch;
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	evaluate(tokens);
	return tokens;
}

void Interpreter::evaluate(const std::vector<std::string>& tokens)
{
	parse_stmt(tokens);
}

void Interpreter::parse_stmt(const std::vector<std::string>& tokens)
{
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");
	std::string next_token = peek(); 

	if (next_token == "config")
	{
		consume(next_token);
		parse_configstmt(tokens);
	}
	else if (next_token == "print")
	{
		consume(next_token);
		parse_printstmt(tokens);
	}
	else if (std::regex_match(next_token, variable))
	{
		
		symbolTable[next_token] = 0;
		consume(next_token);
		parse_assgstmt(tokens);

	}
	else if (next_token == ETX )
	{
		return;
	}
	else 
	{
		std::cout << "error bitch: " << std::endl;
		throw std::runtime_error("error");

	}
}

void Interpreter::parse_configstmt(const std::vector<std::string>& tokens)
{
	std::string next_token = peek();
	consume(next_token);
 
	if (next_token == "dec") 
	{
	}
	else if (next_token == "hex") 
	{
	}
	else if (next_token == "bin") 
	{
	}
}

void Interpreter::parse_printstmt(const std::vector<std::string>& tokens)
{
	std::string next_token = peek();
	std::regex Int("-?[0-9]+");
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");

	if (std::regex_match(next_token, variable))
	{
		result = symbolTable[next_token];
		consume(next_token);
	}
	else if (std::regex_match(next_token, Int)) 
	{
		result = parse_MathExp(tokens);
	}
	else
	{
		std::cout << "error bitch: " << std::endl;
		throw std::runtime_error("Error at token: " + next_token);
	}

	std::cout << result;
}

void Interpreter::parse_assgstmt(const std::vector<std::string>& tokens)
{
	std::string next_token = peek();
	std::string variableToken = peekP();

	if (next_token == ETX)
	{
		return;
	}
	if (next_token != "=")
	{
		std::cout << "error bitch: " << std::endl;
		throw std::runtime_error("error");
	}
	consume(next_token);
	result = parse_MathExp(tokens);
	symbolTable[variableToken] = result;
}

int Interpreter::parse_MathExp(const std::vector<std::string>& tokens)
{

	return parse_SumExp(tokens);
}

int Interpreter::parse_SumExp(const std::vector<std::string>& tokens)
{
	result = parse_ProductExp(tokens);
	std::string next_token = peek();

	while (true)
	{
		if (next_token == "+")
		{
			consume(next_token);
			result += parse_ProductExp(tokens);
		}
		else if (next_token == "-") 
		{
			consume(next_token);
			result -=parse_ProductExp(tokens);
		}
		else 
		{
			break;
		}

		std::string next_token = peek();
	}

	return result;
}

int Interpreter::parse_ProductExp(const std::vector<std::string>& tokens)
{
	result = parse_PrimaryExp(tokens);
	std::string next_token = peek();

	while (true)
	{
		if (next_token == "*")
		{
			consume(next_token);
			result *= parse_ProductExp(tokens);
		}
		else if (next_token == "/")
		{
			consume(next_token);
			result /= parse_ProductExp(tokens);
		}
		else
		{
			break;
		}

		std::string next_token = peek();
	}
	return result;
}

int Interpreter::parse_PrimaryExp(const std::vector<std::string>& tokens)
{
	std::string next_token = peek();
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");
	std::regex Int(("-?[0-9]+"));

	if (next_token == "(")
	{
		consume(next_token);
		result = parse_MathExp(tokens);

		if (next_token != ")")
		{
			std::cout << "error bitch: " << std::endl;
			throw std::runtime_error("Error at token: " + next_token);
		}
		consume(next_token);
	}
	else if (regex_match(next_token, variable))
	{
		result = symbolTable[next_token];
		consume(next_token);
	}
	else if (regex_match(next_token, Int))
	{
		result = symbolTable[next_token];
		consume(next_token);
	}
	else 
	{
		std::cout << "error bitch: " << std::endl;
		throw std::runtime_error("Error at token: " + next_token);
	}
	return result;
}

std::string Interpreter::peek()
{
	return peek(0);
}

std::string Interpreter::peekP()
{
	return peekP(0);
}

std::string Interpreter::peek(int steps)
{
	if (position + steps >= tokens.size()) return ETX;
	return tokens[position + steps];
}

void Interpreter::consume(const std::string& token)
{
	std::string next_token = peek();
	if (next_token == ETX)
		throw std::runtime_error("Consumed past last token\n");
	if (next_token != token)
		throw std::runtime_error("Could not consume token " + token + "\n");
	++position;
}

std::string Interpreter::peekP(int steps)
{
  return tokens[position - steps];
}


