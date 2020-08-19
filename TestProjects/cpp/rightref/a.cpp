#include <stdio.h>
using namespace std;
class A {};
void foo(const A &) {printf("normal ref");}; // 函数1
void foo(A &&) {printf("right ref");}; // 函数2
A a;
const A ac;
int main()
{
	foo(ac);// 函数1将被调用
	foo(a);// 函数1将被调用
	foo(A());// 函数2将被调用
	foo(move(a));// 函数2将被调用
	return 0;
}

