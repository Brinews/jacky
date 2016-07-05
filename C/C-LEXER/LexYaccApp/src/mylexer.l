%{
#include "defs.h"
#include "myparser.h"
%}
%option noyywrap
%option caseless
%%
CREATE      { return CREATE; }
TABLE       { return TABLE; }
INSERT      { return INSERT; }
INTO        { return INTO; }
SELECT      { return SELECT; }
FROM        { return FROM; }
WHERE       { return WHERE; }

EXIT        { return EXIT; }

INT         { return INT; }
CHAR        { return CHAR; }


"="         { yylval.strval = strdup("=");return COMPARISON; }
">"         { yylval.strval = strdup(">");return COMPARISON; }
">="         { yylval.strval = strdup(">=");return COMPARISON; }
"<"         { yylval.strval = strdup("<");return COMPARISON; }
"<="         { yylval.strval = strdup("<=");return COMPARISON; }
"!="         { yylval.strval = strdup("!=");return COMPARISON; }

AND         { return AND; }
OR          { return OR; }




[a-zA-Z]+   { yylval.strval = strdup(yytext); return ID; }
[0-9]+      { yylval.intval = atoi(yytext); return INTEGER; }
[-+*/;(),=]    { return *yytext; }
'(\\.|''|[^'\n])*' |
\"(\\.|\"\"|[^"\n])*\"  { yylval.strval = strdup(yytext); return STRING; } 
[ \t\n]     ;
.           { yyerror("ÎÞÐ§×Ö·û"); }
%%
