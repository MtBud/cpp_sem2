#pragma once
#include <filesystem>
#include "../libs/json.hpp"

struct CConfig{

    nlohmann::json data;
    CConfig();

    void checkCorrectness();
    static void isRelative( std::filesystem::path& path );
};
