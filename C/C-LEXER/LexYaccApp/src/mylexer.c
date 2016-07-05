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
* mylexer.c
* C source file generated from mylexer.l.
* 
* Date: 07/05/16
* Time: 16:00:31
* 
* ALex Version: 2.07
****************************************************************************/

#include <yylex.h>

/* namespaces */
#if defined(__cplusplus) && defined(YYSTDCPPLIB)
using namespace std;
#endif
#if defined(__cplusplus) && defined(YYNAMESPACE)
using namespace yl;
#endif

#define YYFASTLEXER

#line 1 ".\\mylexer.l"

#include "defs.h"
#include "myparser.h"

#line 44 "mylexer.c"
/* repeated because of possible precompiled header */
#include <yylex.h>

/* namespaces */
#if defined(__cplusplus) && defined(YYSTDCPPLIB)
using namespace std;
#endif
#if defined(__cplusplus) && defined(YYNAMESPACE)
using namespace yl;
#endif

#define YYFASTLEXER

#include ".\mylexer.h"

#ifndef YYTEXT_SIZE
#define YYTEXT_SIZE 100
#endif
#ifndef YYUNPUT_SIZE
#define YYUNPUT_SIZE YYTEXT_SIZE
#endif
#ifndef YYTEXT_MAX
#define YYTEXT_MAX 0
#endif
#ifndef YYUNPUT_MAX
#define YYUNPUT_MAX YYTEXT_MAX
#endif

/* yytext */
static char YYNEAR yysatext[(YYTEXT_SIZE) + 1];		/* extra char for \0 */
char YYFAR *YYNEAR YYDCDECL yystext = yysatext;
char YYFAR *YYNEAR YYDCDECL yytext = yysatext;
int YYNEAR YYDCDECL yystext_size = (YYTEXT_SIZE);
int YYNEAR YYDCDECL yytext_size = (YYTEXT_SIZE);
int YYNEAR YYDCDECL yytext_max = (YYTEXT_MAX);

/* yystatebuf */
#if (YYTEXT_SIZE) != 0
static int YYNEAR yysastatebuf[(YYTEXT_SIZE)];
int YYFAR *YYNEAR YYDCDECL yysstatebuf = yysastatebuf;
int YYFAR *YYNEAR YYDCDECL yystatebuf = yysastatebuf;
#else
int YYFAR *YYNEAR YYDCDECL yysstatebuf = NULL;
int YYFAR *YYNEAR YYDCDECL yystatebuf = NULL;
#endif

/* yyunputbuf */
#if (YYUNPUT_SIZE) != 0
static int YYNEAR yysaunputbuf[(YYUNPUT_SIZE)];
int YYFAR *YYNEAR YYDCDECL yysunputbufptr = yysaunputbuf;
int YYFAR *YYNEAR YYDCDECL yyunputbufptr = yysaunputbuf;
#else
int YYFAR *YYNEAR YYDCDECL yysunputbufptr = NULL;
int YYFAR *YYNEAR YYDCDECL yyunputbufptr = NULL;
#endif
int YYNEAR YYDCDECL yysunput_size = (YYUNPUT_SIZE);
int YYNEAR YYDCDECL yyunput_size = (YYUNPUT_SIZE);
int YYNEAR YYDCDECL yyunput_max = (YYUNPUT_MAX);

/* backwards compatability with lex */
#ifdef input
#ifdef YYPROTOTYPE
int YYCDECL yyinput(void)
#else
int YYCDECL yyinput()
#endif
{
	return input();
}
#else
#define input yyinput
#endif

#ifdef output
#ifdef YYPROTOTYPE
void YYCDECL yyoutput(int ch)
#else
void YYCDECL yyoutput(ch)
int ch;
#endif
{
	output(ch);
}
#else
#define output yyoutput
#endif

#ifdef unput
#ifdef YYPROTOTYPE
void YYCDECL yyunput(int ch)
#else
void YYCDECL yyunput(ch)
int ch;
#endif
{
	unput(ch);
}
#else
#define unput yyunput
#endif

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn -rch		/* <warning: unreachable code> off */
#endif
#endif

