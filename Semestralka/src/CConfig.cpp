#include <fstream>
#include "CConfig.h"


CConfig::CConfig(){
    std::ifstream f("src/config.json");
    data = nlohmann::json::parse(f);
}