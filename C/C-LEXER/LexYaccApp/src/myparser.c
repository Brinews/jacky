/****************************************************************************
*                         A C A D E M I C   C O P Y
* 
* This file was produced by an ACADEMIC COPY of Parser Generator. It is for
* use in non-commercial software only. An ACADEMIC COPY of Parser Generator
* can only be used by a student, or a member of an academic community. If
* however you want to use Parser Generator for commercial purposes then you
* will need to purchase a license. For more information see the online help or
* go to the Bumble-Bee Software homepage at:
* 
* http://www.bumblebeesoftware.com
* 
* This notice must remain present in the file. It cannot be removed.
****************************************************************************/

/****************************************************************************
* myparser.c
* C source file generated from myparser.y.
* 
* Date: 07/05/16
* Time: 16:00:31
* 
* AYACC Version: 2.07
****************************************************************************/

#include <yypars.h>

/* namespaces */
#if defined(__cplusplus) && defined(YYSTDCPPLIB)
using namespace std;
#endif
#if defined(__cplusplus) && defined(YYNAMESPACE)
using namespace yl;
#endif

#define YYFASTPARSER

#line 1 ".\\myparser.y"

#include "defs.h"

#line 43 "myparser.c"
/* repeated because of possible precompiled header */
#include <yypars.h>

/* namespaces */
#if defined(__cplusplus) && defined(YYSTDCPPLIB)
using namespace std;
#endif
#if defined(__cplusplus) && defined(YYNAMESPACE)
using namespace yl;
#endif

#define YYFASTPARSER

#include ".\myparser.h"

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYSTACK_SIZE
#define YYSTACK_SIZE 100
#endif
#ifndef YYSTACK_MAX
#define YYSTACK_MAX 0
#endif

/* (state) stack */
#if (YYSTACK_SIZE) != 0
static yystack_t YYNEAR yystack[(YYSTACK_SIZE)];
yystack_t YYFAR *YYNEAR YYDCDECL yysstackptr = yystack;
yystack_t YYFAR *YYNEAR YYDCDECL yystackptr = yystack;
#else
yystack_t YYFAR *YYNEAR YYDCDECL yysstackptr = NULL;
yystack_t YYFAR *YYNEAR YYDCDECL yystackptr = NULL;
#endif

/* attribute stack */
#if (YYSTACK_SIZE) != 0
static YYSTYPE YYNEAR yyattributestack[(YYSTACK_SIZE)];
#ifdef YYPROTOTYPE
void YYFAR *YYNEAR YYDCDECL yysattributestackptr = yyattributestack;
void YYFAR *YYNEAR YYDCDECL yyattributestackptr = yyattributestack;
#else
char YYFAR *YYNEAR YYDCDECL yysattributestackptr = (char YYFAR *) yyattributestack;
char YYFAR *YYNEAR YYDCDECL yyattributestackptr = (char YYFAR *) yyattributestack;
#endif
#else
#ifdef YYPROTOTYPE
void YYFAR *YYNEAR YYDCDECL yysattributestackptr = NULL;
void YYFAR *YYNEAR YYDCDECL yyattributestackptr = NULL;
#else
char YYFAR *YYNEAR YYDCDECL yysattributestackptr = NULL;
char YYFAR *YYNEAR YYDCDECL yyattributestackptr = NULL;
#endif
#endif

int YYNEAR YYDCDECL yysstack_size = (YYSTACK_SIZE);
int YYNEAR YYDCDECL yystack_size = (YYSTACK_SIZE);
int YYNEAR YYDCDECL yystack_max = (YYSTACK_MAX);

/* attributes */
YYSTYPE YYNEAR yyval;
YYSTYPE YYNEAR yylval;
#ifdef YYPROTOTYPE
void YYFAR *YYNEAR YYDCDECL yyvalptr = &yyval;
void YYFAR *YYNEAR YYDCDECL yylvalptr = &yylval;
#else
char YYFAR *YYNEAR YYDCDECL yyvalptr = (char *) &yyval;
char YYFAR *YYNEAR YYDCDECL yylvalptr = (char *) &yylval;
#endif

