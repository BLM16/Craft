#include <iostream>
#include <filesystem>
#include <string>

#include "Utils.h"
#include "Lexer.h"
#include "Parser.h"

namespace fs = std::filesystem;
using std::string;

Lexer LEXER = Lexer();
Parser PARSER = Parser();

int main(int argc, char **argv)
{
	// Get the current path
	fs::path current_path = fs::current_path();

	// Check if the current path contains a craftfile
	bool path_has_craftfile = fs::exists(current_path / ".craft") ? fs::is_regular_file(current_path / ".craft") : false;
	if (!path_has_craftfile)
	{
		std::cout << "Craftfile does not exist in current directory." << std::endl;
		return 1;
	}

	// No function is specified
	if (argc == 1)
	{
		std::cout << "A function must be specified." << std::endl;
		return 1;
	}

	// Get the function to execute from the craftfile
	char* function_to_execue = argv[1];
	// Read the data from the craft file
	string craftfile_data = read_file(current_path / ".craft");

	// Tokenize the craftfile contents
	std::vector<Token> Tokens = LEXER.parse(craftfile_data);
	if (LEXER.getState() == LexerState::ERROR)
	{
		std::cout << "Craft job terminated." << std::endl;
		return 1;
	}

	// Convert the tokens into node trees
	std::vector<Node> Nodes = PARSER.parse(Tokens);
	if (PARSER.getState() == ParserState::ERROR)
	{
		std::cout << "Craft job terminated." << std::endl;
		return 1;
	}
}
