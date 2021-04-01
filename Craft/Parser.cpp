#include "Parser.h"

std::vector<Node> Parser::parse(std::vector<Token> Tokens)
{
	for (Token tok : Tokens)
	{
		switch (tok.Type)
		{
		case TokenType::VARIABLE:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid variable definition");
			else
				currentVar = tok.Value;
			break;

		case TokenType::COMMAND:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid command definition");
			else if (tok.Value != "")
			{
				currentNode.Name = tok.Value;
				ReplaceVariables(tok, currentNode.Name);
				if (parentNode.Name != "")
					parentNode.Nodes.push_back(currentNode);
				else
					Nodes.push_back(currentNode);
				ResetNode(currentNode);
			}
			break;

		case TokenType::FUNCTION:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid function definition");
			else
			{
				if (parentNode.Name != "")
					AppendNode(parentNode);
				parentNode.Name = tok.Value;
			}
			break;

		case TokenType::FUNCTION_CALL:
			if (currentNode.Name != "" || currentVar != "")
				ThrowParserSyntaxError(tok, "Invalid function call");
			else
			{
				int i = FetchNodeIndex(Nodes, tok.Value);
				if (i == -1)
					ThrowParserSyntaxError(tok, "Function is not defined");
				else
				{
					// This pointer might not work
					std::vector<Node>& target = parentNode.Name != "" ? parentNode.Nodes : Nodes;

					Node called = Nodes[i];
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
				if (currentVar == "")
					ThrowParserSyntaxError(tok, "Invalid string literal");
				else
				{
					ReplaceVariables(tok, tok.Value);
					variables[currentVar] = tok.Value;
					currentVar.erase();
				}
			}
			break;

		case TokenType::COMMENT:
			break;

		default:
			ThrowParserSyntaxError(tok, "Unhandled token");
			break;
		}
	}

	// Close the final token
	if (parentNode.Name != "")
		AppendNode(parentNode);

	return Nodes;
}

void Parser::ResetNode(Node& node)
{
	node.Name.erase();
	node.Nodes.clear();
}

void Parser::AppendNode(Node& node)
{
	Nodes.push_back(node);
	ResetNode(node);
}

int Parser::FetchNodeIndex(std::vector<Node> vec, std::string name)
{
	int index = -1;
	auto iter = std::find(vec.begin(), vec.end(), name);

	if (iter != vec.end())
		index = std::distance(vec.begin(), iter);

	return index;
}

void Parser::ReplaceVariables(Token tok, std::string& str)
{
	auto const expr = std::regex("\\$\\((\\w+)\\)");
	while (true)
	{
		auto res = std::smatch{};
		std::regex_search(str, res, expr);

		if (res.empty())
			break;

		auto t = variables.find(res[1]);
		if (t == variables.end())
		{
			ThrowParserSyntaxError(tok, "Variable doesn\'t exist");
			break;
		}
		else
			str.replace(res.position(0), res[1].length() + 3, t->second);
	}
}

void Parser::ThrowParserSyntaxError(Token tok, std::string msg) {
	STATE = ParserState::ERROR;
	std::cerr << "Syntax Error @ln:" << tok.LineNumber << "\n\tError: " << msg << std::endl;
}
