%{
        #include <stdio.h>
        #include <string.h>
        #include "added_struct_function.h"
        #include "parser.tab.h"
        int yyerror(char *s);
        int yylex();
        int end = 0;
%}

id      [a-zA-Z]([a-zA-Z0-9]{0,31})
integer ([0-9])+
comment --(.)*\n

%%
"begin" {return BEGIN_;}
"end" {return END;}
"read" {return READ;}
"write" {return WRITE;}
"(" {return LPAREN;}
")" {return RPAREN;}
";" {return SEMICOLON;}
"," {return COMMA;}
"+" {return PLUOP;}
"-" {return MINUSOP;}
":=" {return ASSIGNOP;}
{id} {sscanf(yytext, "%s", yylval.id); return ID;}
{integer} {yylval.integer = atoi(yytext); return INTLITERAL;}
{comment} ;
[ \t\n] ;
<<EOF>> {if (!end) {end = 1; return SCANEOF;} yyterminate();}
%%

int yywrap (void) {return 1;}