#include <ostream>
#include <iostream>
#include <filesystem>
#include <string>
#include "CConfig.h"
#include "CUtils.h"


std::stringstream& list( std::filesystem::path directory, std::stringstream & out ){
    CConfig conf;
    directory = std::string(conf.data["root"]) + directory.native();
    if( ! std::filesystem::exists(directory) ){
        return out;
    }

    for( const auto &entry: std::filesystem::directory_iterator(directory) ){
        std::string currdir = entry.path();
        out << currdir.substr(currdir.find_last_of('/') + 1) << std::endl;
    }
    return out;
}
