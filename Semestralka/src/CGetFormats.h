#pragma once
#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <filesystem>

class CGetFormats{
public:
    virtual std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) = 0;

    virtual ~CGetFormats();

    static std::stringstream& addContent( std::filesystem::path& path, std::stringstream& message );
};

class CDir : public CGetFormats{
public:
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};

class CText : public CGetFormats{
public:
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};

class CImage : public CGetFormats{
public:
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};

class CVideo : public CGetFormats{
public:
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};