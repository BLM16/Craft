#pragma once

#include <string>
#include <map>

/// <summary>
/// The types of tokens
/// </summary>
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

/// <summary>
/// Maps TokenType's name to a string for use in debugging
/// </summary>
static std::map<TokenType, std::string> TokenStringMap
{
	{ TokenType::VARIABLE, "VARIABLE" },
	{ TokenType::COMMAND, "COMMAND" },
	{ TokenType::FUNCTION, "FUNCTION" },
	{ TokenType::FUNCTION_CALL, "FUNCTION_CALL" },
	{ TokenType::STRING_LITERAL, "STRING_LITERAL" },
	{ TokenType::COMMENT, "COMMENT" },
	{ TokenType::UNSET, "UNSET" }
};

/// <summary>
/// Token holds data and metadata about a portion of the craftfile
/// </summary>
struct Token
{
public:
	/// <summary>
	/// The token instance's type
	/// </summary>
	TokenType Type { TokenType::UNSET };
	/// <summary>
	/// The value of the token instance
	/// </summary>
	std::string Value;
	/// <summary>
	/// The line number in the craftfile that the token is on for use in debuging and error logging
	/// </summary>
	size_t LineNumber { 0 };
};
