#pragma once
#include <string>

class CResolver{
public:
    // accept the messege, check format and decide what to do
    void incoming(std::string);

private:
    // handles GET method
    void get();

    // handles POST method
    void post();

    // handles DELETE method
    void deleteM();
};