#include "String.hpp"

/* unit test */

/* 1. test constructor */
void test_constructor()
{
	String strC('C');
	String strS("string");

	char p[8] = {'H','L','M','N','P','Q','C', '\0'};
	String strA(p);

	String strD(strS);
	String strE = strA;

	std::cout << strC << strS << strA << std::endl;
	std::cout << strD << strE << std::endl;

}

/* 2. test concatenation */
void test_concatenation()
{
	String strA("From ");
	String strB("To ");

	String strC = strA + strB;
	String strD = strA + "Basic";
	String strE = strA + 'A';
	String strF = "Professional " + strB;
	String strG = 'B' + strB;

	std::cout << strC << std::endl;
	std::cout << strD << std::endl;
	std::cout << strE << std::endl;
	std::cout << strF << std::endl;
	std::cout << strG << std::endl;
}

/* 3. test relation operator */
void test_relation()
{
	String strA("big");
	String strB("little");

	if (strA > strB) { std::cout << "Bad" << std::endl;}
	if (strA < strB) { std::cout << "Good" << std::endl;}

	String strC("little");

	if (strB == strC) { std::cout << "Good" << std::endl; };
}

/* 4. test basic operations */
void test_basic_operation()
{
	String strA("Hello World.");
	String strB("World");

	if (strB.length() == 5) {std::cout << "Good" << std::endl; }
	if (strA.findstr(strB, 0) >= 0) {std::cout << "Good" << std::endl; }
	if (strA.substr(6, 5) == strB) {std::cout << "Good" << std::endl; }
	if (strA.findchar('o', 0) >= 0) {std::cout << "Good" << std::endl; }
}

/* 5. test stream */
void test_stringstream()
{
	String str1, str2;

	std::cout << "Input two string: " << std::endl;
	std::cin >> str1 >> str2;

	std::cout << "Your input two string: " << std::endl;
	std::cout << str1 << std::endl;
	std::cout << str2 << std::endl;
}

/* 6. test operator[] */
void test_accessor()
{
	String strA("changeme");

	for (int i = 0; i < strA.length(); i++)
	{
		char t = strA[i];
		strA[i] = t - ( 'a' - 'A');

		//std::cout << strA[i] << " ";
	}

	std::cout << "Accessor test: " << std::endl;
	if (strA == "CHANGEME") {std::cout << "Good" << std::endl; }
}

/* 7. test dynamic-size */
void test_dynamic()
{
	String strA("test a");
	String strB("test b");

	std::cout << "Before Swap:\n";
	std::cout << "strA: " << strA << std::endl;
	std::cout << "strB: " << strB << std::endl;

	strA.swap(strB);

	std::cout << "After Swap:\n";
	std::cout << "strA: " << strA << std::endl;
	std::cout << "strB: " << strB << std::endl;

	strA.resetCapacity(100);
	std::cout << "After Resize:\n";
	std::cout << strA << std::endl;
}

/* 8. test insert */
void test_insert()
{
	String strA("to insert something");

	strA.insertAt("ello ", 0);
	strA.insertAt('H', 0);

	strA.insertAt("string,", 6);
	strA.insertAt(".", strA.length());

	std::cout << "test insert:" << std::endl;
	std::cout << strA << std::endl;
}

/* 9. test justify */
void test_justify()
{
	String strA("AB CD EF");
	String strB;
	
	strB = strA.justify(11);

	std::cout << strB << std::endl;
}

int main()
{
	/* part I */
	test_constructor();
	test_concatenation();
	test_relation();
	test_basic_operation();
	test_stringstream();
	test_accessor();

	/* part II */
	test_dynamic();
	test_insert();

	/* part III */
	test_justify();

	/* part IV */

	return 0;
}
