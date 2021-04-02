#include "Parser.h"

/// <summary>
/// The function that converts Tokens into node trees
/// </summary>
/// <param name="Tokens">The vector of Tokens to convert</param>
/// <returns>A vector of Nodes</returns>
std::vector<Node> Parser::parse(std::vector<Token> Tokens)
{
	// Loop through every token in the vector of Tokens
	for (Token tok : Tokens)
	{
		// Determine how to deal with the token
		switch (tok.Type)
		{
		case TokenType::VARIABLE:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid variable definition");
			else
				// Set currentVar to the variable name
				currentVar = tok.Value;
			break;

		case TokenType::COMMAND:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid command definition");
			else if (tok.Value != "")
			{
				// Command node is created
				currentNode.Name = tok.Value;
				// Replace variables with their values before appending
				ReplaceVariables(tok, currentNode.Name);
				// Append the node to the parent node if defined or else the vector of Nodes
				if (parentNode.Name != "")
					parentNode.Nodes.push_back(currentNode);
				else
					Nodes.push_back(currentNode);
				// Reset the current node
				ResetNode(currentNode);
			}
			break;

		case TokenType::FUNCTION:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid function definition");
			else
			{
				// If there is a parent node, append it because functions can't be nested
				if (parentNode.Name != "")
					AppendNode(parentNode);
				// Set the parent node to be the new function
				parentNode.Name = tok.Value;
			}
			break;

		case TokenType::FUNCTION_CALL:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid function call");
			else
			{
				// Get the index of the called node from the vector of nodes
				int i = FetchNodeIndex(Nodes, tok.Value);
				if (i == -1)
					ThrowParserSyntaxError(tok, "Function is not defined");
				else
				{
					// Set the target to append the function contents to
					std::vector<Node>& target = parentNode.Name != "" ? parentNode.Nodes : Nodes;

					// Get the desired function node by index
					Node called = Nodes[i];
					// Append all the child nodes from the function node to the target
					for (Node n : called.Nodes)
						target.push_back(n);
				}
			}
			break;

		case TokenType::STRING_LITERAL:
			if (currentNode.Name != "")
				ThrowParserSyntaxError(tok, "Invalid string literal");
			else
			{
				// Current variable name is not defined so this is an invalid variable declaration
				if (currentVar == "")
					ThrowParserSyntaxError(tok, "Invalid string literal");
				else
				{
					// Replace the variables in the string with their values
					ReplaceVariables(tok, tok.Value);
					// Add the variable to the map of variables
					variables[currentVar] = tok.Value;
					// Reset the current variable name
					currentVar.erase();
				}
			}
			break;

		case TokenType::COMMENT:
			// Ignore comments
			break;

		default:
			// Token wasn't recognized meaning there was an uncaught lexing error
			ThrowParserSyntaxError(tok, "Unhandled token");
			break;
		}
	}

	// Append the final node to the vector of nodes
	if (parentNode.Name != "")
		AppendNode(parentNode);

	return Nodes;
}

/// <summary>
/// Resets the node passed by reference
/// </summary>
/// <param name="node">Reference to the node to reset</param>
void Parser::ResetNode(Node& node)
{
	node.Name.erase();
	node.Nodes.clear();
}

/// <summary>
/// Appends the node passed by reference to the vector of Nodes
/// </summary>
/// <param name="node">Reference to the node to append</param>
void Parser::AppendNode(Node& node)
{
	Nodes.push_back(node);
	ResetNode(node);
}

/// <summary>
/// Gets the index of a node by name from a given vector of nodes
/// </summary>
/// <param name="vec">The vector to find the node's index in</param>
/// <param name="name">The name of the node to find the index of</param>
/// <returns></returns>
int Parser::FetchNodeIndex(std::vector<Node> vec, std::string name)
{
	// Default the index to "not found"
	int index = -1;
	// Find the node by name in the vector
	auto iter = std::find(vec.begin(), vec.end(), name);

	// The node exists in the vector so set index to the node's index
	if (iter != vec.end())
		index = std::distance(vec.begin(), iter);

	return index;
}

/// <summary>
/// Replaces all of the variables in a string with their values
/// </summary>
/// <param name="tok">Token for use in debugging and error reporting</param>
/// <param name="str">Reference to the string to replace variables in</param>
void Parser::ReplaceVariables(Token tok, std::string& str)
{
	// Define the regex to match variables: $(name)
	auto const expr = std::regex("\\$\\((\\w+)\\)");
	// Loop and replace all variables
	while (true)
	{
		// Define where to put the regex match
		auto res = std::smatch{};
		// Search the string for matches with the variable regex and save the matches to res
		std::regex_search(str, res, expr);

		// There are no more matches so exit the loop
		if (res.empty())
			break;

		// Get the variable by name from variables
		auto t = variables.find(res[1]);
		// The variable doesn't exist so throw an error and exit the loop
		if (t == variables.end())
		{
			ThrowParserSyntaxError(tok, "Variable doesn\'t exist");
			break;
		}
		else
			// Replace the variable call with the variable value
			str.replace(res.position(0), res[1].length() + 3, t->second);
	}
}

/// <summary>
/// Throws errors that are encountered in the parsing process and sets the parser state to error
/// </summary>
/// <param name="tok">Token for use in error reporting</param>
/// <param name="msg">The message to throw as the error</param>
void Parser::ThrowParserSyntaxError(Token tok, std::string msg) {
	STATE = ParserState::ERROR;
	std::cerr << "Syntax Error @ln:" << tok.LineNumber << "\n\tError: " << msg << std::endl;
}
