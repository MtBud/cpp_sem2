#pragma once
#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <filesystem>

/**
 * @class CGetFormats
 * @brief Abstract class used for making classes handling different file types
 */
class CGetFormats{
public:
    /**
     * @brief A virtual destructor
     */
    virtual ~CGetFormats();

    /**
     * @brief Appends correct headers and file content to the HTTP response @see CHTTPMethods::incoming()
     *
     * @param path    Path to the file being sent
     * @param message Stream the headers and content should be written into
     * @return  Returns the "message" stream
     */
    virtual std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) = 0;

    /**
     * @brief Used for appending correct file length and content
     *
     * @param path    Path to the file being sent
     * @param message Stream the headers and content should be written into
     * @return Returns the "message" stream
     */
    static std::stringstream& addContent( std::filesystem::path& path, std::stringstream& message );
};

/**
 * @class CDir
 * @brief Used for assembling responses containing directory contents
 */
class CDir : public CGetFormats{
public:
    /**
     * @see CGetFormats::compose()
     */
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};

/**
 * @class CText
 * @brief Used for assembling responses containing text files
 */
class CText : public CGetFormats{
public:
    /**
     * @see CGetFormats::compose()
     */
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};

/**
 * @class CImage
 * @brief Used for assembling responses containing images
 */
class CImage : public CGetFormats{
public:
    /**
     * @see CGetFormats::compose()
     */
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};

/**
 * @class CVideo
 * @brief Used for assembling responses containing videos
 */
class CVideo : public CGetFormats{
public:
    /**
     * @see CGetFormats::compose()
     */
    std::stringstream& compose( std::filesystem::path& path, std::stringstream& message ) override;
};
