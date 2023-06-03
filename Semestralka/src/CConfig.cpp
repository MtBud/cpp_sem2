#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <string>
#include <ctime>
#include <climits>
#include <vector>
#include <exception>
#include "CConfig.h"
#include "../libs/json.hpp"


CConfig::CConfig(){
    std::filesystem::path path = "src/config.json";
    if( ! exists(path) ){
        throw std::runtime_error("Config file doesn't exist");
    }
    std::ifstream f("src/config.json");
    data = nlohmann::json::parse(f);
}

void CConfig::checkCorrectness(){
    int tmpInt = data["network"]["port"];
    if( tmpInt < 0 || tmpInt > 65535 ){
        throw std::runtime_error("Port number out of range");
    }
    std::string tmpStr = data["network"]["address"];
    std::filesystem::path tmpPath1 = data["root"];
    std::vector < std::filesystem::path > pathVector;
    pathVector.push_back( data["post"] );
    pathVector.push_back( data["scripts"] );
    pathVector.push_back( data["shutdown"] );
    pathVector.push_back( data["password"] );


    int depth = PATH_MAX;
    for( auto& i : data["address-mapping"] ){
        int currDepth = 0;
        std::filesystem::path tmpPath = i[0];
        pathVector.push_back( i[1] );
        while( tmpPath.native().length() != 1 ) {
            currDepth++;
            std::cout << "loop" << std::endl;
            tmpPath = tmpPath.parent_path();
        }
        bool flag = false;
        for( auto& j : data["address-mapping"] ){
            if( flag && i[0] == j[0] )
                throw std::runtime_error("Two same entries in address-mapping");
            if( i[0] == j[0] )
                flag = true;
        }
        if( currDepth > depth )
            throw std::runtime_error("Address mapping has wrong format");
        depth = currDepth;
        pathVector.push_back( tmpPath );
    }

    for( auto& i : data["restricted"]){
        pathVector.push_back(i);
    }

    for( auto& i : data["script-execution"]){
        tmpStr += i;
    }

    for( auto& i : data["file-extensions"]){
        tmpStr += i;
    }

    pathVector.push_back( data["log"]["path"] );

    for( auto& i : pathVector )
        isRelative(i);
    bool tmpBool = data["log"]["console"] && data["log"]["file"];

    if( tmpBool )
        return;
}

void CConfig::isRelative( std::filesystem::path& path ){
    if( path.native()[0] == '.' )
        throw std::runtime_error("Path cannot be relative");
}