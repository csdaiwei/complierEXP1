#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "tree.h"
#include "table.h"

void semantic_analyze(struct tree_node *);

void analyze_extdef_node(struct tree_node *);

void analyze_function_node(struct tree_node*, struct tree_node*, struct tree_node*);

void analyze_compst_node(struct func_descriptor*, struct tree_node*);

struct func_descriptor* create_function(struct tree_node*, struct tree_node*);

struct var_descriptor* create_variable(struct tree_node*, struct tree_node*, struct var_descriptor*);

struct struct_descriptor* create_structure(struct tree_node*);

int init_member_list(struct var_descriptor*, struct tree_node*);

int init_param_list(struct var_descriptor*, struct tree_node*);

void check_stmt_valid(struct tree_node*);

struct var_descriptor* check_exp_valid(struct tree_node*);


#endif