#ifdef YYPROTOTYPE
int YYCDECL yylexeraction(int action)
#else
int YYCDECL yylexeraction(action)
int action;
#endif
{
	yyreturnflg = YYTRUE;
	switch (action) {
	case 1:
		{
#line 8 ".\\mylexer.l"
 return CREATE; 
#line 165 "mylexer.c"
		}
		break;
	case 2:
		{
#line 9 ".\\mylexer.l"
 return TABLE; 
#line 172 "mylexer.c"
		}
		break;
	case 3:
		{
#line 10 ".\\mylexer.l"
 return INSERT; 
#line 179 "mylexer.c"
		}
		break;
	case 4:
		{
#line 11 ".\\mylexer.l"
 return INTO; 
#line 186 "mylexer.c"
		}
		break;
	case 5:
		{
#line 12 ".\\mylexer.l"
 return SELECT; 
#line 193 "mylexer.c"
		}
		break;
	case 6:
		{
#line 13 ".\\mylexer.l"
 return FROM; 
#line 200 "mylexer.c"
		}
		break;
	case 7:
		{
#line 14 ".\\mylexer.l"
 return WHERE; 
#line 207 "mylexer.c"
		}
		break;
	case 8:
		{
#line 16 ".\\mylexer.l"
 return EXIT; 
#line 214 "mylexer.c"
		}
		break;
	case 9:
		{
#line 18 ".\\mylexer.l"
 return INT; 
#line 221 "mylexer.c"
		}
		break;
	case 10:
		{
#line 19 ".\\mylexer.l"
 return CHAR; 
#line 228 "mylexer.c"
		}
		break;
	case 11:
		{
#line 22 ".\\mylexer.l"
 yylval.strval = strdup("=");return COMPARISON; 
#line 235 "mylexer.c"
		}
		break;
	case 12:
		{
#line 23 ".\\mylexer.l"
 yylval.strval = strdup(">");return COMPARISON; 
#line 242 "mylexer.c"
		}
		break;
	case 13:
		{
#line 24 ".\\mylexer.l"
 yylval.strval = strdup(">=");return COMPARISON; 
#line 249 "mylexer.c"
		}
		break;
	case 14:
		{
#line 25 ".\\mylexer.l"
 yylval.strval = strdup("<");return COMPARISON; 
#line 256 "mylexer.c"
		}
		break;
	case 15:
		{
#line 26 ".\\mylexer.l"
 yylval.strval = strdup("<=");return COMPARISON; 
#line 263 "mylexer.c"
		}
		break;
	case 16:
		{
#line 27 ".\\mylexer.l"
 yylval.strval = strdup("!=");return COMPARISON; 
#line 270 "mylexer.c"
		}
		break;
	case 17:
		{
#line 29 ".\\mylexer.l"
 return AND; 
#line 277 "mylexer.c"
		}
		break;
	case 18:
		{
#line 30 ".\\mylexer.l"
 return OR; 
#line 284 "mylexer.c"
		}
		break;
	case 19:
		{
#line 35 ".\\mylexer.l"
 yylval.strval = strdup(yytext); return ID; 
#line 291 "mylexer.c"
		}
		break;
	case 20:
		{
#line 36 ".\\mylexer.l"
 yylval.intval = atoi(yytext); return INTEGER; 
#line 298 "mylexer.c"
		}
		break;
	case 21:
		{
#line 37 ".\\mylexer.l"
 return *yytext; 
#line 305 "mylexer.c"
		}
		break;
	case 22:
	case 23:
		{
#line 39 ".\\mylexer.l"
 yylval.strval = strdup(yytext); return STRING; 
#line 313 "mylexer.c"
		}
		break;
	case 24:
		{
#line 40 ".\\mylexer.l"
;
#line 320 "mylexer.c"
		}
		break;
	case 25:
		{
#line 41 ".\\mylexer.l"
 yyerror("ÎÞÐ§×Ö·û"); 
#line 327 "mylexer.c"
		}
		break;
	default:
		yyassert(0);
		break;
	}
	yyreturnflg = YYFALSE;
	return 0;
}

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn .rch		/* <warning: unreachable code> to the old state */
#endif
#endif
YYCONST yymatch_t YYNEARFAR YYBASED_CODE YYDCDECL yymatch[] = {
	0
};