size_t YYNEAR YYDCDECL yyattribute_size = sizeof(YYSTYPE);

/* yyattribute */
#ifdef YYDEBUG
#ifdef YYPROTOTYPE
static YYSTYPE YYFAR *yyattribute1(int index)
#else
static YYSTYPE YYFAR *yyattribute1(index)
int index;
#endif
{
	YYSTYPE YYFAR *p = &((YYSTYPE YYFAR *) yyattributestackptr)[yytop + index];
	return p;
}
#define yyattribute(index) (*yyattribute1(index))
#else
#define yyattribute(index) (((YYSTYPE YYFAR *) yyattributestackptr)[yytop + (index)])
#endif

#ifdef YYDEBUG
#ifdef YYPROTOTYPE
static void yyinitdebug(YYSTYPE YYFAR **p, int count)
#else
static void yyinitdebug(p, count)
YYSTYPE YYFAR **p;
int count;
#endif
{
	int i;
	yyassert(p != NULL);
	yyassert(count >= 1);

	for (i = 0; i < count; i++) {
		p[i] = &((YYSTYPE YYFAR *) yyattributestackptr)[yytop + i - (count - 1)];
	}
}
#endif

#ifdef YYPROTOTYPE
void YYCDECL yyparseraction(int action)
#else
void YYCDECL yyparseraction(action)
int action;
#endif
{
	switch (action) {
	case 0:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 37 ".\\myparser.y"
 yyval.intval = 1; printf("<%6s>\n\n",yyattribute(1 - 1).strval); 
#line 169 "myparser.c"
			}
		}
		break;
	case 1:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 38 ".\\myparser.y"
 yyval.intval = 1; printf("<%6s>\n\n",yyattribute(2 - 2).strval); 
#line 182 "myparser.c"
			}
		}
		break;
	case 2:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 39 ".\\myparser.y"
 yyval.intval = 1; printf("<%6s>\n\n","error"); 
#line 195 "myparser.c"
			}
		}
		break;
	case 3:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 40 ".\\myparser.y"
 yyval.intval = 1; printf("\n"); 
#line 208 "myparser.c"
			}
		}
		break;
	case 4:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 41 ".\\myparser.y"
 exit(0); 
#line 221 "myparser.c"
			}
		}
		break;
	case 5:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 42 ".\\myparser.y"
 exit(0); 
#line 234 "myparser.c"
			}
		}
		break;
	case 6:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 44 ".\\myparser.y"
 yyval.strval = "empty"; 
#line 247 "myparser.c"
			}
		}
		break;
	case 7:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 45 ".\\myparser.y"
 yyval.strval = "create"; analysis_create(yyattribute(1 - 2).cs_var); 
#line 260 "myparser.c"
			}
		}
		break;
	case 8:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 46 ".\\myparser.y"
 yyval.strval = "insert"; analysis_insert(yyattribute(1 - 2).is_var); 
#line 273 "myparser.c"
			}
		}
		break;
	case 9:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 47 ".\\myparser.y"
 yyval.strval = "select"; analysis_select(yyattribute(1 - 2).ss_var); 
#line 286 "myparser.c"
			}
		}
		break;
	case 10:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[7];
			yyinitdebug(yya, 7);
#endif
			{
#line 51 ".\\myparser.y"
 
                yyval.cs_var = (struct Createdef *)malloc(sizeof(struct Createdef));
                yyval.cs_var -> table = strdup(yyattribute(3 - 6).strval);
                yyval.cs_var -> cdef = yyattribute(5 - 6).cfdef_var; 
                free(yyattribute(3 - 6).strval);
           
#line 304 "myparser.c"
			}
		}
		break;
	case 11:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 60 ".\\myparser.y"

                    struct Createfieldsdef *temp;// = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
                    temp = (struct Createfieldsdef *)yyattribute(1 - 3).cfdef_var;
                    while(temp->next_cdef){
                        temp = temp->next_cdef;
                    }
                    temp->next_cdef = yyattribute(3 - 3).cfdef_var;
                
