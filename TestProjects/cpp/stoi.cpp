#include <iostream>
#include <string>
#include <exception> 
#include <cerrno>

#define HFP_AG_STOI_TRY_CATCH(val, arg)                                     \
    do {                                                                    \
        try {                                                               \
            val = std::stoi(arg);                                           \
        } catch (std::exception& e){                                        \
            printf("%s():Catch exception %s\n", __FUNCTION__, e.what());      \
        }                                                                   \
    } while (0)                                                             \

int main()
{
    int val = 99;
    std::string p = "";
    HFP_AG_STOI_TRY_CATCH(val, p);
    printf("val = %d\n", val);

    // try {
    //     std::cout << std::stoi("1111111111111111111111111") << std::endl;
    // } catch (std::exception& e) {
    //     std::cout << "catch exception is " << e.what() << std::endl;
    // }
    return 0;
}