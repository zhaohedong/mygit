#include <iostream>
#include <regex>
#include <string>

int main()
{
	std::string k;
	std::cout << k << '\n';

	std::string s = "<1,2>";
	std::regex regex("\\,");
	std::vector<std::string> out(
					std::sregex_token_iterator(s.begin(), s.end(), regex, -1),
					std::sregex_token_iterator()
					);

	for (auto &s: out) {
        if (s.find("1") != std::string::npos)
		    std::cout << s << '\n';
        if (s.find("2") != std::string::npos)
		    std::cout << s << '\n';
	}

	return 0;
}