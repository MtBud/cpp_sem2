#pragma once
#include <string>

/**
 * @class CLogger
 * @brief Handles logging
 *
 * The class is fairly minimal and doesn't really have to be a class. It's defined as one for easier extension
 * in the future, if needed.
 */
class CLogger{
public:

    /**
     * Function sends "message" to the output defined in the config file.
     * @param message Log message
     */
    void static log ( const std::string& message );
};