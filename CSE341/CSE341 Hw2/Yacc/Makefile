target: test

test:
	flex gpp_lexer.l 
	yacc -d gpp_interpreter.y
	cc lex.yy.c y.tab.c
	./a.out
