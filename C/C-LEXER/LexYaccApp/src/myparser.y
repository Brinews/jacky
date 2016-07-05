%{
#include "defs.h"
%}

%union{
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
}
%token CREATE TABLE INSERT INTO SELECT FROM WHERE INT CHAR EXIT
%token <strval> ID COMPARISON STRING
%token <intval> INTEGER
%type <intval> program
%type <strval>  statements
%type <cfdef_var> fieldsdefinition field_type
%type <cs_var> create_stmt
%type <ifdef_var> ifieldsdefinition ifield_type
%type <is_var> insert_stmt
%type <sf_var> fields_star fields
%type <st_var> tables 
%type <cons_var> condition where_conditions
%type <ss_var> select_stmt

%token <strval> AND OR
%left '+' '-'
%left '*' '/'
//---------------------------------------------------------------------------------------------------------------
%%
program: statements             { $$ = 1; printf("<%6s>\n\n",$1); }  
       | program statements     { $$ = 1; printf("<%6s>\n\n",$2); }
       | program error ';'      { $$ = 1; printf("<%6s>\n\n","error"); }
       | error ';'              { $$ = 1; printf("\n"); } 
       | program EXIT ';'       { exit(0); } 
       | EXIT ';'               { exit(0); }
       ; 
statements: ';' { $$ = "empty"; }
          | create_stmt ';' { $$ = "create"; analysis_create($1); }
          | insert_stmt ';' { $$ = "insert"; analysis_insert($1); }
          | select_stmt ';' { $$ = "select"; analysis_select($1); }
          ;
create_stmt:
           CREATE TABLE ID '(' fieldsdefinition ')' 
           { 
                $$ = (struct Createdef *)malloc(sizeof(struct Createdef));
                $$ -> table = strdup($3);
                $$ -> cdef = $5; 
                free($3);
           }
           ;
fieldsdefinition: field_type 
                | fieldsdefinition ',' field_type
                {
                    struct Createfieldsdef *temp;// = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
                    temp = (struct Createfieldsdef *)$1;
                    while(temp->next_cdef){
                        temp = temp->next_cdef;
                    }
                    temp->next_cdef = $3;
                }
                ;
field_type: ID INT     
          {
                $$ = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
                memset($$, 0, sizeof(struct Createfieldsdef));
                $$ -> col = strdup($1);
                $$ -> type = _INT;
                free($1);
          }
          | ID CHAR '(' INTEGER ')'
          {
                $$ = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
                memset($$, 0, sizeof(struct Createfieldsdef));
                $$ -> col = strdup($1);
                $$ -> type = _CHAR;
                $$ -> length = $4;
                free($1);                 
          }
          ;
insert_stmt: INSERT INTO ID '(' ifieldsdefinition ')'
           {
                $$ = (struct Insertdef *)malloc(sizeof(struct Insertdef));
                $$->table = strdup($3);
                $$->idef = $5;
                free($3);
           }
           ;
ifieldsdefinition: ifield_type
                 | ifieldsdefinition ',' ifield_type 
                 {
                    struct Insertfieldsdef *temp;// = (struct Insertfieldsdef *)malloc(sizeof(struct Insertfieldsdef));
                    temp = (struct Insertfieldsdef *)$1;
                    while(temp->next_idef){
                        temp = temp->next_idef;
                    }
                    temp->next_idef = $3;
                 }           
                 ;
ifield_type: ID
           { 
                $$ = (struct Insertfieldsdef *)malloc(sizeof(struct Insertfieldsdef));
                memset($$, 0, sizeof(struct Insertfieldsdef));
                $$->type = _CHAR;
                $$->strcont = strdup($1);
                free($1);
           }
           | INTEGER
           {
                $$ = (struct Insertfieldsdef *)malloc(sizeof(struct Insertfieldsdef));
                memset($$, 0, sizeof(struct Insertfieldsdef));
                $$->type = _INT;
                $$->intcont = $1;
           }
           ;

select_stmt: SELECT fields_star FROM tables where_conditions
           {
                struct Selectedfields *temp;
                
                printf("Tables: %s\n", $4->table);
                printf_Selectcondition($5);
                if(!strcmp($2->field,"*")){
                    printf("*");
                }
                else if($2->field){
                    printf("\n");
                    temp = (struct Selectedfields *) $2;
                    while(temp){
                        printf("-%s- ",temp->field);
                        temp = temp->next_sf;
                    }
                }
           }
           ;
fields_star: fields { $$ = $1; }
           | '*'
           {
                $$ = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
                memset($$, 0, sizeof(struct Selectedfields));
                $$->field = "*";
           }
           ;
fields: ID  
      {
            $$ = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
            memset($$, 0, sizeof(struct Selectedfields));
            $$->field = strdup($1);
            free($1);
      } 
      | fields ',' ID
      {
            struct Selectedfields *temp;// = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
            
            struct Selectedfields *temp_next = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
            memset(temp_next, 0, sizeof(struct Selectedfields));
            
            temp = (struct Selectedfields *)$1;
            
            while(temp->next_sf){
                temp = temp->next_sf;
            }
            temp_next->field = strdup($3);
            temp->next_sf = temp_next;
            free($3);
      }
      ;
tables: ID 
      {
            $$ = (struct Selectedtables *)malloc(sizeof(struct Selectedtables));
            memset($$, 0, sizeof(struct Selectedtables));
            $$->table = strdup($1);
            free($1);
      }
      ;
where_conditions: /* empty */ { $$ = NULL; }
          | WHERE condition  
          {
            $$ = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$ = $2;
          }
          | where_conditions AND condition 
          {
            $$ = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->left = $1;
            $$->right = $3;
            $$->comp_op = "a";
          }
          | where_conditions OR condition
          {
            $$ = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->left = $1;
            $$->right = $3;
            $$->comp_op = "o";   
          }
          ;
condition: ID COMPARISON INTEGER
         {
            $$ = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->comp_op = strdup($2);
            $$->left = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->left->type = 0;
            $$->left->str_value = strdup($1);
            $$->right = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->right->type = 2;
            $$->right->int_value = $3;
            free($1);free($2);
         } 
         | ID COMPARISON STRING
         {
            $$ = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->comp_op = strdup($2);
            $$->left = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->left->type = 0;
            $$->left->str_value = strdup($1);
            $$->right = (struct Conditions *)malloc(sizeof(struct Conditions));
            $$->right->type = 1;
            $$->right->str_value = strdup($3);
            free($1);free($2);free($3);
         }
         ;


//----------------------------------------------------------------------------------------------------------------------
%%
void yyerror(char *s){
    printf("%s\n",s);
}
int main(void){
    printf("\n");
    yyparse();
    return 0;
}