#line 324 "myparser.c"
			}
		}
		break;
	case 12:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 70 ".\\myparser.y"

                yyval.cfdef_var = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
                memset(yyval.cfdef_var, 0, sizeof(struct Createfieldsdef));
                yyval.cfdef_var -> col = strdup(yyattribute(1 - 2).strval);
                yyval.cfdef_var -> type = _INT;
                free(yyattribute(1 - 2).strval);
          
#line 343 "myparser.c"
			}
		}
		break;
	case 13:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[6];
			yyinitdebug(yya, 6);
#endif
			{
#line 78 ".\\myparser.y"

                yyval.cfdef_var = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
                memset(yyval.cfdef_var, 0, sizeof(struct Createfieldsdef));
                yyval.cfdef_var -> col = strdup(yyattribute(1 - 5).strval);
                yyval.cfdef_var -> type = _CHAR;
                yyval.cfdef_var -> length = yyattribute(4 - 5).intval;
                free(yyattribute(1 - 5).strval);                 
          
#line 363 "myparser.c"
			}
		}
		break;
	case 14:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[7];
			yyinitdebug(yya, 7);
#endif
			{
#line 88 ".\\myparser.y"

                yyval.is_var = (struct Insertdef *)malloc(sizeof(struct Insertdef));
                yyval.is_var->table = strdup(yyattribute(3 - 6).strval);
                yyval.is_var->idef = yyattribute(5 - 6).ifdef_var;
                free(yyattribute(3 - 6).strval);
           
#line 381 "myparser.c"
			}
		}
		break;
	case 15:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 97 ".\\myparser.y"

                    struct Insertfieldsdef *temp;// = (struct Insertfieldsdef *)malloc(sizeof(struct Insertfieldsdef));
                    temp = (struct Insertfieldsdef *)yyattribute(1 - 3).ifdef_var;
                    while(temp->next_idef){
                        temp = temp->next_idef;
                    }
                    temp->next_idef = yyattribute(3 - 3).ifdef_var;
                 
#line 401 "myparser.c"
			}
		}
		break;
	case 16:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 107 ".\\myparser.y"
 
                yyval.ifdef_var = (struct Insertfieldsdef *)malloc(sizeof(struct Insertfieldsdef));
                memset(yyval.ifdef_var, 0, sizeof(struct Insertfieldsdef));
                yyval.ifdef_var->type = _CHAR;
                yyval.ifdef_var->strcont = strdup(yyattribute(1 - 1).strval);
                free(yyattribute(1 - 1).strval);
           
#line 420 "myparser.c"
			}
		}
		break;
	case 17:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 115 ".\\myparser.y"

                yyval.ifdef_var = (struct Insertfieldsdef *)malloc(sizeof(struct Insertfieldsdef));
                memset(yyval.ifdef_var, 0, sizeof(struct Insertfieldsdef));
                yyval.ifdef_var->type = _INT;
                yyval.ifdef_var->intcont = yyattribute(1 - 1).intval;
           
#line 438 "myparser.c"
			}
		}
		break;
	case 18:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[6];
			yyinitdebug(yya, 6);
#endif
			{
#line 124 ".\\myparser.y"

                struct Selectedfields *temp;
                
                printf("Tables: %s\n", yyattribute(4 - 5).st_var->table);
                printf_Selectcondition(yyattribute(5 - 5).cons_var);
                if(!strcmp(yyattribute(2 - 5).sf_var->field,"*")){
                    printf("*");
                }
                else if(yyattribute(2 - 5).sf_var->field){
                    printf("\n");
                    temp = (struct Selectedfields *) yyattribute(2 - 5).sf_var;
                    while(temp){
                        printf("-%s- ",temp->field);
                        temp = temp->next_sf;
                    }
                }
           
#line 467 "myparser.c"
			}
		}
		break;
	case 19:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 142 ".\\myparser.y"
 yyval.sf_var = yyattribute(1 - 1).sf_var; 
