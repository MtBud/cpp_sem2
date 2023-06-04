#include <ostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <map>
#include <algorithm>
#include <iostream>
#include "CGetFormats.h"
#include "util.h"

CGetFormats::~CGetFormats() = default;


std::stringstream& CDir::compose( std::filesystem::path& path, std::stringstream& message ){
    message << "Content-Type: text/plain; charset=UTF-8" << "\r\n";
    std::stringstream content;
    list( path, content );
    message << "Content-Length: " << content.str().length() << "\r\n";
    message << "\r\n";
    message << content.str();
    return message;
}


std::stringstream& CText::compose( std::filesystem::path& path, std::stringstream& message ){
    std::string extension( path.extension() );
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower );

    if( extension == ".txt" )
        message << "Content-Type: text/plain; charset=UTF-8" << "\r\n";
    else
        message << "Content-Type: text/" << extension.substr(1) << "; charset=UTF-8" << "\r\n";

    return addContent( path, message );
}

std::stringstream& CImage::compose( std::filesystem::path& path, std::stringstream& message ){
    std::string extension( path.extension() );
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower );

    if( extension == ".jpg" )
        extension = ".jpeg";

    message << "Content-Type: image/" << extension.substr(1) << "\r\n";

    return addContent( path, message );
}

std::stringstream& CVideo::compose( std::filesystem::path& path, std::stringstream& message ){
    std::string extension( path.extension() );
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower );

    message << "Content-Type: video/" << extension.substr(1) << "\r\n";

    return addContent( path, message );
}

//----------------------------------------------------------------------------------------------------------------------

std::stringstream& CGetFormats::addContent( std::filesystem::path& path, std::stringstream& message ){
    std::ifstream ifs(path);
    std::string content = std::string( (std::istreambuf_iterator<char>(ifs) ),
                                       (std::istreambuf_iterator<char>() ) );
    ifs.close();
    message << "Content-Length: " << content.length() << "\r\n";
    message << "\r\n";
    message << content;
    return message;
}
