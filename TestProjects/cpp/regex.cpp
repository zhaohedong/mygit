#include <iostream>
#include <regex>
#include <string>

enum {
    Aa,
    Bb,
    Cc
};

int main()
{
    std::string number = "123d";
    const std::regex base_regex("[^\\d]");
    if (std::regex_search(number, base_regex)) {
        std::cout << "found" << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }

    std::cout << "Aa is " + std::to_string(Aa) << std::endl;

	return 0;
}