#include <vector>
class BASE {
public:
    int a;
private:
    int b;
};

class Derived : public BASE {
public:
    int c;
};

std::vector<BASE> v;

void Push(BASE& in){
    v.push_back(in);
}

void Pop(BASE* out) {
    *out = v.back();
    Derived* dd = (Derived*)out;
    printf("dd->a = %d, dd->c = %d\n", dd->a, dd->c);
    v.pop_back();
}

void FunA() {
    Derived* d = new Derived();
    d->a = 666;
    d->c = 999;
    Push(*d);
}

void FunB(BASE& base) {
    Derived *dd = (Derived*)&base;
    printf("dd->a = %d, dd->c = %d\n", dd->a, dd->c);
}

int main()
{
    FunA();
    BASE* t;
    Pop(t);
    Derived* dd = (Derived*)t; 
    printf("dd->a = %d, dd->c = %d\n", dd->a, dd->c);

    Derived tmp;
    tmp.a = 1000;
    tmp.c = 2000;
    FunB(tmp);

    return 0;
}