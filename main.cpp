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
        directory = std::string(argv[1]);
        file_name = std::string(argv[2]);
    } else {
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
    }

    if (directory.size() >= 2 && directory[0] == '~' && directory[1] == '/') {
#if defined(_WINDOWS)
        std::string home_path = std::getenv("HOMEPATH");        std::string home_drive = std::getenv("HOMEDRIVE");
        directory = home_drive + home_path + directory.substr(1);
#elif defined(_UNIX)
        directory = std::getenv("HOME") + directory.substr(1);
#endif
    }

    std::vector<std::string> directories;
    
    directories.push_back(directory);

    while(!directories.empty()) {
        auto dir = directories[directories.size()-1];
        
        directories.pop_back();
        
        try{
            for(auto& entry : std::filesystem::directory_iterator(dir, std::filesystem::directory_options::skip_permission_denied)){
                if(entry.is_directory() && !entry.is_symlink()){
                directories.push_back(entry.path());
                }
                if(entry.path().filename() == file_name || file_name == "*"){
                    std::cout << entry << std::endl;
                }
            }
        }catch(const std::exception& exc){
            // std::cerr << exc.what();
            std::cout << "Access to " << dir << " denied." << std::endl;
        }
    }

    return 0;
}


// void eghhhhhhhh(){
    
//     try {
//         for (auto& entry : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
//             if (entry.path().filename() == file_name) {
//                 std::cout << entry << std::endl;
//             }
//         }
//     }
//     catch (const std::exception& exc)
//     {
//         std::cerr << exc.what();
//     }
// }
