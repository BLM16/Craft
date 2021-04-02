#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

// Simplify the namespace for std::filesystem
namespace fs = std::filesystem;

/// <summary>
/// Reads a file by path into a string
/// </summary>
/// <param name="path">The path of the file to read</param>
/// <returns>A string of the file contents</returns>
std::string read_file(fs::path path);
/// <summary>
/// Trims whitespace off the start and end of a given string
/// </summary>
/// <param name="str">String to trim</param>
/// <returns>The trimmed string</returns>
std::string trim(std::string str);
