#pragma once

#include <vector>
#include <string>

struct Node
{
public:
	std::string Name;
	std::vector<Node> Nodes;

	// Used to find Nodes by name in a vector
	bool operator==(std::string n)
	{
		return this->Name == n;
	}
};
