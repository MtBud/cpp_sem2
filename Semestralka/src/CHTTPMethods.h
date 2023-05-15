#pragma once
#include <string>

class CHTTPMethods{
public:
    virtual void incoming( const std::string& request);
};

class CGet : public CHTTPMethods{
public:
    void incoming( const std::string& request) override;
};

class CPost : public CHTTPMethods{
public:
    void incoming( const std::string& request) override;
};

class CPut : public CHTTPMethods{
public:
    void incoming( const std::string& request) override;
};

class CDelete : public CHTTPMethods{
public:
    void incoming( const std::string& request) override;
};