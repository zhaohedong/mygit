#include <iostream>
#include <string>
#include <regex>
int main()
{
#if 0
    std::string arg = "(0,1,1x,2,2x,3,4)";
    std::string tmpStr = arg.substr(1, arg.length() - 2);
    std::regex regex("\\,");
    std::vector<std::string> out(std::sregex_token_iterator(tmpStr.begin(), tmpStr.end(), regex, -1),
                                  std::sregex_token_iterator());
    for (std::string &s: out) {
        std::cout << s << std::endl;
    }

    std::string str("AT+CIND:(123)r");
    std::string str2("AT+CIND");
    std::string str3 = std::string(str, str2.length() + 1, str.length() - str2.length() - 2);
    //std::string str3 = std::string(str, 2, -1);
    std::cout << str3 << std::endl;
#endif

    // static const std::string CIND_SETTINGS1 = "(\"service\",(0-1)),(\"call\",(0,1)),(\"callsetup\",(0-3)),"\
    //                                          "(\"callheld\",(0-2)),(\"signal\",(0-5)),(\"roam\",(0,1)),"\
    //                                          "(\"battchg\",(0-5)))";
    // static const std::string CIND_SETTINGS2 = "(\"1service\",(0-1)),(\"call\",(0,1)),(\"callsetup\",(0-3)),(\"callheld\",(0-2)),(\"signal\",(0-5)),(\"roam\",(0,1)),(\"battchg\",(0-5)))";

    // if(CIND_SETTINGS1 == CIND_SETTINGS2) {
    //     std::cout << "matched" << std::endl;
    // } else {
    //     std::cout << "not matched" << std::endl;
    // }
    std::string str;
    std::string str2 = "abcdefghijklmn";
    const char* p = str.c_str();
    auto p2 = str2.c_str();
    strcmp(p2, p);
    printf("p is %s, p is %p", p, p);
    
    return 0;
}
