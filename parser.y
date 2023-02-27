%{
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <ctype.h>
        #include "added_struct_function.h"

        int yyerror (char *s);
        int yylex();
        extern "C" FILE *yyin;
%}

%union {char id[32];
        int integer;
        struct expr_struct expr_st;
        struct expr_list_struct exprlist_st;
        struct primary_struct primary_st;
        struct id_list_struct idlist_st;
        struct stat_struct stat_st;}

%start system_goal
%token BEGIN_ END 
%token READ WRITE
%token LPAREN RPAREN 
%token SEMICOLON COMMA
%token PLUOP MINUSOP 
%token ASSIGNOP 
%token SCANEOF
%token <id> ID
%token <integer> INTLITERAL
%type <expr_st> expression
%type <exprlist_st> expr_list
%type <primary_st> primary
%type <idlist_st> id_list
%type <stat_st> statement

%%
system_goal     : program SCANEOF {;}
                ;

program         : BEGIN_ statement_list END {code_gen();}
                ;

statement_list  : statement {memcpy(global_stat_ls + stat_num, &($1), sizeof(stat_struct)); stat_num++;}
                | statement_list statement {memcpy(global_stat_ls + stat_num, &($2), sizeof(stat_struct)); stat_num++;}
                ;

statement       : ID ASSIGNOP expression SEMICOLON {assign(&($$), $1, &($3));}
                | READ LPAREN id_list RPAREN SEMICOLON {read_func(&($$), &($3));}
                | WRITE LPAREN expr_list RPAREN SEMICOLON {write_func(&($$), &($3));}
                ;

id_list         : ID {pass_id_to_idlist(&($$), $1);}
                | id_list COMMA ID {pass_idlist_to_idlist(&($$), &($1), $3);}
                ;

expr_list       : expression {pass_expr_to_exprlist(&($$), &($1));}
                | expr_list COMMA expression {pass_exprlist_to_exprlist(&($$), &($1), &($3));}
                ;

expression      : primary {pass_primary_to_expression(&($1), &($$));}
                | expression PLUOP primary {pass_expr_OP_pri_to_expr(&($$), &($1), &($3), PLUOP);}
                | expression MINUSOP primary {pass_expr_OP_pri_to_expr(&($$), &($1), &($3), MINUSOP);}
                ;

primary         : ID {pass_id_to_primary($1, &($$));}
                | INTLITERAL {pass_integer_to_primary($1, &($$));}
                | PLUOP primary {pass_OP_primary_to_primary(&($$), &($2), PLUOP);}
                | MINUSOP primary {pass_OP_primary_to_primary(&($$), &($2), MINUSOP);}
                | LPAREN expression RPAREN {pass_L_expr_R_to_primary(&($$), &($2));}
                ;
%%

int yyerror(char *s) {
        printf("Syntax Error: %s\n", s);
        return 0;
}
int main(int argc, char const *argv[]){
        extern FILE* yyin;
        yyin = fopen(argv[1], "r");
        crt_saved_pt = saved_reg;
        crt_temp_pt = temp_reg;
        crt_bridge_pt = bridge_reg;
        crt_mem_pt = mem;
        prim_num = 0;
        expr_num = 0;
        stat_num = 0;

        prim = 0;

        printf("file opened\n");
        yyparse();
        printf("parse completed!");
        return 0;
}