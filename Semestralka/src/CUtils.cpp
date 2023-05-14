#include <iostream>
#include "CUtils.h"

void CDisplay::launch( const std::string& argument ){
    std::cout << argument << std::endl;
}

void CContent::launch( const std::string& argument ){
    std::cout << argument << std::endl;
}

void CExecute::launch( const std::string& argument ){
    std::cout << argument << std::endl;
}

void CChangeConfig::launch( const std::string& argument ){
    std::cout << argument << std::endl;
}