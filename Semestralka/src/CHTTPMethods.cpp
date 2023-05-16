#include <iostream>
#include "CHTTPMethods.h"
#include "CUtils.h"

void CGet::incoming( const std::string& request){
    std::cout << "CGet: " << request << std::endl;
}

void CPost::incoming( const std::string& request){

}

void CPut::incoming( const std::string& request){

}

void CDelete::incoming( const std::string& request){

}
