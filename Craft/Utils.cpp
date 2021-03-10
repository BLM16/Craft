#include "Utils.h"

string read_file(fs::path path) {

	// Make sure the file exists
	if (!fs::exists(path) || !fs::is_regular_file(path)) {
		std::cerr << "File: " << path << " does not exist." << std::endl;
		return "";
	}

	// Define storage variables for the data
	string data;
	string line;

	// Open the file
	std::ifstream file(path);

	// Read through every line in the file and append it to data
	if (file.is_open()) {
		while (std::getline(file, line))
			data += line;

		file.close();
	}

	return data;
}
