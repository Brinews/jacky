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
* myparser.h
* C header file generated from myparser.y.
* 
* Date: 07/05/16
* Time: 16:00:31
* 
* AYACC Version: 2.07
****************************************************************************/

#ifndef _MYPARSER_H
#define _MYPARSER_H

#include <yypars.h>

#ifndef YYSTYPE
union tagYYSTYPE {
#line 5 ".\\myparser.y"

    int intval;
    double floatval;
    char *strval;
    struct Createfieldsdef *cfdef_var;
    struct Createdef *cs_var;
    struct Insertfieldsdef *ifdef_var;
    struct Insertdef *is_var;
    struct Selectedfields *sf_var;
    struct Selectedtables *st_var;
    struct Conditions *cons_var;
    struct Selectstruct *ss_var;

#line 48 "myparser.h"
};

#define YYSTYPE union tagYYSTYPE
#endif

extern YYSTYPE YYNEAR yylval;

#define CREATE 257
#define TABLE 258
#define INSERT 259
#define INTO 260
#define SELECT 261
#define FROM 262
#define WHERE 263
#define INT 264
#define CHAR 265
#define EXIT 266
#define ID 267
#define COMPARISON 268
#define STRING 269
#define INTEGER 270
#define AND 271
#define OR 272
#endif