int YYNEAR YYDCDECL yytransitionmax = 196;
YYCONST yytransition_t YYNEARFAR YYBASED_CODE YYDCDECL yytransition[] = {
	{ 0, 0 },
	{ 4, 1 },
	{ 4, 1 },
	{ 0, 42 },
	{ 33, 15 },
	{ 49, 37 },
	{ 50, 37 },
	{ 35, 16 },
	{ 36, 17 },
	{ 37, 18 },
	{ 38, 19 },
	{ 39, 20 },
	{ 40, 21 },
	{ 41, 22 },
	{ 34, 15 },
	{ 25, 24 },
	{ 24, 25 },
	{ 28, 27 },
	{ 27, 28 },
	{ 44, 32 },
	{ 45, 33 },
	{ 46, 34 },
	{ 47, 35 },
	{ 48, 36 },
	{ 4, 1 },
	{ 5, 1 },
	{ 6, 1 },
	{ 42, 42 },
	{ 30, 10 },
	{ 51, 39 },
	{ 52, 40 },
	{ 7, 1 },
	{ 8, 1 },
	{ 8, 1 },
	{ 8, 1 },
	{ 8, 1 },
	{ 8, 1 },
	{ 8, 1 },
	{ 53, 41 },
	{ 8, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 9, 1 },
	{ 31, 12 },
	{ 8, 1 },
	{ 10, 1 },
	{ 11, 1 },
	{ 12, 1 },
	{ 0, 43 },
	{ 32, 13 },
	{ 13, 1 },
	{ 14, 1 },
	{ 15, 1 },
	{ 14, 1 },
	{ 16, 1 },
	{ 17, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 18, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 19, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 20, 1 },
	{ 21, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 22, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 54, 45 },
	{ 43, 43 },
	{ 26, 42 },
	{ 55, 46 },
	{ 56, 47 },
	{ 57, 48 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 14, 1 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 9, 9 },
	{ 58, 49 },
	{ 59, 50 },
	{ 60, 51 },
	{ 61, 52 },
	{ 62, 53 },
	{ 63, 55 },
	{ 64, 58 },
	{ 65, 60 },
	{ 66, 61 },
	{ 67, 62 },
	{ 68, 63 },
	{ 69, 64 },
	{ 29, 43 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 70, 65 },
	{ 23, 5 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 },
	{ 14, 70 }
};

YYCONST yystate_t YYNEARFAR YYBASED_CODE YYDCDECL yystate[] = {
	{ 0, 0, 0 },
	{ -3, -8, 0 },
	{ 1, 0, 0 },
	{ 0, 0, 25 },
	{ 0, 0, 24 },
	{ 0, 104, 25 },
	{ 24, 0, 25 },
	{ 27, 0, 25 },
	{ 0, 0, 21 },
	{ 0, 67, 20 },
	{ 0, -33, 14 },
	{ 0, 0, 11 },
	{ 0, -11, 12 },
	{ 70, -22, 19 },
	{ 70, 0, 19 },
	{ 70, -68, 19 },
	{ 70, -81, 19 },
	{ 70, -74, 19 },
	{ 70, -69, 19 },
	{ 70, -72, 19 },
	{ 70, -58, 19 },
	{ 70, -53, 19 },
	{ 70, -59, 19 },
	{ 0, 0, 16 },
	{ 42, -19, 0 },
	{ 0, -18, 23 },
	{ 42, 0, 0 },
	{ 43, -22, 0 },
	{ 0, -21, 22 },
	{ 43, 0, 0 },
	{ 0, 0, 15 },
	{ 0, 0, 13 },
	{ 70, -49, 19 },
	{ 70, -45, 19 },
	{ 70, -48, 19 },
	{ 70, -51, 19 },
	{ 70, -56, 19 },
	{ 70, -78, 19 },
	{ 70, 0, 18 },
	{ 70, -47, 19 },
	{ 70, -36, 19 },
	{ 70, -31, 19 },
	{ -24, -7, 23 },
	{ -27, 45, 22 },
	{ 70, 0, 17 },
	{ 70, 1, 19 },
	{ 70, 21, 19 },
	{ 70, 3, 19 },
	{ 70, 11, 19 },
	{ 70, 56, 19 },
	{ 70, 47, 9 },
	{ 70, 58, 19 },
	{ 70, 52, 19 },
	{ 70, 47, 19 },
	{ 70, 0, 10 },
	{ 70, 46, 19 },
	{ 70, 0, 8 },
	{ 70, 0, 6 },
	{ 70, 49, 19 },
	{ 70, 0, 4 },
	{ 70, 65, 19 },
	{ 70, 64, 19 },
	{ 70, 65, 19 },
	{ 70, 66, 19 },
	{ 70, 52, 19 },
	{ 70, 80, 19 },
	{ 70, 0, 2 },
	{ 70, 0, 7 },
	{ 70, 0, 1 },
	{ 70, 0, 3 },
	{ 0, 73, 5 }
};

YYCONST yybackup_t YYNEARFAR YYBASED_CODE YYDCDECL yybackup[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

#line 42 ".\\mylexer.l"


