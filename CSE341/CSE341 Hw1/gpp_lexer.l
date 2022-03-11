%{
    int flag = 1;
%}

value [0]|[1-9][0-9]*|[0][.][0-9]+|[1-9]+[.][0-9]+
leading_zero_error [0][0-9]*|[0][0-9]*[.][0-9]+
general_error [.][0-9]*|[0-9]+[.]|.

%%
"and"                   {printf("KW_AND\n");}
"or"                    {printf("KW_OR\n");}
"not"                   {printf("KW_NOT\n");}
"equal"                 {printf("KW_EQUAL\n");}
"less"                  {printf("KW_LESS\n");}
"nil"                   {printf("KW_NIL\n");}
"list"                  {printf("KW_LIST\n");}
"append"                {printf("KW_APPEND\n");}
"concat"                {printf("KW_CONCAT\n");}
"set"                   {printf("KW_SET\n");}
"deffun"                {printf("KW_DEFFUN\n");}
"for"                   {printf("KW_FOR\n");}
"if"                    {printf("KW_IF\n");}
"exit"                  {printf("KW_EXIT\n");}
"load"                  {printf("KW_LOAD\n");}
"disp"                  {printf("KW_DISP\n");}
"true"                  {printf("KW_TRUE \n");}
"false"                 {printf("KW_FALSE\n");}
"+"                     {printf("OP_PLUS\n");}
"-"                     {printf("OP_MINUS\n");}
"/"                     {printf("OP_DIV\n");}
"*"                     {printf("OP_MULT\n");}
"("                     {printf("OP_OP\n");}
")"                     {printf("OP_CP\n");}
"**"                    {printf("OP_DBLMULT\n");}
"\""                    {if(flag == 1){printf("OP_OC\n");flag = 2;} else{printf("OP_CC\n");flag = 1;}}
","                     {printf("OP_COMMA\n");}
";;".*                  {printf("COMMENT\n");}
{value}                 {printf("VALUE\n");}
{leading_zero_error}    {printf("SYNTAX ERROR INTEGERS CANNOT START WITH LEADING ZEROS\n");}
[a-zA-Z][a-zA-Z0-9]*    {printf("IDENTIFIER\n");}
[0-9][a-zA-Z0-9]+       {printf("SYNTAX ERROR IDENTIFIERS CANNOT START WITH LEADING DIGITS");}
[ \t\n]                 {;}
{general_error}         {printf("SYNTAX ERROR %s CANNOT BE TOKENIZED\n", yytext);}
%%

yywrap() {}

int main(){
    extern FILE* yyin;
    yyin = fopen("input.g++", "r");
    yylex();
    return 0;
}