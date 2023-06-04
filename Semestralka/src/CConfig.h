#pragma once
#include <filesystem>
#include "../libs/json.hpp"
/**
 * @class CConfig
 * @brief A class handling the server configuration.
 *
 * CConfig is a simple class. Its purpose is to make configuration variables easily accessible anywhere by
 * creating an instance of the class.
 */
struct CConfig{
    /**
     * @brief A constant path to the configuration file
     */
    const std::string CONFIG_PATH = "src/config.json";

    /**
     * @brief Configuration data
     */
    nlohmann::json data;

    /**
     * @brief A constructor
     *
     * Reads the configuration file and parses the contents.
     */
    CConfig();

    /**
     * @brief Check if configuration contains correct information
     *
     * Function is checking if the config file contains all needed configuration.
     * ***********************************************************************************
     * Configuration file must contain:
     *
     * json object - "network"
     *      - "port": port used for binding
     *      - "address": IPv4 address of the network interface
     *
     * json object - "log"
     *      - "path": path to the log file, can be an empty string for unspecified path
     *      - "console": boolean value signaling if the output should be sent to console or not
     *      - "file": boolean value signaling if the output should be sent to a log file or not
     *
     * "root": root directory of the server filesystem
     * "post": default directory for posting files
     * "shutdown": address used for remote server shutdown
     * "password": admin password used for authentication
     * ***********************************************************************************
     * Configuration file may contain:
     * json array - "address-mapping" Every entry is an array of two paths. Entries must be sorted in a descending order
     *                                based on the number of directories in the first path.
     *      - first: path to be substituted
     *      - second: substitution path
     *
     * json array - "restricted" Array contains paths to directories that are restricted and need authentication
     *
     * json object - "script-execution" Entries have file extensions as their names and the value is a shell command
     *                used to run said script. A shell command has to contain $filename. $filename is going to be
     *                substituded by the path to the script.
     *
     * json object - "file-extensions" Contains file extensions and their associated datatype. Is used to check
     *               for datatype support and filling http headers.
     */
    void checkCorrectness();

    /**
     * @brief Check if a path is relative.
     *
     * @param path to check
     * @return Function doesn't return anything. If the path is relative it throws a std::runtime_error.
     */
    static void isRelative( std::filesystem::path& path );
};
