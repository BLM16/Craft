#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char **argv) {

	// Get the current path
	fs::path current_path = fs::current_path();

	// Check if the current path contains a craftfile
	bool path_has_craftfile = fs::exists(current_path / ".craft") ? fs::is_regular_file(current_path / ".craft") : false;
	if (!path_has_craftfile) {
		std::cout << "Craftfile does not exist in current directory." << std::endl;
		return 1;
	}

	// No function is specified
	if (argc == 1) {
		std::cout << "A function must be specified." << std::endl;
		return 1;
	}

	// Get the function to execute
	char* function_to_execue = argv[1];
}