#pragma once
#include <string>
#include <ostream>
#include <filesystem>
#include <vector>

std::stringstream& list( std::filesystem::path directory, std::stringstream& out );
