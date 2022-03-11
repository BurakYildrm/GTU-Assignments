%{
    #include "gpp_interpreter.h"
    int yylex();
    void yyerror(char* s);
%}

%union {
    int val;
    char id[16];
    void* generic;
}

%start START
%token KW_AND
%token KW_OR
%token KW_NOT
%token KW_EQUAL
%token KW_LESS
%token KW_NIL
%token KW_LIST
%token KW_APPEND
%token KW_CONCAT
%token KW_SET
%token KW_DEFFUN
%token KW_FOR
%token KW_IF
%token KW_EXIT
%token KW_LOAD
%token KW_DISP
%token KW_TRUE
%token KW_FALSE
%token OP_PLUS
%token OP_MINUS
%token OP_DIV
%token OP_MULT
%token OP_OP
%token OP_CP
%token OP_DBLMULT
%token OP_OC
%token OP_CC
%token OP_COMMA
%token COMMENT
%token KW_WHILE
%token KW_DEFVAR
%token<val> VALUE
%token<id> IDENTIFIER
%type<generic> VALUES EXPLISTI LISTVALUE
%type<val> EXPI EXPB INPUT START
%type<generic> IDLIST

%%
START: INPUT {printf("SYNTAX RECOGNIZED.\nRESULT: %d\n", $1);}
| START INPUT {printf("SYNTAX RECOGNIZED.\nRESULT: %d\n", $2);}
;
INPUT: EXPI
| EXPLISTI
| COMMENT
;
LISTVALUE: '\'' OP_OP VALUES OP_CP {$$ = $3;}
| '\'' OP_OP OP_CP
| KW_NIL {$$ = NULL;}
;
VALUES: VALUES VALUE {addInt($1, $2); $$ = $1;}
| VALUE {$$ = createIntList($1);}
;
EXPI: VALUE {$$ = $1;}
| IDENTIFIER {$$ = getIdVal($1);}
| OP_OP OP_PLUS EXPI EXPI OP_CP {$$ = $3 + $4;}
| OP_OP OP_MINUS EXPI EXPI OP_CP {$$ = $3 - $4;}
| OP_OP OP_MULT EXPI EXPI OP_CP {$$ = $3 * $4;}
| OP_OP OP_DIV EXPI EXPI OP_CP {$$ = $3 / $4;}
| OP_OP IDENTIFIER EXPLISTI OP_CP
| OP_OP KW_SET IDENTIFIER EXPI OP_CP {$$ = $4; updateVar($3, $4);}
| OP_OP KW_DEFFUN IDENTIFIER IDLIST EXPLISTI OP_CP
| OP_OP KW_IF EXPB EXPLISTI OP_CP
| OP_OP KW_IF EXPB EXPLISTI EXPLISTI OP_CP
| OP_OP KW_WHILE OP_OP EXPB OP_CP EXPLISTI OP_CP
| OP_OP KW_FOR OP_OP IDENTIFIER EXPI EXPI OP_CP EXPLISTI OP_CP
| OP_OP KW_DEFVAR IDENTIFIER EXPI OP_CP {$$ = $4; addVar($3, $4);}
;
EXPB: OP_OP KW_AND EXPB EXPB OP_CP {$$ = $3 && $4;}
| OP_OP KW_OR EXPB EXPB OP_CP {$$ = $3 || $4;}
| OP_OP KW_NOT EXPB OP_CP {$$ = !$3;}
| OP_OP KW_EQUAL EXPB EXPB OP_CP {$$ = ($3 == $4) ? 1 : 0;}
| OP_OP KW_EQUAL EXPI EXPI OP_CP {$$ = ($3 == $4) ? 1 : 0;}
| KW_TRUE {$$ = 1;}
| KW_FALSE {$$ = 0;}
;
EXPLISTI: OP_OP KW_CONCAT EXPLISTI EXPLISTI OP_CP
| OP_OP KW_APPEND EXPI EXPLISTI OP_CP
| LISTVALUE
;
IDLIST: IDLIST IDENTIFIER
| IDENTIFIER
;
%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
    yyparse();
}
