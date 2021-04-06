#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Token.h"
#include "Utils.h"

/// <summary>
/// The states of the lexer
/// </summary>
enum class LexerState : short
{
	OK, // The lexer state is normal
	ERROR // The lexer has encountered an error
};

/// <summary>
/// Used to tokenize a craftfile
/// </summary>
class Lexer
{
public:
	/// <summary>
	/// The function that tokenizes the craftfile data
	/// </summary>
	/// <param name="data">The data to tokenize</param>
	/// <returns>A vector of Tokens</returns>
	std::vector<Token> parse(std::string data);
	/// <summary>
	/// Gets the lexer instance's state
	/// </summary>
	/// <returns>LexerState of the lexer instance</returns>
	LexerState getState() { return STATE; };

private:
	/// <summary>
	/// The lexer instance's current state
	/// </summary>
	LexerState STATE { LexerState::OK };
	/// <summary>
	/// The vector containing the Tokens that gets returned from parse
	/// </summary>
	std::vector<Token> Tokens;
	/// <summary>
	/// The current token that is being parsed
	/// </summary>
	Token currentToken;
	/// <summary>
	/// The line number in the craftfile for use in debugging and error reporting
	/// </summary>
	int lineNumber { 1 };
	/// <summary>
	/// Used to store whether or not the current character is escaped with a backslash
	/// </summary>
	bool isEscaped = false;

	/// <summary>
	/// Adds the current Token to the vector of Tokens and resets the current Token
	/// </summary>
	void CloseToken();
	/// <summary>
	/// Throws errors that are encountered in the lexing process and sets the lexer state to error
	/// </summary>
	/// <param name="msg">The message to throw as the error</param>
	void ThrowLexerSyntaxError(std::string msg);
};