#line 480 "myparser.c"
			}
		}
		break;
	case 20:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 144 ".\\myparser.y"

                yyval.sf_var = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
                memset(yyval.sf_var, 0, sizeof(struct Selectedfields));
                yyval.sf_var->field = "*";
           
#line 497 "myparser.c"
			}
		}
		break;
	case 21:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 151 ".\\myparser.y"

            yyval.sf_var = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
            memset(yyval.sf_var, 0, sizeof(struct Selectedfields));
            yyval.sf_var->field = strdup(yyattribute(1 - 1).strval);
            free(yyattribute(1 - 1).strval);
      
#line 515 "myparser.c"
			}
		}
		break;
	case 22:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 158 ".\\myparser.y"

            struct Selectedfields *temp;// = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
            
            struct Selectedfields *temp_next = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
            memset(temp_next, 0, sizeof(struct Selectedfields));
            
            temp = (struct Selectedfields *)yyattribute(1 - 3).sf_var;
            
            while(temp->next_sf){
                temp = temp->next_sf;
            }
            temp_next->field = strdup(yyattribute(3 - 3).strval);
            temp->next_sf = temp_next;
            free(yyattribute(3 - 3).strval);
      
#line 542 "myparser.c"
			}
		}
		break;
	case 23:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yya, 2);
#endif
			{
#line 175 ".\\myparser.y"

            yyval.st_var = (struct Selectedtables *)malloc(sizeof(struct Selectedtables));
            memset(yyval.st_var, 0, sizeof(struct Selectedtables));
            yyval.st_var->table = strdup(yyattribute(1 - 1).strval);
            free(yyattribute(1 - 1).strval);
      
#line 560 "myparser.c"
			}
		}
		break;
	case 24:
		{
#line 182 ".\\myparser.y"
 yyval.cons_var = NULL; 
#line 568 "myparser.c"
		}
		break;
	case 25:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yya, 3);
#endif
			{
#line 184 ".\\myparser.y"

            yyval.cons_var = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var = yyattribute(2 - 2).cons_var;
          
#line 583 "myparser.c"
			}
		}
		break;
	case 26:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 189 ".\\myparser.y"

            yyval.cons_var = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->left = yyattribute(1 - 3).cons_var;
            yyval.cons_var->right = yyattribute(3 - 3).cons_var;
            yyval.cons_var->comp_op = "a";
          
#line 601 "myparser.c"
			}
		}
		break;
	case 27:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 196 ".\\myparser.y"

            yyval.cons_var = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->left = yyattribute(1 - 3).cons_var;
            yyval.cons_var->right = yyattribute(3 - 3).cons_var;
            yyval.cons_var->comp_op = "o";   
          
#line 619 "myparser.c"
			}
		}
		break;
	case 28:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 204 ".\\myparser.y"

            yyval.cons_var = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->comp_op = strdup(yyattribute(2 - 3).strval);
            yyval.cons_var->left = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->left->type = 0;
            yyval.cons_var->left->str_value = strdup(yyattribute(1 - 3).strval);
            yyval.cons_var->right = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->right->type = 2;
            yyval.cons_var->right->int_value = yyattribute(3 - 3).intval;
            free(yyattribute(1 - 3).strval);free(yyattribute(2 - 3).strval);
         
#line 642 "myparser.c"
			}
		}
		break;
	case 29:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yya, 4);
