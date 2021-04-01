#pragma once

#include <string>
#include <map>

enum class TokenType : short
{
	VARIABLE,
	COMMAND,
	FUNCTION,
	FUNCTION_CALL,
	STRING_LITERAL,
	COMMENT,
	UNSET
};

static std::map<TokenType, std::string> TokenStringMap {
	{ TokenType::VARIABLE, "VARIABLE" },
	{ TokenType::COMMAND, "COMMAND" },
	{ TokenType::FUNCTION, "FUNCTION" },
	{ TokenType::FUNCTION_CALL, "FUNCTION_CALL" },
	{ TokenType::STRING_LITERAL, "STRING_LITERAL" },
	{ TokenType::COMMENT, "COMMENT" },
	{ TokenType::UNSET, "UNSET" }
};

struct Token
{
public:
	// Initialize the token type to unset
	enum TokenType Type { TokenType::UNSET };
	std::string Value;
	size_t LineNumber { 0 };
};
