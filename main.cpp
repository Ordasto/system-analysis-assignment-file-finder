#include <iostream>
#include <vector>
#include <filesystem>

// convenient WINDOWS or UNIX defines
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define _WINDOWS 1
#elif defined(__linux__) || defined(__unix__)
    #define _UNIX 1
#endif

int main(int argc, char* argv[]) {
    std::string directory;
    std::string file_name;

    if (argc == 3) {
        // first argument is name of program
        // if we get 2 more, they should be treated as the directory and file_name
        directory = std::string(argv[1]);
        file_name = std::string(argv[2]);
    } else {
        // If we dont get 3 arguments (directory and filename) get input from terminal entries
        std::cout << "Enter a directory to search and then a file to search for." << std::endl;

        // print help and get directory
        std::cout << "Enter directory" << std::endl;
        std::cout << "Use / to depict the root directory" << std::endl;
        std::cout << "Use ./ to depict the current directory" << std::endl;
        std::cout << "Use ~/ to depict the home directory" << std::endl;
        std::cout << std::endl << "> ";
        std::cin >> directory;
        std::cout << std::endl;

        // get filename
        std::cout << "Enter filename"<<std::endl;
        std::cout << "Use * to match all files" << std::endl;
        std::cout << "> ";
        std::cin >> file_name;
        std::cout << std::endl;
    }

    // if directory starts with ~/ get home directory depending on os and append to front of directory variable
    if (directory.size() >= 2 && directory[0] == '~' && directory[1] == '/') {
#if defined(_WINDOWS)
        // On windows, the home path needs drive and path environment variables
        std::string home_path = std::getenv("HOMEPATH");        std::string home_drive = std::getenv("HOMEDRIVE");
        directory = home_drive + home_path + directory.substr(1);
#elif defined(_UNIX)
        // On unix systems, the home directory is just the HOME environment variable
        directory = std::getenv("HOME") + directory.substr(1);
#endif
    }

    // Create a vector of directories we treat as a stack
    std::vector<std::string> directories;
    
    // add starting directory
    directories.push_back(directory);

    // while we have directories to search
    while(!directories.empty()) {
        // get the last directory in vec
        auto dir = directories[directories.size()-1];
        // remove last directory
        directories.pop_back();

        // try to iterate on all entries in a directory
        try{
            // for each entry
            for(auto& entry : std::filesystem::directory_iterator(dir, std::filesystem::directory_options::skip_permission_denied)){
                // if entry is a directory and not a symlink/shortcut, add it to directory
                if(entry.is_directory() && !entry.is_symlink()){
                    directories.push_back(entry.path());
                }
                // if entry filename is equal to file name var or file_name is *
                // print entry
                if(entry.path().filename() == file_name || file_name == "*"){
                    std::cout << entry << std::endl;
                }
            }
        }catch(const std::exception& exc){
            // if we run into an exception, print access denied.
            std::cout << "Access to " << dir << " denied." << std::endl;
        }
    }

    // exit succesfully
    return 0;
}

