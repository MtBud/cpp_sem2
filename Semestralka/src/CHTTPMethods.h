#pragma once
#include <string>

class CHTTPMethods{
public:
    virtual void incoming( const std::string& method);
};

class CGet : public CHTTPMethods{
public:
    void incoming( const std::string& method) override;
};

class CPost : public CHTTPMethods{
public:
    void incoming( const std::string& method) override;
};

class CPut : public CHTTPMethods{
public:
    void incoming( const std::string& method) override;
};

class CDelete : public CHTTPMethods{
public:
    void incoming( const std::string& method) override;
};