#include "main.h"

bool recursive = false;
bool print_directories = false;
bool filter_images = false;

void mark_flags(int argc, const char* argv[])
{
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--recursive") == 0) {
            recursive = true;
        }
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--print-directories") == 0) {
            print_directories = true;
        }
        if (strcmp(argv[i], "-fi") == 0 || strcmp(argv[i], "--filter-images") == 0) {
            filter_images = true;
        }
    }
}

bool is_image(const fs::path& path)
{
    dir::filetype type = dir::try_parse_filetype(path);
    return dir::is_image(type);
}

void print_mismatched_extensions(const std::vector<fs::path> paths)
{
    for (auto& entry : paths) {
        if (!dir::does_filetype_match_extension(entry)) {
            const std::string extension = entry.has_extension()
                ? entry.extension().generic_string()
                : "none";
            std::cout << "\033[1;31mMismatch Found: \033[0m"
                      << "File " << entry.filename() << " has mismatched extension. "
                      << "Expected any of: "
                      << "\033[1;31m" << dir::expected_extension(entry) << "\033[0m"
                      << ", found "
                      << "\033[1;31m" << extension << "\033[0m." << std::endl;
        } else {
            std::cout << "\033[1;32mFile found:\033[0m " << entry.generic_string() << std::endl;
        }
    }
}

int main(int argc, const char* argv[])
{
    mark_flags(argc, argv);

    std::vector<fs::path> paths;
    if (filter_images) {
        paths = dir::list_entry_paths(recursive, print_directories, is_image);
    } else {
        paths = dir::list_entry_paths(recursive, print_directories);
    }
    print_mismatched_extensions(paths);

    return 0;
}
