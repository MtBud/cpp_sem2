#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <filesystem>
#include "CHTTPMethods.h"

class CGetFormats{
public:
    std::stringstream& composeWrapper( std::filesystem::path path, std::stringstream& message );
private:
    virtual std::stringstream& compose( std::filesystem::path path, std::stringstream& message ) = 0;
};

class CText : public CGetFormats{
private:
    std::stringstream& compose( std::filesystem::path path, std::stringstream& message ) override;
};

class CImage : public CGetFormats{
private:
    std::stringstream& compose( std::filesystem::path path, std::stringstream& message ) override;
};

class CVideo : public CGetFormats{
private:
    std::stringstream& compose( std::filesystem::path path, std::stringstream& message ) override;
};