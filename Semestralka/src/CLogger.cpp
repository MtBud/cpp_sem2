#include <string>
#include <iostream>
#include "CLogger.h"

void CLogger::log ( const std::string& message){
    std::cout << message << std::endl;
}

/*
void CLogger::database ( const std::string& message );

void CLogger::file ( const std::string& message );

void CLogger::terminal ( const std::string& message );
*/