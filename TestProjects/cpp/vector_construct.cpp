
#include <iostream>
#include <vector>
using namespace std;
/*
总结：使用数组初始化vector数组：必须指出数组第n个元素以及数组n+x元素的下一位置的地址
如果超过原数组的下一个元素的范围，就会把超出部分初始化为垃圾数字
*/
int main()
{
	const size_t arr_size = 6;
	int arr[arr_size] = {0, 1, 2, 3, 4, 5};
	vector<int> vec0(arr, arr+arr_size);   
	for (vector<int>::iterator i = vec0.begin(); i!=vec0.end(); ++i)  //6
		cout << *i << endl;   //0, 1, 2, 3, 4, 5
    cout << &vec0[0] << endl;
    cout << &arr[0] << endl;
	vector<int> vec1(arr, arr + arr_size+1);
	for (vector<int>::iterator i = vec1.begin(); i != vec1.end(); ++i)
		cout << *i << ends;   //0， 1, 2, 3, 4, 5，垃圾
	cout << endl;
	vector<int> vec2(arr+1, arr + 3);
	for (vector<int>::iterator i = vec2.begin(); i != vec2.end(); ++i)
		cout << *i << ends;  // 1, 2
	system("pause");
}
