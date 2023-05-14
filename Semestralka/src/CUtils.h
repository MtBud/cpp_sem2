#pragma once
#include <string>

class CUtils{
public:
    virtual void launch( const std::string& argument ) = 0;
};

class CDisplay : public CUtils{
public:
    void launch( const std::string& argument ) override;
};

class CContent : public CUtils{
public:
    void launch( const std::string& argument ) override ;
};

class CExecute : public CUtils{
public:
    void launch( const std::string& argument ) override;
};

class CChangeConfig : public CUtils{
public:
    void launch( const std::string& argument ) override;
};