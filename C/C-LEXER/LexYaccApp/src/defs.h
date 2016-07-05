#ifndef _COMPILER_H
#define _COMPILER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
enum Type { _INT, _CHAR };
struct Createfieldsdef{
    char *col;
    enum Type type;
    int length;
    struct Createfieldsdef *next_cdef;
};
struct Createdef{
    char *table;
    struct Createfieldsdef *cdef;
};
struct Insertfieldsdef{
    enum Type type;
    int intcont;
    char *strcont;
    struct Insertfieldsdef *next_idef;
};
struct Insertdef{
    char *table;
    struct Insertfieldsdef *idef;
};

struct Conditions{
      struct Conditions *left;
      struct Conditions *right;
      char *comp_op;
      int type;  // 0 ×Ö¶Î   1 ×Ö·û´®   2 Êý×Ö
      char *str_value;
      int int_value;
};
struct Selectedfields{
    char *field;
    struct Selectedfields *next_sf;
};
struct Selectedtables{
    char *table;
    struct Selectedtables *next_st;
};
struct Selectstruct{
    struct Selectedfields *sf;
    struct Selectedttables *st;
    struct Conditions *cons;
};

int yylex(void);
void yyerror(char *);
void analysis(char *);
int analysis_create(struct Createdef *_create);
int analysis_insert(struct Insertdef *_insert);
int analysis_select(struct Selectstruct *_select);

#pragma   comment   (linker,"/NODEFAULTLIB:libc.lib")

#endif
