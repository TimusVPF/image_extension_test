#include "dir/filetypes.h"

#define BUFFER_CAPACITY 20

namespace dir {

std::unordered_map<dir::filetype, std::string> extension_map = {
    { JPG, ".jpg|.jpeg|.jpe|.jif|.jfif|.jfi" },
    { PNG, ".png|.apng" },
    { WEBP, ".webp|.riff" },
    { BMP, ".bmp|.dib" },
    { GIF, ".gif" },
    { TIFF, ".tiff|.tif" },
    { UNKNOWN, "" },
};

bool is_jpg(unsigned char buffer[BUFFER_CAPACITY])
{
    return (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && buffer[3] == 0xE0) // JPG
        || (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && buffer[3] == 0xE1) // JPG EXIF
        || (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && buffer[3] == 0xE8); // JPG SPIFF
}

bool is_png(unsigned char buffer[BUFFER_CAPACITY])
{
    return buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4E && buffer[3] == 0x47
        && buffer[4] == 0x0D && buffer[5] == 0x0A && buffer[6] == 0x1A && buffer[7] == 0x0A;
}

bool is_webp(unsigned char buffer[BUFFER_CAPACITY])
{
    return buffer[0] == 0x52 && buffer[1] == 0x49 && buffer[2] == 0x46 && buffer[3] == 0x46
        && buffer[8] == 0x57 && buffer[9] == 0x45 && buffer[10] == 0x50;
}

bool is_bmp(unsigned char buffer[BUFFER_CAPACITY])
{
    return buffer[0] == 0x42 && buffer[1] == 0x4D;
}

bool is_gif(unsigned char buffer[BUFFER_CAPACITY])
{
    return (buffer[0] == 0x47 && buffer[1] == 0x49 && buffer[2] == 0x46 && buffer[3] == 0x38 && buffer[4] == 0x37 && buffer[5] == 0x61)
        || (buffer[0] == 0x47 && buffer[1] == 0x49 && buffer[2] == 0x46 && buffer[3] == 0x38 && buffer[4] == 0x39 && buffer[5] == 0x61);
}

bool is_tiff(unsigned char buffer[BUFFER_CAPACITY])
{
    return (buffer[0] == 0x49 && buffer[1] == 0x49 && buffer[2] == 0x2A && buffer[3] == 0x00) // Little Endian
        || (buffer[0] == 0x4D && buffer[1] == 0x4D && buffer[2] == 0x00 && buffer[3] == 0x2A); // Big Endian
}

filetype try_parse_filetype(const fs::path& path)
{
    unsigned char buffer[BUFFER_CAPACITY];
    std::ifstream file_stream(path.generic_u8string(), std::ifstream::in | std::ifstream::binary);
    file_stream.read((char*)buffer, BUFFER_CAPACITY);
    file_stream.close();

    if (is_jpg(buffer)) {
        return JPG;
    }
    if (is_png(buffer)) {
        return PNG;
    }
    if (is_webp(buffer)) {
        return WEBP;
    }
    if (is_bmp(buffer)) {
        return BMP;
    }
    if (is_gif(buffer)) {
        return GIF;
    }
    if (is_tiff(buffer)) {
        return TIFF;
    }

    return UNKNOWN;
}

bool is_image(filetype type)
{
    switch (type) {
    case JPG:
    case PNG:
    case WEBP:
    case BMP:
    case GIF:
    case TIFF:
        return true;
    default:
        return false;
    }
}

std::string expected_extension(const fs::path& path)
{
    const filetype type = try_parse_filetype(path);
    if (extension_map.find(type) != extension_map.end()) {
        return extension_map[type];
    }
    return "";
}

bool does_filetype_match_extension(const fs::path& path)
{
    std::string expected = expected_extension(path);
    return expected.length() <= 0
        || expected.find(path.extension().generic_string()) != std::string::npos;
}

}
