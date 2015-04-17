/*
if(子类没有自定义构造函数）｛
        if(基类没有自定义构造函数)｛
              用子类定义对象时，先自动调用基类的默认构造函数，再调子类的默认构造函数。
         } else if(基类有自定义无参构造函数)｛
              用子类定义对象时，先自动调用基类的自定义无参构造函数，再调子类的默认构造函数。
         ｝else {
              用子类定义对象时，编译提示没有默认构造函数可用
         ｝
｝else if(子类自定义构造函数但没有调用基类的某个构造函数)｛
        if(基类没有自定义构造函数)｛
              用子类定义对象时，先自动调用基类的默认构造函数，再调子类的自定义构造函数。
         ｝else if(基类有自定义无参构造函数)｛
              用子类定义对象时，先自动调用基类的自定义无参构造函数，再调子类的自定义构造函数。
         ｝else {
              用子类定义对象时，编译提示没有默认构造函数可用
         ｝
｝
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class Base {
	public:
		Base(int a) {
			cout << a << endl;
		}
};

class Derived : Base {
	public:
		Derived(int a, int d) {
			cout << a + d << endl;
		}
};


int main()
{
	Derived d;

	vector<int> ivec;
	copy(istream_iterator<int>(cin),istream_iterator<int>(),back_inserter(ivec));
	sort(ivec.begin(),ivec.end());
	unique(ivec.begin(),ivec.end());
	copy(ivec.begin(),ivec.end(),ostream_iterator<int>(cout," "));

	system("pause");

	return 0;
} 
