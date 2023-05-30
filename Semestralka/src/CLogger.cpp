#include <string>
#include <iostream>
#include <fstream>
#include "CLogger.h"
#include "CConfig.h"

void CLogger::log ( const std::string& message){
    CConfig conf;
    if( conf.data["log"]["console"][0] )
        std::cout << message << std::endl;

    if( conf.data["log"]["file"][0] ){
        std::ofstream ofs( std::string( conf.data["root"] ) + "/log/main.log"  );
        ofs << message << std::endl;
        ofs.close();
    }
}

// maybe add a database later