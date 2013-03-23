CC = gcc
CFLAGS = -I. -Werror

LEXLIB = -lfl

LexicalAnalyser: lex.yy.c
	$(CC) $(CFLAGS) lex.yy.c $(LEXLIB) -o C

lex.yy.c: lexical.l utility.h
	flex lexical.l

clean:
	rm lex.yy.c
	rm C
