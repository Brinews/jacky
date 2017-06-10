#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include "parser.h"
#include "printer.h"

using namespace std;

/*
void test_simple()
{
    ifstream ifs;
    ifs.open("test.json");
    assert(ifs.is_open());
 
    Node root;
	ifs >> root;
 
    std::string name = root.find_value("name").getString();
    int age = root.find_value("age").getInt();
 
    std::cout<<name<<std::endl;
    std::cout<<age<<std::endl;
}

Node test_array()
{
    ifstream ifs;
    ifs.open("test2.json");
    assert(ifs.is_open());

    JSONParser reader;
    Node root;
    if (!reader.parse(ifs, root, false))
    {
        cout << "Parse Error\n";
        return Node::null;
    }
 
    std::string name;
    int age;
    bool mari;
    double salary;

    int size = root.size();
    for (int i=0; i<size; ++i)
    {
        name = root.get_value(i).find_value("name").getString();
        Node fav = root.get_value(i).find_value("favourite");
        if (fav != Node::null) {
            for (int j = 0; j < fav.size(); ++j) {
                cout << fav.get_value(j).getString() << endl;
            }
        }
        age = root.get_value(i).find_value("age").getInt();

        mari = root.get_value(i).find_value("married").getBool();
        salary = root.get_value(i).find_value("salary").getDouble();
 
        std::cout<<name<<" "<<age<<" "<<mari<<" "<<salary<<std::endl;
    }

	return root;
}

void test_write(const Node &v)
{
	PrettyPrinter writer;
	string out = writer.print(v);
	cout << out << endl;
}
*/

int is_array(string query)
{
	int start = query.find('[');
	int end = query.find(']');

	if (start >= 0 && end == query.length()-1 && start < end) {
		return atoi(query.substr(start+1, end-start-1).c_str());
	}

	return -1;
}

Node &get_sub_value(Node &find, string query)
{
	cout << "Q:" << query << endl;
	int index = -1;
	if ((index = is_array(query)) >= 0) {
		if (query.at(0) != '[') {
			find = find.find_value(query.substr(0, query.find('[')-1));
		}
		find = find.get_value(index);
	} else {
		find = find.find_value(query);
	}

	return find;
}

std::string query_json(Node &v, string query)
{
	int pos, nextPos;
	Node &find = v;

	do {
		pos = query.find('/');
		nextPos = query.substr(pos+1).find('/');
		query = query.substr(pos+1);

		if (nextPos < 0) break;

		find = get_sub_value(find, query.substr(0, nextPos-pos));
	} while (nextPos > 0);

	find = get_sub_value(find, query);

	return find.toString();
}

int main(int argc, char **argv)
{
	int pretty = 0;
	char *filename;
	char outfile[128];

	if (argc != 2 && argc != 3) {
		std::cerr << "Usage: json [-pretty] <in-file-name>" << std::endl;
		return -1;
	}

	if (argc == 3) {
		if (strcmp(argv[1], "-pretty") == 0) {
			pretty = 1;
		}
		filename = argv[2];
	} else {
		filename = argv[1];
	}

	ifstream ifs(filename);

	if (!ifs.is_open()) {
		std::cerr << filename << " : open failed.\n";
		return -1;
	}


	Node root;
	JSONParser reader;

	if (reader.parse(ifs, root, false)) {
		cout << "valid\n";

		if (pretty == 1) {
			char *p = filename;
			while (*p != '.' && *p != '\0') p++;
			*p = '\0';

			strcpy(outfile, filename);
			strcat(outfile, ".pretty.json");

			PrettyPrinter writer;
			ofstream fout(outfile);

			fout << writer.print(root);

			fout.close();
		}
	} else {
		cout << reader.getWrongMessages();
	}

	ifs.close();

	if (strncmp(filename, "test2", 5) == 0) {
		cout << "Query /[0]/name:\n";
		cout << query_json(root, "/[0]/name") << endl;
	}

	return 0;
}
