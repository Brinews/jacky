#include "sqlhelper.h"

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv)
{
	SqlHelper mySql;
	char qry[64];

	sprintf(qry, "update userinfo set ui_passwd='111' where ui_uname='test'");

	mySql.SqlWrite(qry);

	sprintf(qry, "select * from userinfo");
	vector<vector<string> > ret = mySql.SqlRead(qry);

	for (int i = 0; i < ret.size(); i++) {
		for (int j = 0; j < ret[i].size(); j++) {
			cout << ret[i][j] << ", ";
		}
		cout << endl;
	}

	return 0;
}
