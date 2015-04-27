// weak_ptr constructor example
#include <iostream>
#include <memory>
#include <map>
#include <string>
using namespace std;

class A{
protected:
	int a;
public:
	A() { a = 4;}
};

class B : public A {
	int &b;
public:
	B() : b(a){}
	void test()  {
		std::cout<<"a = " << a << " and b = " << b <<endl ;
		a = 5;
		std::cout<<"a = " << a << " and b = " << b <<endl ;
		b = 6;
		std::cout<<"a = " << a << " and b = " << b <<endl ;
	}
};

int main () {
	B tst;

	tst.test();
	system("pause");

	return 0;
}