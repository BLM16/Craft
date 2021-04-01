#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <regex>

#include "Token.h"
#include "Node.h"

// Used to determine whether to continue or not after parsing
enum class ParserState : short
{
	OK,
	ERROR
};

class Parser
{
public:
	std::vector<Node> parse(std::vector<Token> Tokens);
	ParserState getState() { return STATE; };

private:
	ParserState STATE{ ParserState::OK };

	void ResetNode(Node& node);
	void AppendNode(Node& node);
	int FetchNodeIndex(std::vector<Node> vec, std::string name);
	void ReplaceVariables(Token tok, std::string& str);
	void ThrowParserSyntaxError(Token tok, std::string msg);

	std::vector<Node> Nodes;

	Node parentNode;
	Node currentNode;
	std::string currentVar;

	std::map<std::string, std::string> variables;
};
