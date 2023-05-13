#include <fstream>
#include "CConfig.h"


CConfig::CConfig(){
    std::ifstream f("config.json");
    data = nlohmann::json::parse(f);
}