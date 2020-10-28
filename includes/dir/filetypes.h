#ifndef __FILETYPES_H__
#define __FILETYPES_H__

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

namespace dir {

typedef enum filetype {
    JPG,
    PNG,
    WEBP,

    BMP,

    GIF,

    UNKNOWN,
} filetype;

/**
 * Checks the true filetype based off of the file signature
 * instead of the extension.
 */
filetype try_parse_filetype(const fs::path& path);
std::string expected_extension(const fs::path& path);
bool does_filetype_match_extension(const fs::path& path);
bool is_image(filetype type);

}

#endif
