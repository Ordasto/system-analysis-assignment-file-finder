#include <iostream>
#include <vector>
#include <filesystem>

// convenient WINDOWS or UNIX defines
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define _WINDOWS 1
#elif defined(__linux__) || defined(__unix__)
    #define _UNIX 1
#endif

int main() {
    std::string directory;
    std::string file_name;

    std::cout << "Enter a directory to search and then a file to search for." << std::endl;

    std::cout << "Enter directory" << std::endl;
    std::cout << "Use / to depict the root directory" << std::endl;
    std::cout << "Use ./ to depict the current directory" << std::endl;
    std::cout << "Use ~/ to depict the home directory" << std::endl;
    std::cout << std::endl << "> ";
    std::cin >> directory;
    std::cout << std::endl;

    std::cout << "Enter filename";
    std::cout << std::endl << "> ";
    std::cin >> file_name;
    std::cout << std::endl;


    if (directory.size() >= 2 && directory[0] == '~' && directory[1] == '/') {
#if defined(_WINDOWS)
        std::string home_path = std::getenv("HOMEPATH");
        std::string home_drive = std::getenv("HOMEDRIVE");
        directory = home_drive + home_path + directory.substr(1);
#elif defined(_UNIX)
        directory = std::getenv("HOME") + directory.substr(1);
#endif
    }

    for (auto& entry : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
        if (entry.path().filename() == file_name) {
            std::cout << entry << std::endl;
        }
    }

    return 0;
}
