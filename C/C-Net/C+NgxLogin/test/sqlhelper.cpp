#include "sqlhelper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SqlHelper::SqlHelper()
{
	strcpy(dbuser, "root");
	strcpy(dbpasswd, "gis");
	strcpy(dbaddr, "127.0.0.1");
	strcpy(dbname, "test");

	con = NULL;
	con = mysql_init((MYSQL *) 0);

	if (con != NULL &&
		mysql_real_connect(con, dbaddr, dbuser, dbpasswd, dbname, 
							3306, NULL, 0)) {
		if (mysql_select_db(con, dbname)) {
			fprintf(stderr, "change database failed.\n");
		}
	} else {
		fprintf(stderr, "connection to mysql failed.\n");
	}
}

vector< vector<string> > SqlHelper::SqlRead(const char *qry)
{
	vector <vector<string> > ret;

	if (mysql_real_query(con, qry, strlen(qry))) {
		fprintf(stderr, "select from database failed.");
		return ret;
	}

	MYSQL_RES *res;
	MYSQL_ROW row;

	res = mysql_store_result(con);
	while (row = mysql_fetch_row(res)) {
		int t = 0;
		vector<string> vrow;
		vrow.clear();

		for (t = 1; t < mysql_num_fields(res); t++) {
			vrow.push_back(row[t]);
		}

		ret.push_back(vrow);
	}

	mysql_free_result(res);

	return ret;
}

bool SqlHelper::SqlWrite(const char *qry)
{
	if (mysql_real_query(con, qry, strlen(qry))) {
		fprintf(stderr, "update database failed.");
		return false;
	}
	return true;
}

void SqlHelper::SqlClose()
{
	if (con != NULL) mysql_close(con);
}

SqlHelper::~SqlHelper()
{
}
