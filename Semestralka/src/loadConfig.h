#pragma once
#include <string>
#include <map>

class CConfig{
    std::map < std::string, std::string >;
    std::map < std::string, int >;

public:
    // constructor fills the maps with config data
    CConfig();

    template <typename T>
    T option ( std::string );

private:
    void fillMaps();
};