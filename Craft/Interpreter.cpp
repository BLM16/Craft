#include "Interpreter.h"

/// <summary>
/// The function that executes the desired function from the craftfile
/// </summary>
/// <param name="function">The name of the function to execute</param>
/// <param name="nodes">The node tree for the craftfile</param>
void Interpreter::execute(std::string function, std::vector<Node> nodes)
{
	// Make sure we have a valid terminal to execute commands on
	if (!CheckTerminalExists())
		return ThrowInterpreterSyntaxError("No terminal exists to run shell commands");

	// Loop through every node in the vector of Nodes
	for (Node node : nodes)
	{
		// Determine how to deal with each node
		switch (node.Type)
		{
		case NodeType::COMMAND:
			// Run the command on a shell
			system(node.Name.c_str());
			break;

		case NodeType::FUNCTION:
			// Run all the commands in the function if it is the called function
			if (node.Name == function)
				execute("", node.Nodes);
			break;

		default:
			ThrowInterpreterSyntaxError("Unexpected node type encountered");
			break;
		}
	}
}

/// <summary>
/// Checks if a terminal exists that we can use
/// </summary>
/// <returns>Whether or not there is a terminal we can use</returns>
bool Interpreter::CheckTerminalExists()
{
	return system(NULL);
}

/// <summary>
/// Throws errors that are encountered by the interpreter and sets the interpreter state to error
/// </summary>
/// <param name="msg">The message to throw as the error</param>
void Interpreter::ThrowInterpreterSyntaxError(std::string msg)
{
		STATE = InterpreterState::ERROR;
		std::cerr << "Interpreter Error: " << msg << std::endl;
}
