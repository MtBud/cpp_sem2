/**
 * @file util.h
 * @brief Miscelaneous utility functions
 *
 * Header contains supportive functions that aren't tied to any specific class.
 */

#pragma once
#include <string>
#include <ostream>
#include <filesystem>
#include <vector>

/**
 * @brief List directory contents
 *
 * @param directory Path to the directory that should be listed
 * @param out Stream that the directory contents should be appended to.
 * @return Return unedited stream if the directory doesn't exist. If the directory is empty, add the message
 *         "Directory is empty". Otherwise list the directory contents separated by newline.
 */
std::stringstream& list( std::filesystem::path directory, std::stringstream& out );

/**
 * @brief Splits a string into a vector of strings based on "delimiter"
 *
 * @param data String to be split
 * @param delimiter Delimiter used for splitting
 * @return Vector of strings, without the delimiters.
 */
std::vector<std::string> parse( std::string data, const std::string& delimiter );
