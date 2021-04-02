#pragma once

#include <vector>
#include <string>

/// <summary>
/// Node holds command values, or sub-nodes if the node is a function
/// </summary>
struct Node
{
public:
	/// <summary>
	/// Node instance's name or value
	/// </summary>
	std::string Name;
	/// <summary>
	/// Vector of sub-nodes used if the node instance is a function
	/// </summary>
	std::vector<Node> Nodes;

	/// <summary>
	/// Compares a string to the node instance's name for use in finding the node by name in vectors
	/// </summary>
	/// <param name="n">String to compare to the node instance's name</param>
	/// <returns>Does the string match the node instance's name</returns>
	bool operator==(std::string n)
	{
		return this->Name == n;
	}
};
