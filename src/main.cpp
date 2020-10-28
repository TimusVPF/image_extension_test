#include "main.h"

bool recursive = false;
bool print_directories = false;
bool filter_images = false;
bool should_fix_extensions = false;

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
        if (strcmp(argv[i], "--fix-extensions") == 0) {
            should_fix_extensions = true;
        }
    }
}

bool is_image(const fs::path& path)
{
    dir::filetype type = dir::try_parse_filetype(path);
    return dir::is_image(type);
}

void handle_fix(const fs::path path, const std::string expected)
{
    if (!should_fix_extensions) {
        return;
    }

    std::cout << "\033[1;33mOperation started\033[0m: Attempting to fix mismatched filename " << path << std::endl;
    const std::string replacement = expected.substr(0, expected.find('|'));
    fs::path operable_path = path;
    fs::rename(path, operable_path.replace_extension(replacement));
    std::cout << "\033[1;32mSuccessfully changed the filename to " << path << ".\033[0m" << std::endl;
}

void print_mismatched_extensions(const std::vector<fs::path> paths)
{
    for (auto& entry : paths) {
        if (!dir::does_filetype_match_extension(entry)) {
            const std::string extension = entry.has_extension()
                ? entry.extension().generic_string()
                : "none";
            const std::string expected = dir::expected_extension(entry);
            std::cout << "\033[1;31mMismatch Found: \033[0m"
                      << "File " << entry.filename() << " has mismatched extension. "
                      << "Expected any of: "
                      << "\033[1;31m" << expected << "\033[0m"
                      << ", found "
                      << "\033[1;31m" << extension << "\033[0m." << std::endl;

            handle_fix(entry, expected);
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
