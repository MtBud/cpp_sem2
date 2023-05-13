#pragma once
#include <string>

class CLogger{
public:
    // function only acts as a switch to call one or more of the methods
    void log ( const std::string& message );


private:
    void database ( const std::string& message );

    void file ( const std::string& message );

    void terminal ( const std::string& message );
};