#endif
			{
#line 216 ".\\myparser.y"

            yyval.cons_var = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->comp_op = strdup(yyattribute(2 - 3).strval);
            yyval.cons_var->left = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->left->type = 0;
            yyval.cons_var->left->str_value = strdup(yyattribute(1 - 3).strval);
            yyval.cons_var->right = (struct Conditions *)malloc(sizeof(struct Conditions));
            yyval.cons_var->right->type = 1;
            yyval.cons_var->right->str_value = strdup(yyattribute(3 - 3).strval);
            free(yyattribute(1 - 3).strval);free(yyattribute(2 - 3).strval);free(yyattribute(3 - 3).strval);
         
#line 665 "myparser.c"
			}
		}
		break;
	default:
		yyassert(0);
		break;
	}
}
#ifdef YYDEBUG
YYCONST yysymbol_t YYNEARFAR YYBASED_CODE YYDCDECL yysymbol[] = {
	{ "$end", 0 },
	{ "\'(\'", 40 },
	{ "\')\'", 41 },
	{ "\'*\'", 42 },
	{ "\',\'", 44 },
	{ "\';\'", 59 },
	{ "error", 256 },
	{ "CREATE", 257 },
	{ "TABLE", 258 },
	{ "INSERT", 259 },
	{ "INTO", 260 },
	{ "SELECT", 261 },
	{ "FROM", 262 },
	{ "WHERE", 263 },
	{ "INT", 264 },
	{ "CHAR", 265 },
	{ "EXIT", 266 },
	{ "ID", 267 },
	{ "COMPARISON", 268 },
	{ "STRING", 269 },
	{ "INTEGER", 270 },
	{ "AND", 271 },
	{ "OR", 272 },
	{ NULL, 0 }
};

YYCONST char *YYCONST YYNEARFAR YYBASED_CODE YYDCDECL yyrule[] = {
	"$accept: program",
	"program: statements",
	"program: program statements",
	"program: program error \';\'",
	"program: error \';\'",
	"program: program EXIT \';\'",
	"program: EXIT \';\'",
	"statements: \';\'",
	"statements: create_stmt \';\'",
	"statements: insert_stmt \';\'",
	"statements: select_stmt \';\'",
	"create_stmt: CREATE TABLE ID \'(\' fieldsdefinition \')\'",
	"fieldsdefinition: field_type",
	"fieldsdefinition: fieldsdefinition \',\' field_type",
	"field_type: ID INT",
	"field_type: ID CHAR \'(\' INTEGER \')\'",
	"insert_stmt: INSERT INTO ID \'(\' ifieldsdefinition \')\'",
	"ifieldsdefinition: ifield_type",
	"ifieldsdefinition: ifieldsdefinition \',\' ifield_type",
	"ifield_type: ID",
	"ifield_type: INTEGER",
	"select_stmt: SELECT fields_star FROM tables where_conditions",
	"fields_star: fields",
	"fields_star: \'*\'",
	"fields: ID",
	"fields: fields \',\' ID",
	"tables: ID",
	"where_conditions:",
	"where_conditions: WHERE condition",
	"where_conditions: where_conditions AND condition",
	"where_conditions: where_conditions OR condition",
	"condition: ID COMPARISON INTEGER",
	"condition: ID COMPARISON STRING"
};
#endif

YYCONST yyreduction_t YYNEARFAR YYBASED_CODE YYDCDECL yyreduction[] = {
	{ 0, 1, -1 },
	{ 1, 1, 0 },
	{ 1, 2, 1 },
	{ 1, 3, 2 },
	{ 1, 2, 3 },
	{ 1, 3, 4 },
	{ 1, 2, 5 },
	{ 2, 1, 6 },
	{ 2, 2, 7 },
	{ 2, 2, 8 },
	{ 2, 2, 9 },
	{ 3, 6, 10 },
	{ 4, 1, -1 },
	{ 4, 3, 11 },
	{ 5, 2, 12 },
	{ 5, 5, 13 },
	{ 6, 6, 14 },
	{ 7, 1, -1 },
	{ 7, 3, 15 },
	{ 8, 1, 16 },
	{ 8, 1, 17 },
	{ 9, 5, 18 },
	{ 10, 1, 19 },
	{ 10, 1, 20 },
	{ 11, 1, 21 },
	{ 11, 3, 22 },
	{ 12, 1, 23 },
	{ 13, 0, 24 },
	{ 13, 2, 25 },
	{ 13, 3, 26 },
	{ 13, 3, 27 },
	{ 14, 3, 28 },
	{ 14, 3, 29 }
};

