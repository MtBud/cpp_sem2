#include <string>

class CLogger{
public:
    void log ( std::string );

private:
    void database ( std::string );

    void file ( std::string );

    void terminal ( std::string );
};