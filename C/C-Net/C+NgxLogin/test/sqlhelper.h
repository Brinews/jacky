#ifndef _SQL_HELPER_
#define _SQL_HELPER_

#include "mysql.h"
#include <vector>
#include <string>

using namespace std;

class SqlHelper {
 public:
	 SqlHelper();

	 vector< vector<string> > SqlRead(const char *qry);
	 bool SqlWrite(const char *qry);

	 void SqlClose();

	 ~SqlHelper();

 private:
	 char dbuser[32];
	 char dbpasswd[32];
	 char dbaddr[32];
	 char dbname[32];
	 char dbtable[32];

	 MYSQL *con;
};

#endif
