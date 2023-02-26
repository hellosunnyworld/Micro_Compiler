# include "added_struct_function.h"
# include <cstdio>
# include <iostream>
#include "parser.tab.h"

struct primary_struct global_prim_ls[500];
struct expr_struct global_expr_ls[500];
struct stat_struct global_stat_ls[500];
int prim_num;
int expr_num;
int stat_num;

int saved_reg[8];
int temp_reg[8];
int bridge_reg[3];
int mem[50];
std::vector<std::pair<std::string, std::string>> symbol_table;
int* crt_saved_pt; // pointing to the first available saved_reg
int* crt_temp_pt;
int* crt_bridge_pt;
int* crt_mem_pt; 
std::vector<std::string> mips_code;

void assign(stat_struct* statement, char id[33], expr_struct* expression){
        // statement: ID ASSIGNOP expression SEMICOLON
        printf("assign\n");
        statement -> mode = 0;
        strcpy(statement ->id, id);
        //printf(":=mode %d\n", statement -> mode);
        
        statement -> expr_id = expr_num;
        expression ->pos = expr_num;
        strcpy(expression ->lst_type, "statement");
        memcpy(global_expr_ls + expr_num, expression, sizeof(expr_struct));
        expr_num += 1;

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }

        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
        printf("----------------------\n");
}
void read_func(stat_struct* statement, id_list_struct* id_list){
        // statement: READ LPAREN id_list RPAREN SEMICOLON
        printf("read_func\n");
        statement -> mode = 1;
        //printf("read mode %d\n", statement -> mode);
        memcpy(statement -> id_ls, id_list -> id_ls, sizeof(id_list -> id_ls));
        statement ->id_ls_size = id_list ->size;


        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
void write_func(stat_struct* statement, expr_list_struct* expr_list){
        //statement: WRITE LPAREN expr_list RPAREN SEMICOLON
        printf("write_func\n");
        statement -> mode = 2;
        //printf("write mode %d\n", statement -> mode);
        memcpy(statement -> expr_ls, expr_list -> expr_ls, sizeof(expr_list -> expr_ls));
        statement -> expr_ls_size = expr_list ->size;
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}

void pass_id_to_idlist(id_list_struct* id_list, char id[33]){
        //id_list         : ID
        printf("pass_id_to_idlist\n");
        strcpy(id_list ->id_ls[id_list->size], id);
        id_list ->size += 1;
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
void pass_idlist_to_idlist(id_list_struct* target_id_list, id_list_struct* id_list, char id[33]){
        //id_list: id_list COMMA ID
        printf("pass_idlist_to_idlist\n");
        for (int i = 0; i < id_list->size; i++){
                strcpy(target_id_list->id_ls[target_id_list->size],  id_list->id_ls[i]);
                target_id_list->size += 1;
        }
        strcpy(target_id_list->id_ls[target_id_list->size],  id);
        target_id_list->size += 1;
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}

void pass_expr_to_exprlist(expr_list_struct* expr_list, expr_struct* expression){
        //expr_list       : expression
        printf("pass_expr_to_exprlist\n");
        expr_list->size = 1;
        expression->pos = 0;
        strcpy(expression->lst_type, "expr_list"); // ie. write_statement
        memcpy(expr_list->expr_ls, expression, sizeof(expr_struct));
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
void pass_exprlist_to_exprlist(expr_list_struct* target_expr_list, expr_list_struct* expr_list, expr_struct* expression){
        //expr_list: expr_list COMMA expression
        printf("pass_exprlist_to_exprlist\n");
        for (int i = 0; i < expr_list->size; i++){
                expr_list->expr_ls[i].pos = target_expr_list->size;
                memcpy(&target_expr_list->expr_ls[target_expr_list->size], &expr_list->expr_ls[i], sizeof(expr_list->expr_ls[i]));
                target_expr_list->size += 1;
                printf("passed expr %d\n", i);
        }
        printf("end passing expr_list\n");  

        expression->pos = target_expr_list->size;
        strcpy(expression->lst_type, "expr_list"); // ie. write_statement  
        memcpy(&target_expr_list->expr_ls[target_expr_list->size], expression, sizeof(expr_struct));  
        target_expr_list->size += 1;
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}

void pass_primary_to_expression(primary_struct* primary, expr_struct* expression){
        //expression      : primary
        printf("pass_primary_to_expression\n");
        expression ->mode = 0;
        //printf("expr mode %d\n", expression ->mode);
        
        expression ->nxt_prim_id = prim_num;

        primary ->pos = prim_num;
        strcpy(primary ->lst_type, "expression");
        primary -> lst_id = expression -> pos;
        memcpy(global_prim_ls + prim_num, primary, sizeof(primary_struct));
        prim_num += 1;
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
void pass_expr_OP_pri_to_expr(expr_struct* target_expr, expr_struct* expression, primary_struct* primary, int op){
        //expression: expression PLUOP/MIUSOP primary
        printf("pass_expr_OP_pri_to_expr\n");
        target_expr ->mode = 1;
        target_expr ->nxt_prim_id = prim_num;
        target_expr ->nxt_expr_id = expr_num;
        target_expr ->op = op;

        primary ->pos = prim_num;
        strcpy(primary ->lst_type, "expression");
        primary -> lst_id = target_expr -> pos;
        memcpy(global_prim_ls + prim_num, primary, sizeof(primary_struct));
        prim_num += 1;

        expression ->pos = expr_num;
        strcpy(expression ->lst_type, "expression");
        expression -> lst_id = target_expr -> pos;
        memcpy(global_expr_ls + expr_num, expression, sizeof(expr_struct));
        expr_num++;


        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}

void pass_id_to_primary(char id[33], primary_struct* primary){
        //primary         : ID
        printf("pass_id_to_primary\n");
        primary ->mode = 0;
        strcpy(primary ->id, id);
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
void pass_integer_to_primary(int integer, primary_struct* primary){
        //primary         : INTLITERAL
        printf("pass_integer_to_primary\n");
        primary ->mode = 1;
        primary ->val = integer;
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
void pass_OP_primary_to_primary(primary_struct* target_primary, primary_struct* primary, int op){
        //primary: PLUOP primary
        printf("pass_OP_primary_to_primary\n");
        target_primary ->mode = 2;
        target_primary ->op = op;
        target_primary ->nxt_id = prim_num;
        if (op == PLUOP)
                target_primary ->val = primary ->val;
        else if(op == MINUSOP)
                target_primary ->val = -primary ->val;

        primary ->pos = prim_num;
        strcpy(primary ->lst_type, "primary");
        primary -> lst_id = target_primary -> pos;
        memcpy(global_prim_ls + prim_num, primary, sizeof(primary_struct));
        prim_num += 1;
        

        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
void pass_L_expr_R_to_primary(primary_struct* primary, expr_struct* expression){
        //primary: LPAREN expression RPAREN
        printf("pass_L_expr_R_to_primary\n");
        primary ->mode = 4;
        primary ->nxt_id = prim_num;

        expression ->pos = expr_num;
        strcpy(expression ->lst_type, "primary");
        expression -> lst_id = primary -> pos;
        memcpy(global_expr_ls + expr_num, expression, sizeof(expr_struct));
        expr_num++;
        
        // for (int i = 0; i < expr_num; i++){
        //         printf("expr %d\n",i);
        //         printf("expr mode %d\n", global_expr_ls[i].mode);
        // }
        for (int i = 0; i < prim_num; i++){
                printf("prim %d\n",i);
                printf("prim mode %d\n", global_prim_ls[i].mode);
        }
}
//-------------------------------------------------------------------------------------------
void add_to_table(char var[33], std::string addr){
        //printf("add_to_table\n");
        symbol_table.push_back(std::make_pair(std::string(var), addr));
        
        // for (int i = 0; i < prim_num; i++){
        //         printf("stat %d\n",i);
        //         printf("prim value %d\n", global_prim_ls[i].val);
        // }
}
std::string search_table(char var[33]){
        //printf("search_table\n");
        std::string addr, cmd;
        if (crt_bridge_pt - bridge_reg > 1)
                crt_bridge_pt = bridge_reg;
        for (int i = 0; i < symbol_table.size(); i++){
                if (!strcmp(symbol_table[i].first.c_str(), var))
                        addr = symbol_table[i].second;
        }
        if (addr[addr.size() - 1] == ')'){
                cmd = "lw $t" + std::to_string(crt_bridge_pt - bridge_reg + 8) + ", " + addr;
                mips_code.push_back(cmd);
                addr = "$t" + std::to_string(crt_bridge_pt - bridge_reg + 8);
                crt_bridge_pt += 1;
        }
        return addr;
        // for (int i = 0; i < prim_num; i++){
        //         printf("stat %d\n",i);
        //         printf("prim value %d\n", global_prim_ls[i].val);
        // }
        // printf("end searching\n");
}
//-------------------------------------------------------------------------------------------
void compute_expression(struct expr_struct* target_expr){
// return the result reg
        //std::cout << "compute_expression " << print_expr(target_expr) << "\n" << std::endl;
        struct primary_struct* primary = global_prim_ls + target_expr ->nxt_prim_id;
        struct expr_struct* expression;

        std::string a;

        if (target_expr ->mode == 0){
                switch (primary->mode)
                {
                case 0:
                        strcpy(target_expr->result_reg, search_table(primary->id).c_str());
                        break;
                case 1: case 2:
                        strcpy(target_expr->result_reg, store_one_cal("addi", "temp!", "$zero", std::to_string(primary->val)).c_str());
                        break; 
                case 4:
                        expression = global_expr_ls + primary->nxt_id;
                        compute_expression(expression);
                        strcpy(target_expr->result_reg, expression->result_reg);
                        break;               
                default:
                        break;
                }
        }
        else if ((target_expr -> mode == 1) && (target_expr -> op == PLUOP)){
                struct expr_struct* expression = global_expr_ls + target_expr->nxt_expr_id;
                struct expr_struct* sub_expr;
                compute_expression(expression);
                std::string expr_result_reg(expression->result_reg);
                switch (primary->mode)
                {
                case 0:
                        strcpy(target_expr->result_reg, store_one_cal("add", "temp!", expr_result_reg, search_table(primary->id)).c_str());
                        break;
                case 1: case2: 
                        strcpy(target_expr->result_reg, store_one_cal("addi", "temp!", expr_result_reg, std::to_string(primary->val)).c_str());
                        break;
                case 4:
                        sub_expr = global_expr_ls + primary->nxt_id;
                        compute_expression(sub_expr);
                        strcpy(target_expr->result_reg, store_one_cal("add", "temp!", expr_result_reg, std::string(sub_expr->result_reg)).c_str());
                        break;               
                default:
                        break;
                }                
        }
        else if ((target_expr -> mode == 1) && (target_expr -> op == MINUSOP)){
                struct expr_struct* expression = global_expr_ls + target_expr->nxt_expr_id;
                struct expr_struct* sub_expr;
                compute_expression(expression);
                std::string expr_result_reg(expression->result_reg);
                switch (primary->mode)
                {
                case 0:
                        strcpy(target_expr->result_reg, store_one_cal("sub", "temp!", expr_result_reg, search_table(primary->id)).c_str());
                        break;
                case 1: case 2:
                        strcpy(target_expr->result_reg, store_one_cal("addi", "temp!", expr_result_reg, std::to_string(-primary->val)).c_str());
                        break;
                case 4:
                        sub_expr = global_expr_ls + primary->nxt_id;
                        compute_expression(sub_expr);
                        strcpy(target_expr->result_reg, store_one_cal("sub", "temp!", expr_result_reg, std::string(sub_expr->result_reg)).c_str());
                        break;               
                default:
                        break;
                }                
        }       
        
        // for (int i = 0; i < prim_num; i++){
        //         printf("stat %d\n",i);
        //         printf("prim value %d\n", global_prim_ls[i].val);
        // }
}

std::string store_one_cal(char op[5], char id[33], std::string var1, std::string var2){
// store id, id = var1 op var2 (store:move pt, gen mips, add table)
        //printf("store_one_cal\n");
        std::string cmd, addr, crt_reg_id;

        if (var1[var1.size() - 1] == ')'){
                cmd = "lw $t" + std::to_string(crt_bridge_pt - bridge_reg + 8) + ", " + var1;
                mips_code.push_back(cmd);
                var1 = "$t" + std::to_string(crt_bridge_pt - bridge_reg + 8);
                crt_bridge_pt += 1;
        }
        if (var2[var2.size() - 1] == ')'){
                cmd = "lw $t" + std::to_string(crt_bridge_pt - bridge_reg + 8) + ", " + var2;
                mips_code.push_back(cmd);
                var2 = "$t" + std::to_string(crt_bridge_pt - bridge_reg + 8);
                crt_bridge_pt += 1;
        }
        
        std::string ops(op);
        cmd = ops + " ";
        if (strcmp(id, "temp!") && (crt_saved_pt - saved_reg < 8)){
                addr = "$s" + std::to_string(crt_saved_pt - saved_reg);
                cmd += addr + ", "+ var1 + ", " + var2;
                crt_saved_pt += 1;
                mips_code.push_back(cmd);
        }
        else if (strcmp(id, "temp!") == 0) {
                if (var1[1] == 't')
                        addr = var1;
                else if (var2[1] == 't')
                        addr = var2;
                else{
                        if (crt_temp_pt - temp_reg >= 8)
                                crt_temp_pt = temp_reg;
                        addr = "$t" + std::to_string(crt_temp_pt - temp_reg);
                        crt_temp_pt += 1;
                }
                cmd += addr + ", "+ var1 + ", " + var2;
                mips_code.push_back(cmd);
        }
        else{
                crt_bridge_pt = bridge_reg;
                crt_reg_id = std::to_string(8 + crt_bridge_pt - bridge_reg); // bridge regs start from t8
                cmd += "$t" + crt_reg_id + ", "+ var1 + ", " + var2; 
                mips_code.push_back(cmd);
                addr = std::to_string(crt_mem_pt - mem) + "($sp)";
                cmd = "sw $t" + crt_reg_id + ", " + addr;
                crt_mem_pt += 1;
                mips_code.push_back(cmd);
        }     
        if (strcmp(id, "temp!"))
                add_to_table(id, addr);

        
        // for (int i = 0; i < prim_num; i++){
        //         printf("stat %d\n",i);
        //         printf("prim value %d\n", global_prim_ls[i].val);
        // }
        return addr;
}
void code_gen_read(char id_ls[30][33], int size){
        printf("code_gen_read\n");
        for (int i = 0; i < size; i++){
                mips_code.push_back("addi $v0, $zero, 5");
                mips_code.push_back("syscall");
                store_one_cal("add", id_ls[i], std::string("$v0"), std::string("$zero"));
        }
        
        // for (int i = 0; i < prim_num; i++){
        //         printf("stat %d\n",i);
        //         printf("prim value %d\n", global_prim_ls[i].val);
        // }
}
void code_gen_write(struct expr_struct expr_ls[30], int size){
        printf("code_gen_write %d\n", size);
        for (int i = 0; i < size; i++){
                compute_expression(expr_ls + i);
                mips_code.push_back("addi $v0, $zero, 1");
                mips_code.push_back(std::string("add $a0, ") + expr_ls[i].result_reg + ", $zero");
                mips_code.push_back("syscall");
        }
        
        // for (int i = 0; i < prim_num; i++){
        //         printf("stat %d\n",i);
        //         printf("prim value %d\n", global_prim_ls[i].val);
        // }
}
void code_gen(){
        printf("code_gen\n");

        expr_struct* expression;
        for (int i = 0; i < stat_num; i++){
                printf("---------------\n");
                printf("stat %d\n",i);
                crt_temp_pt = temp_reg;
                crt_bridge_pt = bridge_reg;
                // printf("prim mode %d\n",global_prim_ls[global_expr_ls[global_stat_ls[i].expr_id].nxt_prim_id].mode);
                global_stat_ls[i].pos = i;
                switch (global_stat_ls[i].mode)
                {
                case 0:
                        expression = global_expr_ls + global_stat_ls[i].expr_id;
                        expression->lst_id = i;
                        printf("precompute\n");
                        compute_expression(expression);
                        store_one_cal("add", global_stat_ls[i].id, std::string(expression->result_reg), std::string("$zero")).c_str();
                        break;
                case 1:
                        code_gen_read(global_stat_ls[i].id_ls, global_stat_ls[i].id_ls_size);
                        break;
                case 2:
                        code_gen_write(global_stat_ls[i].expr_ls, global_stat_ls[i].expr_ls_size);
                        break;
                default:
                        break;
                }
        }
        std::cout << "print!" << std::endl;
        for (int i = 0; i < mips_code.size(); i++)
                std::cout << mips_code[i] << std::endl;
}
// int main(){
//         printf("hello");
//         return 0;
// }

std::string print_prim(primary_struct* primary){
        std::string prim;
        switch(primary->mode){
        case 0:
                prim = std::string(primary->id);
                break;
        case 1: case 2:
                prim = std::to_string(primary->val);
                break;
        case 4:
                prim = "(" + print_expr(global_expr_ls + primary->nxt_id) + ")";
                break;
        default:
                prim = std::string("");
                break;
        }
        return prim;
}
std::string print_expr(expr_struct* expr){
        printf("print expr. mode %d\n", expr->mode);
        std::string expression = "";
        std::string str1, str2;
        switch (expr->mode)
        {
        case 0:
                expression = print_prim(global_prim_ls + expr->nxt_prim_id);
                break;
        case 1:
                printf("mode 1\n");
                if (expr->op == PLUOP){
                        printf("str1 ");
                        str1 = print_expr(global_expr_ls + expr->nxt_expr_id);
                        std::cout << str1 << std::endl;
                        str2 = print_prim(global_prim_ls + expr->nxt_prim_id);
                        printf("str2 ");
                        std::cout << str2 << std::endl;
                        expression = str1 + "+" + str2;
                }
                else{
                        printf("str1 ");
                        str1 = print_expr(global_expr_ls + expr->nxt_expr_id);
                        std::cout << str1 << std::endl;
                        str2 = print_prim(global_prim_ls + expr->nxt_prim_id);
                        printf("str2 ");
                        std::cout << str2 << std::endl;
                        expression = str1 + "-" + str2;
                }
                break;
        default:
                break;
        }
        printf("expr %s", expression);
        return expression;
}