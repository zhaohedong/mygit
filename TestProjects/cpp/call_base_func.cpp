#include <iostream>
#include <string>

using namespace std;

class A {
public:
    void func1(string prefix) {
        cout << prefix << "A::func1" << endl;
    }
    virtual void func2(string prefix) {
        cout << prefix << "A::func2" << endl;
        func1(prefix + "  ");
    }
    virtual void func3(string prefix) {
        cout << prefix << "A::func3" << endl;
	func4(prefix);
    }

     virtual void func4(string prefix) {
        cout << prefix << "A::func4" << endl;
    }
};

class B: public A {
public:
    void func1(string prefix) {
        cout << prefix << "B::func1" << endl;
    }

    void func2(string prefix) {
        cout << prefix << "B::func2" << endl;
        A::func3(prefix + "  ");
    }

    void func3(string prefix) {
        cout << prefix << "B::func3" << endl;
    }

    void func4(string prefix) {
        cout << prefix << "B::func4" << endl;
    }
};

int main() {
    B b;

    b.func2("");
}