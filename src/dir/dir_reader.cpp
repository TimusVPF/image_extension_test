#include "dir/dir_reader.h"

namespace dir {

bool default_filter(const fs::path& path)
{
    return true;
}

std::vector<fs::path> list_entry_paths(const fs::path& path, const bool recursive,
    const bool print_directories, bool (*filter)(const fs::path& path))
{
    std::vector<fs::path> paths;

    for (const auto& entry : fs::directory_iterator(path)) {
        const bool is_directory = entry.is_directory();
        if (recursive && is_directory) {
            const std::vector<fs::path> additional_entries = list_entry_paths(entry, recursive, print_directories, filter);
            paths.reserve(paths.size() + additional_entries.size());
            paths.insert(paths.end(), additional_entries.begin(), additional_entries.end());
        }
        if ((print_directories || !is_directory) && filter(entry)) {
            paths.push_back(entry.path());
        }
    }

    return paths;
}

std::vector<fs::path> list_entry_paths(const bool recursive, const bool print_directories, bool (*filter)(const fs::path& path))
{
    return list_entry_paths(fs::current_path(), recursive, print_directories, filter);
}

std::vector<fs::path> list_entry_paths(const bool recursive, const bool print_directories)
{
    return list_entry_paths(fs::current_path(), recursive, print_directories, default_filter);
}

}
