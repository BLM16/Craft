#include "Utils.h"

/// <summary>
/// Reads a file by path into a string
/// </summary>
/// <param name="path">The path of the file to read</param>
/// <returns>A string of the file contents</returns>
std::string read_file(fs::path path)
{
	// Make sure the file exists
	if (!fs::exists(path) || !fs::is_regular_file(path))
	{
		std::cerr << "File: " << path << " does not exist." << std::endl;
		return "";
	}

	// Define storage variables for the data
	std::string data;
	std::string line;

	// Open the file
	std::ifstream file(path);

	// Make sure the file opened without error
	if (file.is_open())
	{
		// Read through every line in the file and append it to data
		while (std::getline(file, line))
			data += line + "\n";

		// Close the file
		file.close();
	}

	return data;
}

/// <summary>
/// Trims whitespace off the start and end of a given string
/// </summary>
/// <param name="str">String to trim</param>
/// <returns>The trimmed string</returns>
std::string trim(std::string str)
{
	// Define what chars to remove
	const char* whitespaceToRemove = " \t";

	// Erase all the leading and trailing chars defined in whitespaceToRemove
	str.erase(str.find_last_not_of(whitespaceToRemove) + 1);
	str.erase(0, str.find_first_not_of(whitespaceToRemove));

	return str;
}
