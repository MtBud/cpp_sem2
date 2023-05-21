#pragma once
#include <string>
#include <vector>

class CUtils{
public:
    virtual void launch( const std::vector<std::string>& arguments, std::ostream& out ) = 0;
};

class CDisplay : public CUtils{
public:
    void launch( const std::vector<std::string>& arguments, std::ostream& out ) override;
};

class CContent : public CUtils{
public:
    void launch( const std::vector<std::string>& arguments, std::ostream& out ) override ;
    static void list( const std::string& root, const std::string& directory, std::ostream& out );
};

class CExecute : public CUtils{
public:
    void launch( const std::vector<std::string>& arguments, std::ostream& out ) override;
};

class CChangeConfig : public CUtils{
public:
    void launch( const std::vector<std::string>& arguments, std::ostream& out ) override;
};