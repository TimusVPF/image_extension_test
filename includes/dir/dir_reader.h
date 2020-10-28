#ifndef __DIR_READER_H__
#define __DIR_READER_H__

#include <filesystem>
#include <string>
#include <vector>

#include "filetypes.h"

namespace fs = std::filesystem;

namespace dir {

/**
 * A default filter that always returns true - this is used
 * if there shouldn't be any file filtration whatsoever.
 */
bool default_filter(const fs::path& path);
/**
 * Lists all files within a designated path with a
 * custom filtering function.
 */
std::vector<fs::path> list_entry_paths(const fs::path& path, const bool recursive, const bool print_directories, bool (*filter)(const fs::path&));
/**
 * Lists all files within the current executable directory with
 * a custom filtering function.
 */
std::vector<fs::path> list_entry_paths(const bool recursive, const bool print_directories, bool (*filter)(const fs::path&));
/**
 * Lists all files within the current executable directory with
 * no filtering function used (uses `default_filter`).
 */
std::vector<fs::path> list_entry_paths(const bool recursive, const bool print_directories);

}

#endif
