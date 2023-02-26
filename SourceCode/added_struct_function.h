# ifndef _added_struct_function_h
# define _added_struct_function_h
# endif

# include <string>
# include <cstring>
# include <vector>

struct primary_struct
{
        int mode; // 0-5
        char id[33];
        int val;
        int op;
        int pos; // self_pos in global_prim_ls
        int nxt_id; // child's pos in global_prim_ls / global_expr_ls
        int lst_id; // pos of the parent expr/statement in global_expr_ls / stat_ls
        char result_reg[10];
        char lst_type[15]; // expression/statement
};

struct expr_struct {
        int mode; // 0-2
        int pos; // self_pos in global_expr_ls
        int nxt_prim_id; // pos in global_prim_ls 
        int nxt_expr_id; // pos in global_expr_ls
        int lst_id; // pos of the parent expr/statement in global_expr_ls / stat_ls
        int op;
        char result_reg[10];
        char lst_type[15]; // expression/statement
};

struct expr_list_struct
{
        struct expr_struct expr_ls[30];
        int size;
};

struct stat_struct
{
        int mode; // 0-2
        char id[33];
        int expr_id;
        int pos; // self_pos in global_stat_ls
        char id_ls[30][33];
        int id_ls_size;
        struct expr_struct expr_ls[30];
        int expr_ls_size;
};

struct id_list_struct
{
        char id_ls[30][33];
        int size;
};
extern struct primary_struct global_prim_ls[500];
extern struct expr_struct global_expr_ls[500];
extern struct stat_struct global_stat_ls[500];
extern int prim_num, expr_num, stat_num;

extern int saved_reg[8];
extern int temp_reg[8];
extern int bridge_reg[3];
extern int mem[50];
extern std::vector<std::pair<std::string, std::string>> symbol_table;
extern int* crt_saved_pt; // pointing to the first available saved_reg
extern int* crt_temp_pt;
extern int* crt_bridge_pt;
extern int* crt_mem_pt; 

extern std::vector<std::string> mips_code;

void assign(stat_struct* statement, char id[33], expr_struct* expression);
void read_func(stat_struct* statement, id_list_struct* id_list);
void write_func(stat_struct* statement, expr_list_struct* expr_list);

void pass_id_to_idlist(id_list_struct* id_list, char id[33]);
void pass_idlist_to_idlist(id_list_struct* target_id_list, id_list_struct* id_list, char id[33]);

void pass_expr_to_exprlist(expr_list_struct* expr_list, expr_struct* expression);
void pass_exprlist_to_exprlist(expr_list_struct* target_expr_list, expr_list_struct* expr_list, expr_struct* expression);

void pass_primary_to_expression(primary_struct* primary, expr_struct* expression);
void pass_expr_OP_pri_to_expr(expr_struct* target_expr, expr_struct* expression, primary_struct* primary, int op);
void pass_expr_OP_pri_to_expr(expr_struct* target_expr, expr_struct* expression, primary_struct* primary, int op);

void pass_id_to_primary(char id[33], primary_struct* primary);
void pass_integer_to_primary(int integer, primary_struct* primary);
void pass_OP_primary_to_primary(primary_struct* target_primary, primary_struct* primary, int op);
void pass_L_expr_R_to_primary(primary_struct* primary, expr_struct* expression);

void add_to_table(char var[33], std::string addr);
std::string search_table(char var[33]);

void compute_expression(struct expr_struct* expression); // get the reg storing the result
std::string store_one_cal(char op[5], char id[33], std::string var1, std::string var2); // store id, id = var1 op var2 (store:move pt, gen mips, add table)
void code_gen_assign(); // return the reg storing the assigned val
void code_gen_read(char id_ls[30][33], int size);
void code_gen_write(struct expr_struct expr_ls[30], int size);
void code_gen(); // when iterating global_stat_list, label all elements' pos and save these pos to their direct children.

std::string print_prim(primary_struct* primary);
std::string print_expr(expr_struct* expr);