int YYNEAR YYDCDECL yytokenaction_size = 269;
YYCONST yytokenaction_t YYNEARFAR YYBASED_CODE YYDCDECL yytokenaction[] = {
	{ 7, YYAT_ACCEPT, 0 },
	{ 5, YYAT_SHIFT, 15 },
	{ 0, YYAT_ERROR, 0 },
	{ 51, YYAT_SHIFT, 40 },
	{ 42, YYAT_SHIFT, 50 },
	{ 38, YYAT_SHIFT, 48 },
	{ 51, YYAT_SHIFT, 41 },
	{ 42, YYAT_SHIFT, 51 },
	{ 38, YYAT_SHIFT, 49 },
	{ 59, YYAT_SHIFT, 63 },
	{ 59, YYAT_SHIFT, 64 },
	{ 45, YYAT_SHIFT, 54 },
	{ 45, YYAT_SHIFT, 55 },
	{ 37, YYAT_SHIFT, 46 },
	{ 37, YYAT_SHIFT, 47 },
	{ 62, YYAT_SHIFT, 65 },
	{ 56, YYAT_SHIFT, 62 },
	{ 55, YYAT_SHIFT, 52 },
	{ 52, YYAT_SHIFT, 59 },
	{ 49, YYAT_SHIFT, 37 },
	{ 47, YYAT_SHIFT, 56 },
	{ 35, YYAT_SHIFT, 44 },
	{ 29, YYAT_SHIFT, 36 },
	{ 28, YYAT_SHIFT, 34 },
	{ 27, YYAT_SHIFT, 33 },
	{ 26, YYAT_SHIFT, 32 },
	{ 21, YYAT_SHIFT, 31 },
	{ 20, YYAT_SHIFT, 30 },
	{ 18, YYAT_SHIFT, 29 },
	{ 17, YYAT_SHIFT, 28 },
	{ 14, YYAT_SHIFT, 27 },
	{ 13, YYAT_SHIFT, 26 },
	{ 11, YYAT_SHIFT, 25 },
	{ 10, YYAT_SHIFT, 24 },
	{ 9, YYAT_SHIFT, 23 },
	{ 6, YYAT_SHIFT, 19 },
	{ 4, YYAT_SHIFT, 14 },
	{ 3, YYAT_SHIFT, 13 },
	{ 2, YYAT_SHIFT, 12 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ 7, YYAT_SHIFT, 1 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ 5, YYAT_SHIFT, 16 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ 7, YYAT_SHIFT, 20 },
	{ 7, YYAT_SHIFT, 3 },
	{ 0, YYAT_SHIFT, 2 },
	{ 7, YYAT_SHIFT, 4 },
	{ -1, YYAT_ERROR, 0 },
	{ 7, YYAT_SHIFT, 5 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ 7, YYAT_SHIFT, 21 },
	{ -1, YYAT_ERROR, 0 },
	{ 0, YYAT_SHIFT, 6 }
};

