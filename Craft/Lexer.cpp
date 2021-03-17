#include "Lexer.h"

std::vector<Token> Lexer::parse(std::string data)
{
	for (char c : data)
	{
		switch (c)
		{
		case '\\':
			if (!isEscaped)
				isEscaped = true;
			else if (currentToken.Type != TokenType::UNSET)
			{
				// Real backslash character is being used
				currentToken.Value.append(1, c);
				isEscaped = false;
			}
			else
				ThrowLexerSyntaxError("Invalid character escape");
			break;

		case '#':
			if (!isEscaped && currentToken.Type == TokenType::UNSET)
			{
				if (currentToken.Value == "")
					currentToken.Type = TokenType::COMMENT;
				else
				{
					CloseToken();
					currentToken.Type = TokenType::COMMENT;
				}
			}
			else if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
			{
				if (isEscaped)
					isEscaped = false;
				currentToken.Value.append(1, c);
			}
			else
			{
				if (isEscaped)
					isEscaped = false;
				ThrowLexerSyntaxError("Invalid use of character \"#\"");
			}
			break;

		case '\"':
			if (currentToken.Type == TokenType::UNSET)
			{
				if (!isEscaped)
					// Token is unset so make it a string
					currentToken.Type = TokenType::STRING_LITERAL;
				else
					ThrowLexerSyntaxError("Invalid character escape");
			}
			else if (currentToken.Type == TokenType::STRING_LITERAL)
			{
				if (isEscaped)
				{
					// Treat as character not end of string literal
					currentToken.Value.append(1, c);
					isEscaped = false;
				}
				else
					// The string is ended
					CloseToken();
			}
			break;

		case '=':
			if (currentToken.Type == TokenType::UNSET)
			{
				if (isEscaped)
				{
					ThrowLexerSyntaxError("Invalid character escape");
					isEscaped = false;
				}
				else
				{
					currentToken.Type = TokenType::VARIABLE;
					CloseToken();
				}
			}
			else if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
				currentToken.Value.append(1, c);
			else
				ThrowLexerSyntaxError("Invalid use of character \"=\"");
			break;

		case ':':
			if (currentToken.Type == TokenType::UNSET)
			{
				if (isEscaped)
				{
					ThrowLexerSyntaxError("Invalid character escape");
					isEscaped = false;
				}
				else
				{
					currentToken.Type = TokenType::FUNCTION;
					CloseToken();
				}
			}
			else if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
				currentToken.Value.append(1, c);
			else
				ThrowLexerSyntaxError("Invalid use of character \":\"");
			break;

		case '!':
			if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
				currentToken.Value.append(1, c);
			else if (currentToken.Type != TokenType::UNSET)
				ThrowLexerSyntaxError("Invalid use of character \"!\"");
			else
				currentToken.Type = TokenType::FUNCTION_CALL;
			break;

		case ' ':
		case '\t':
			if (currentToken.Type != TokenType::STRING_LITERAL && currentToken.Type != TokenType::COMMENT && currentToken.Type != TokenType::UNSET)
				CloseToken();
			else
				// Token is a string or comment so the space is part of it
				currentToken.Value.append(1, c);
			break;

		case '\n':
			if (isEscaped)
				isEscaped = false;
			else
				CloseToken();
			lineNumber++;
			break;

		default:
			currentToken.Value.append(1, c);
			break;
		}
	}

	return Tokens;
}

void Lexer::CloseToken()
{
	// Type has not been determined, therefore it is a command
	if (currentToken.Type == TokenType::UNSET)
		currentToken.Type = TokenType::COMMAND;

	// Set the line number on the token
	currentToken.LineNumber = lineNumber;
	// Trim the whitespaces
	currentToken.Value = trim(currentToken.Value);

	// Push the token to the list
	Tokens.push_back(currentToken);

	// Reset the current token
	currentToken.Type = TokenType::UNSET;
	currentToken.Value.erase();
}

void Lexer::ThrowLexerSyntaxError(std::string msg) {
	STATE = LexerState::ERROR;
	std::cerr << "Syntax Error @ln:" << lineNumber << "\n\tError: " << msg << std::endl;
}
