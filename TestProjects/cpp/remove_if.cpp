#include <iostream>
#include <algorithm>
#include <string>
#include <regex>
int main()
{
    std::string arg("1*23#489");
    //arg.erase(remove_if(arg.begin(), arg.end(), isspace), arg.end());

    const std::regex base_regex("[^\\d*#]");
    int ret = std::regex_search(arg, base_regex);
    std::cout << "match result :"+std::to_string(ret)  << std::endl;
    return 0;
}