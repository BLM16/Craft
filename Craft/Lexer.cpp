#include "Lexer.h"

/// <summary>
/// The function that tokenizes the craftfile data
/// </summary>
/// <param name="data">The data to tokenize</param>
/// <returns>A vector of Tokens</returns>
std::vector<Token> Lexer::parse(std::string data)
{
	// Loop through every character in the data
	for (char c : data)
	{
		// Determine the type of token the character is part of
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
					// A comment has been reached so we close the previous token and comment the rest of the line
					CloseToken();
					currentToken.Type = TokenType::COMMENT;
				}
			}
			else if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
			{
				// Token is either a string literal or a comment already so we just append the # to the current token
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
					// Token is a variable so we set it to be a variable and close the token
					currentToken.Type = TokenType::VARIABLE;
					CloseToken();
				}
			}
			else if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
				// Current token is a string literal or a comment so we just append the = to the current token
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
					// Token is a function so we set it to be a function and close the token
					currentToken.Type = TokenType::FUNCTION;
					CloseToken();
				}
			}
			else if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
				// Current token is a string literal or a comment so we just append the : to the current token
				currentToken.Value.append(1, c);
			else
				ThrowLexerSyntaxError("Invalid use of character \":\"");
			break;

		case '!':
			if (currentToken.Type == TokenType::STRING_LITERAL || currentToken.Type == TokenType::COMMENT)
				// Current token is a string literal or a comment so we just append the ! to the current token
				currentToken.Value.append(1, c);
			else if (currentToken.Type != TokenType::UNSET)
				ThrowLexerSyntaxError("Invalid use of character \"!\"");
			else
				// Token is a function call
				currentToken.Type = TokenType::FUNCTION_CALL;
			break;

		case ' ':
		case '\t':
			if (currentToken.Type != TokenType::STRING_LITERAL && currentToken.Type != TokenType::COMMENT && currentToken.Type != TokenType::UNSET)
				// Whitespace has been reached where the current token is not a string literal or a comment or unset so we close the token
				CloseToken();
			else
				// Current token is a string or comment so we append the space to it
				currentToken.Value.append(1, c);
			break;

		case '\n':
			if (isEscaped)
				// Newline is escaped so we just treat this as if the next line is on the current line
				isEscaped = false;
			else
				// Newline is not escaped meaning the current token is closed
				CloseToken();
			// Increment the line number used for debugging and error logging
			lineNumber++;
			break;

		default:
			// The character is not one of the "keywords" or reserved tokens so we append it to the current token
			currentToken.Value.append(1, c);
			break;
		}
	}

	return Tokens;
}

/// <summary>
/// Adds the current Token to the vector of Tokens and resets the current Token
/// </summary>
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

/// <summary>
/// Throws errors that are encountered in the lexing process and sets the lexer state to error
/// </summary>
/// <param name="msg">The message to throw as the error</param>
void Lexer::ThrowLexerSyntaxError(std::string msg) {
	STATE = LexerState::ERROR;
	std::cerr << "Syntax Error @ln:" << lineNumber << "\n\tError: " << msg << std::endl;
}
