    #include <iostream>
    #include <filesystem>
    namespace fs = std::filesystem;

    int main()
    {
        std::string filename = "C:\\MyDirectory\\MyFile.bat";
        // std::cout << fs::path(filename).stem() << std::endl;
        // std::cout << fs::path("/foo/bar.txt").filename() << std::endl;
        // std::cout << fs::path("/foo/bar.txt").stem() << std::endl;

        // printf("filename is %s", std::filesystem::path("/foo/bar.txt").filename().c_str());
        printf("filename is [%s]\n", __FILE__);
        printf("filename is %s\n", std::filesystem::path(__FILE__).filename().c_str());

        std::string number = "abc;";
        if(number.at(number.length() - 1) == ';') {
            std::cout << "end is ;" << std::endl;
        } else {
            std::cout << "end is not ;" << std::endl;
        }
        number = number.substr(0, number.length() - 1);
        std::cout << number << std::endl;
#if 0
    std::cout << fs::path(filename).filename() << '\n'
        << fs::path(filename).stem() << '\n'
        << fs::path("/foo/bar.txt").filename() << '\n'
        << fs::path("/foo/bar.txt").stem() << '\n'
        << fs::path("/foo/.bar").filename() << '\n'
        << fs::path("/foo/bar/").filename() << '\n'
        << fs::path("/foo/.").filename() << '\n'
        << fs::path("/foo/..").filename() << '\n'
        << fs::path(".").filename() << '\n'
        << fs::path("..").filename() << '\n'
        << fs::path("/").filename() << '\n';
#endif 
    }