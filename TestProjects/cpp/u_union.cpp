#include<iostream>                                 //包含头文件
#include<stdlib.h>
using namespace std;
#include <exception>

// struct Point
// {
//     Point() {}
//     Point(int x, int y): x_(x), y_(y) {} 
//     int x_, y_;
// };

// struct stu1 {
//     int a1;
//     std::string str1_;
// };

// struct stu2 {
//     int b1;
//     std::string str2_;
// };

// struct stu3 {
//     int c1;
//     std::string str3_;
//     int c2;
// };

// union U
// {
//     stu1 stu1_;
//     stu2 stu2_;
//     stu3 stu3_;
//     // int z;
//     // double w;
//     // std::string str_;
//     // Point p; // Invalid in C++03; valid in C++11.
//     U() {} // Due to the Point member, a constructor definition is now needed.
//     // U(const Point& pt) : p(pt) {} // Construct Point object using initializer list.
//     // U& operator=(const Point& pt) { new(&p) Point(pt); return *this; } // Assign Point object using placement 'new'.
//     ~U() {}
// };

// struct dataAll {
//     std::string strA1_;
//     int cc_;
//     std::string strA2_;
//     //U u_;
//     union U
//     {
//         stu1 stu1_;
//         stu2 stu2_;
//         stu3 stu3_;
//         // int z;
//         // double w;
//         // std::string str_;
//         // Point p; // Invalid in C++03; valid in C++11.
//         U() {} // Due to the Point member, a constructor definition is now needed.
//         // U(const Point& pt) : p(pt) {} // Construct Point object using initializer list.
//         // U& operator=(const Point& pt) { new(&p) Point(pt); return *this; } // Assign Point object using placement 'new'.
//         ~U() {}
//     };
// };

// #include <string>
// using namespace std;

// union U {
//     std::string s;
//     int n;
// public:
//     U() { new(&s) std::string; }
//     ~U() { s.~string(); }
// };

// int main() {
//     U u;
//     return 0;
// }

#include <cstring>
using namespace std;

class StudentA1{
public:
    std::string str1_;
    bool gender;
    int age;
};

class StudentA2{
public:
    std::string str2_;
    bool gender;
    int age;
};

class StudentA3{
public:
    std::string str3_;
    bool gender;
    int age;
    std::string str4_;
};

class Singer {
public:
    // enum Type { STUDENT, NATIVE, FOREIGENR };
    // Singer(bool g, int a) : s(g, a) { t = STUDENT; }
    // Singer(int i) : id(i) { t = NATIVE; }
    // Singer(const char* n, int s) {
    //     int size = (s > 9) ? 9 : s;
    //     memcpy(name , n, size);
    //     name[s] = '\0';
    //     t = FOREIGENR;
    // }
    Singer() {}
    ~Singer(){}
    // Type t;
    int id;
    char name[10];
    union {
        StudentA1 s1;
        StudentA2 s2;
        StudentA3 s3;
    };
};
int main() {
    // Singer(true, 13);
    // Singer(310217);
    // Singer("J Michael", 9);
    Singer singer;
    Singer singer2 = singer;
    singer.s1.str1_ = "str1";
    cout << singer.s1.str1_ << endl;

    return 0;
}

// int main()
// {
//     dataAll data;
//     return 0;
// }