YYCONST yystateaction_t YYNEARFAR YYBASED_CODE YYDCDECL yystateaction[] = {
	{ 2, 1, YYAT_DEFAULT, 7 },
	{ 0, 0, YYAT_REDUCE, 7 },
	{ -21, 1, YYAT_ERROR, 0 },
	{ -221, 1, YYAT_ERROR, 0 },
	{ -224, 1, YYAT_ERROR, 0 },
	{ -41, 1, YYAT_ERROR, 0 },
	{ -24, 1, YYAT_ERROR, 0 },
	{ 0, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 1 },
	{ -25, 1, YYAT_ERROR, 0 },
	{ -26, 1, YYAT_ERROR, 0 },
	{ -27, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 4 },
	{ -236, 1, YYAT_ERROR, 0 },
	{ -237, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 23 },
	{ 0, 0, YYAT_REDUCE, 24 },
	{ -233, 1, YYAT_ERROR, 0 },
	{ -16, 1, YYAT_REDUCE, 22 },
	{ 0, 0, YYAT_REDUCE, 6 },
	{ -32, 1, YYAT_ERROR, 0 },
	{ -33, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 2 },
	{ 0, 0, YYAT_REDUCE, 8 },
	{ 0, 0, YYAT_REDUCE, 9 },
	{ 0, 0, YYAT_REDUCE, 10 },
	{ -15, 1, YYAT_ERROR, 0 },
	{ -16, 1, YYAT_ERROR, 0 },
	{ -244, 1, YYAT_ERROR, 0 },
	{ -245, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 3 },
	{ 0, 0, YYAT_REDUCE, 5 },
	{ 0, 0, YYAT_DEFAULT, 49 },
	{ 0, 0, YYAT_DEFAULT, 51 },
	{ 0, 0, YYAT_REDUCE, 26 },
	{ -242, 1, YYAT_REDUCE, 27 },
	{ 0, 0, YYAT_REDUCE, 25 },
	{ -251, 1, YYAT_ERROR, 0 },
	{ -36, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 12 },
	{ 0, 0, YYAT_REDUCE, 19 },
	{ 0, 0, YYAT_REDUCE, 20 },
	{ -37, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 17 },
	{ 0, 0, YYAT_DEFAULT, 55 },
	{ -260, 1, YYAT_REDUCE, 21 },
	{ 0, 0, YYAT_REDUCE, 14 },
	{ -20, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 11 },
	{ -248, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 16 },
	{ -264, 1, YYAT_ERROR, 0 },
	{ -250, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 28 },
	{ 0, 0, YYAT_DEFAULT, 55 },
	{ -250, 1, YYAT_ERROR, 0 },
	{ -254, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 13 },
	{ 0, 0, YYAT_REDUCE, 18 },
	{ -260, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 29 },
	{ 0, 0, YYAT_REDUCE, 30 },
	{ -26, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 32 },
	{ 0, 0, YYAT_REDUCE, 31 },
	{ 0, 0, YYAT_REDUCE, 15 }
};

int YYNEAR YYDCDECL yynontermgoto_size = 19;

YYCONST yynontermgoto_t YYNEARFAR YYBASED_CODE YYDCDECL yynontermgoto[] = {
	{ 7, 22 },
	{ 7, 9 },
	{ 33, 42 },
	{ 33, 43 },
	{ 7, 10 },
	{ 32, 38 },
	{ 32, 39 },
	{ 7, 11 },
	{ 5, 17 },
	{ 5, 18 },
	{ 0, 7 },
	{ 0, 8 },
	{ 55, 61 },
	{ 54, 60 },
	{ 51, 58 },
	{ 49, 57 },
	{ 44, 53 },
	{ 35, 45 },
	{ 28, 35 }
};

YYCONST yystategoto_t YYNEARFAR YYBASED_CODE YYDCDECL yystategoto[] = {
	{ 9, 7 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ -2, -1 },
	{ 0, -1 },
	{ -2, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 6, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 1, -1 },
	{ -5, -1 },
	{ 0, -1 },
	{ 4, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 2, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 10, -1 },
	{ 0, -1 },
	{ 6, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ -1, -1 },
	{ -2, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 }
};

YYCONST yydestructor_t YYNEARFAR *YYNEAR YYDCDECL yydestructorptr = NULL;

YYCONST yytokendest_t YYNEARFAR *YYNEAR YYDCDECL yytokendestptr = NULL;
int YYNEAR YYDCDECL yytokendest_size = 0;

YYCONST yytokendestbase_t YYNEARFAR *YYNEAR YYDCDECL yytokendestbaseptr = NULL;
int YYNEAR YYDCDECL yytokendestbase_size = 0;
#line 231 ".\\myparser.y"

void yyerror(char *s){
    printf("%s\n",s);
}
int main(void){
    printf("\n");
    yyparse();
    return 0;
}

