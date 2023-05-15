#include <ostream>
#include <iostream>
#include <filesystem>
#include <string>
#include "CConfig.h"
#include "CUtils.h"

void CDisplay::launch( const std::vector<std::string>& arguments, std::ostream& out ){
    std::cout << arguments[1] << std::endl;
}

// ----------------------------------------------------------------------

void CContent::launch( const std::vector<std::string>& arguments, std::ostream& out ){
    if( arguments.size() > 2 ){
        out << "Too many arguments" << std::endl;
        return;
    }

    CConfig conf;
    std::string root = conf.data["root"];

    // print server root directory
    if( arguments.size() == 1){
        list(root, "", out);
        return;
    }

    // print directory specified in the argument
    list(root, arguments[1], out);
}

void CContent::list( const std::string& root, const std::string& directory, std::ostream& out ){
    try {
        for( const auto &entry: std::filesystem::directory_iterator(root + directory) ){
            std::string currdir = entry.path();
            std::cout << currdir.substr(currdir.find_last_of('/') + 1) << std::endl;
        }
    }
    catch( const std::exception& e ){
        std::cout << "No such file or directory" << std::endl;
    }
}

// ----------------------------------------------------------------------



void CExecute::launch( const std::vector<std::string>& arguments, std::ostream& out ){
    return;
}

void CChangeConfig::launch( const std::vector<std::string>& arguments, std::ostream& out ){
    return;
}