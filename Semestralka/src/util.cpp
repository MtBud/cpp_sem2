#include <ostream>
#include <iostream>
#include <filesystem>
#include <string>
#include "CConfig.h"
#include "util.h"


std::stringstream& list( std::filesystem::path directory, std::stringstream & out ){
    CConfig conf;
    size_t length = out.str().length();
    directory = std::string(conf.data["root"]) + directory.native();
    if( ! std::filesystem::exists(directory) ){
        return out;
    }

    for( const auto &entry: std::filesystem::directory_iterator(directory) ){
        std::string currdir = entry.path();
        out << currdir.substr(currdir.find_last_of('/') + 1) << std::endl;
    }

    if( length == out.str().length())
        out << "Directory is empty" << std::endl;

    return out;
}


//----------------------------------------------------------------------------------------------------------------------

std::vector<std::string> parse( std::string data, const std::string& delimiter ){
    std::vector<std::string> parsed;
    size_t delimSize = delimiter.size();
    while(true){
        size_t pos = data.find(delimiter);
        if(pos == std::string::npos){
            if( ! data.empty() )
                parsed.push_back(data);
            break;
        }
        parsed.push_back(data.substr(0,pos));
        data.erase(0,pos + delimSize);
    }

    return parsed;
}