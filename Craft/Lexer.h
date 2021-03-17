#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Token.h"
#include "Utils.h"

// Used to determine whether to continue or not after lexing
enum class LexerState
{
	OK,
	ERROR
};

class Lexer
{
public:
	std::vector<Token> parse(std::string data);
	LexerState getState() { return STATE; };

private:
	LexerState STATE { LexerState::OK };

	void CloseToken();
	void ThrowLexerSyntaxError(std::string msg);
	
	std::vector<Token> Tokens;
	Token currentToken;
	int lineNumber { 1 };

	// Is the current character escaped with a backslash
	bool isEscaped = false;
};
