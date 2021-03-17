#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using std::string;

string read_file(fs::path path);
string trim(string str);
