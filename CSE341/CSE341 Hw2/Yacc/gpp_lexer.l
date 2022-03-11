%{
    #include "y.tab.h"
    int flag = 1;
%}

value [0]|[1-9][0-9]*|[0][.][0-9]+|[1-9]+[.][0-9]+
leading_zero_error [0][0-9]*|[0][0-9]*[.][0-9]+
general_error [.][0-9]*|[0-9]+[.]|.

%%
"and"                   {return KW_AND;}
"or"                    {return KW_OR;}
"not"                   {return KW_NOT;}
"equal"                 {return KW_EQUAL;}
"less"                  {return KW_LESS;}
"nil"                   {return KW_NIL;}
"list"                  {return KW_LIST;}
"append"                {return KW_APPEND;}
"concat"                {return KW_CONCAT;}
"set"                   {return KW_SET;}
"deffun"                {return KW_DEFFUN;}
"for"                   {return KW_FOR;}
"if"                    {return KW_IF;}
"exit"                  {return KW_EXIT;}
"load"                  {return KW_LOAD;}
"disp"                  {return KW_DISP;}
"true"                  {return KW_TRUE ;}
"false"                 {return KW_FALSE;}
"+"                     {return OP_PLUS;}
"-"                     {return OP_MINUS;}
"/"                     {return OP_DIV;}
"*"                     {return OP_MULT;}
"("                     {return OP_OP;}
")"                     {return OP_CP;}
"**"                    {return OP_DBLMULT;}
"\""                    {if(flag == 1){return OP_OC;flag = 2;} else{return OP_CC;flag = 1;}}
","                     {return OP_COMMA;}
"while"                 {return KW_WHILE;}
"defvar"                {return KW_DEFVAR;}
";;".*                  {return COMMENT;}
{value}                 {yylval.val = atoi(yytext);return VALUE;}
{leading_zero_error}    {printf("SYNTAX ERROR INTEGERS CANNOT START WITH LEADING ZEROS");exit(1);}
[a-zA-Z][a-zA-Z0-9]*    {strcpy(yylval.id, yytext); return IDENTIFIER;}
[0-9][a-zA-Z0-9]+       {printf("SYNTAX ERROR IDENTIFIERS CANNOT START WITH LEADING DIGITS");exit(1);}
[ \t\n]                 {;}
{general_error}         {printf("SYNTAX ERROR %s CANNOT BE TOKENIZED", yytext);exit(1);}
%%

int yywrap(void) {return 1;}

/*int main(){
    extern FILE* yyin;
    yyin = fopen("input.g++", "r");
    yylex();
    return 0;
}*/