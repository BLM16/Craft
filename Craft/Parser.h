#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <regex>

#include "Token.h"
#include "Node.h"

/// <summary>
/// The states of the parser
/// </summary>
enum class ParserState : short
{
	OK, // The parser state is normal
	ERROR // The parser has encountered an error
};

/// <summary>
/// Used to parse Tokens into node trees
/// </summary>
class Parser
{
public:
	/// <summary>
	/// The function that converts Tokens into node trees
	/// </summary>
	/// <param name="Tokens">The vector of Tokens to convert</param>
	/// <returns>A vector of Nodes</returns>
	std::vector<Node> parse(std::vector<Token> Tokens);
	/// <summary>
	/// Gets the parser instance's state
	/// </summary>
	/// <returns>ParserState of the parser instance</returns>
	ParserState getState() { return STATE; };

private:
	/// <summary>
	/// The parser instance's current state
	/// </summary>
	ParserState STATE{ ParserState::OK };
	/// <summary>
	/// The vector containing the Nodes that gets returned from parse
	/// </summary>
	std::vector<Node> Nodes;
	/// <summary>
	/// The current parent node that is being parsed
	/// </summary>
	Node parentNode;
	/// <summary>
	/// The current node that is being parsed
	/// </summary>
	Node currentNode;
	/// <summary>
	/// The current variable name used to map variables
	/// </summary>
	std::string currentVar;
	/// <summary>
	/// The map of variable names to their values
	/// </summary>
	std::map<std::string, std::string> variables;

	/// <summary>
	/// Resets the node passed by reference
	/// </summary>
	/// <param name="node">Reference to the node to reset</param>
	void ResetNode(Node& node);
	/// <summary>
	/// Appends the node passed by reference to the vector of Nodes
	/// </summary>
	/// <param name="node">Reference to the node to append</param>
	void AppendNode(Node& node);
	/// <summary>
	/// Gets the index of a node by name from a given vector of nodes
	/// </summary>
	/// <param name="vec">The vector to find the node's index in</param>
	/// <param name="name">The name of the node to find the index of</param>
	/// <returns></returns>
	int FetchNodeIndex(std::vector<Node> vec, std::string name);
	/// <summary>
	/// Replaces all of the variables in a string with their values
	/// </summary>
	/// <param name="tok">Token for use in debugging and error reporting</param>
	/// <param name="str">Reference to the string to replace variables in</param>
	void ReplaceVariables(Token tok, std::string& str);
	/// <summary>
	/// Throws errors that are encountered in the parsing process and sets the parser state to error
	/// </summary>
	/// <param name="tok">Token for use in error reporting</param>
	/// <param name="msg">The message to throw as the error</param>
	void ThrowParserSyntaxError(Token tok, std::string msg);
};
