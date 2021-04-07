#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "Node.h"

/// <summary>
/// The states of the interpreter
/// </summary>
enum class InterpreterState : short
{
	OK,
	ERROR
};

/// <summary>
/// Used to interpret and execute commands from nodes
/// </summary>
class Interpreter
{
public:
	/// <summary>
	/// The function that executes the desired function from the craftfile
	/// </summary>
	/// <param name="function">The name of the function to execute</param>
	/// <param name="nodes">The node tree for the craftfile</param>
	void execute(std::string function, std::vector<Node> nodes);
	/// <summary>
	/// Gets the interpreter instance's state
	/// </summary>
	/// <returns>InterpreterState of the interpreter instance</returns>
	InterpreterState getState() { return STATE; };

private:
	/// <summary>
	/// The interpreter instance's current state
	/// </summary>
	InterpreterState STATE { InterpreterState::OK };

	/// <summary>
	/// Checks if a terminal exists that we can use
	/// </summary>
	/// <returns>Whether or not there is a terminal we can use</returns>
	bool CheckTerminalExists();
	/// <summary>
	/// Throws errors that are encountered by the interpreter and sets the interpreter state to error
	/// </summary>
	/// <param name="msg">The message to throw as the error</param>
	void ThrowInterpreterSyntaxError(std::string msg);
};
