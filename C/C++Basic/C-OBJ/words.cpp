#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <stdlib.h>

using namespace std;

/******************METHOD 1***************************/
class strClass {
public:
	char word[6];

	strClass(const char *s) {
		memset(word, 0, sizeof(word));  /* init */
		strcpy(word, s);				/* copy str */
	}

	bool operator < (const strClass& _rhs) const
	{
		if (strcmp(_rhs.word, word) > 0)
			return true;

		return false;
	}
};

/* map for statistics */
map<strClass, int> myStrMap;

void word_count_I(strClass str)
{
	map<strClass, int>::iterator itr;

	itr = myStrMap.find(str);

	if (itr != myStrMap.end())
		myStrMap[str]++; //find , then add counter
	else
		myStrMap.insert(pair<strClass, int>(str, 1));
}

void dump_I(ofstream &fcout) /* display result */
{
	map<strClass, int>::iterator itr;

	for (itr = myStrMap.begin(); itr != myStrMap.end(); itr++)
	{
		fcout << itr->first.word << " : " << itr->second << endl;
	}
}

/******************METHOD 2***************************/

map<string, int> stringMap;

void word_count_II(string str)
{
	map<string, int>::iterator itr;

	itr = stringMap.find(str);

	if (itr != stringMap.end())
		stringMap[str]++; // accumlate when found
	else
		stringMap.insert(pair<string, int>(str, 1)); // add when no found
}

void dump_II(ofstream &fcout)
{
	map<string, int>::iterator itr;

	for (itr = stringMap.begin(); itr != stringMap.end(); itr++)
	{
		fcout << itr->first << " = " << itr->second << endl;
	}
}

/******************METHOD 3***************************/

#define MAX 1000000

int hash_map[MAX] = {0};

void word_count_III(const char *str)
{
	int tmp = 0;

	for (unsigned int i = 0; i < strlen(str); i++)
	{
		tmp = tmp * 10 + (str[i] - 'A');
	}

	hash_map[tmp]++;
}

string word_i(int i)
{
	string str = "";

	while (i > 0) {
		char c = 'A' + i%10;
		str = c + str;
		i /= 10;
	}

	return str;
}

void dump_III(ofstream &fcout)
{
	int cnt = 0;

	for (int i = 0; i < MAX; i++)
	{
		if (hash_map[i] != 0) {
			fcout << word_i(i).substr(1) << " # " << hash_map[i] << endl;
			cnt ++;
		}
	}

	//cout << cnt << endl;
}

int main(int argc, char **argv)
{
	string str;
	ifstream fin("seq.list");

	while (fin >> str) {

		cout << str << endl;

	/* solution-1 */
		strClass sc(str.c_str());
		word_count_I(sc);

	/* solution-2 */
		word_count_II(str);

	/* solution-3 */
		str = "B" + str;
		word_count_III(str.c_str());
	}

	fin.close();

	ofstream fout1("result1.txt");
	ofstream fout2("result2.txt");
	ofstream fout3("result3.txt");

	dump_I(fout1);
	dump_II(fout2);
	dump_III(fout3);

	fout1.close();
	fout2.close();
	fout3.close();

	return 0;
}
