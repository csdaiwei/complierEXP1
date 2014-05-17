/*translate syntax tree into intermediate representation*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "translate.h"
#include "intercode.h"
#include "../common/table.h"
#include "../common/tree.h"
#include "../common/tokenset.h"

int used_temp_num = 0;

void 
intermediate_generate(struct tree_node* program_node){

	assert(program_node -> unit_code == Program);

	//find each fundec and translate into intermediate code
	struct tree_node* extdef_node = program_node -> child -> child;
	while( extdef_node != NULL){

		if(extdef_node -> child -> sibling -> unit_code == FunDec)
			translate_func(extdef_node);
		extdef_node = extdef_node -> sibling -> child;
	}
}

void
translate_func(struct tree_node* extdef_node){

	assert(extdef_node -> child -> sibling -> unit_code == FunDec);

	struct tree_node* fundec_node = extdef_node -> child -> sibling;
	
	printf("Function %s :\n", (char *)fundec_node -> child -> unit_value);
	
	if(fundec_node -> child -> sibling -> sibling -> unit_code == VarList){
		struct tree_node* varlist_node = fundec_node -> child -> sibling -> sibling;
		while(true){
			struct tree_node* vardec_node = varlist_node -> child -> child -> sibling;
			if(vardec_node -> child -> unit_code == ID){
				printf("Param %s\n", (char *)vardec_node -> child -> unit_value);
			}
			else{
				printf("error, use array as a parameter\n");
			}
			if(varlist_node -> child -> sibling != NULL)
				varlist_node = varlist_node -> child -> sibling -> sibling;
			else
				break;
		}
	}

	struct tree_node* stmtlist_node = fundec_node -> sibling -> child -> sibling -> sibling;
	while(stmtlist_node -> child != NULL){
		translate_stmt(stmtlist_node -> child);
		stmtlist_node = stmtlist_node -> child -> sibling;
	}
}

void
translate_stmt(struct tree_node* stmt_node){

	assert(stmt_node -> unit_code == Stmt);
	struct tree_node* first_child = stmt_node -> child;

	if(first_child -> unit_code == Exp){
		translate_exp(first_child);
	} 
	else if(first_child -> unit_code == CompSt){
		struct tree_node* inner_stmtlist_node = first_child -> child -> sibling -> sibling;
		while(inner_stmtlist_node -> child != NULL){
			translate_stmt(inner_stmtlist_node -> child);
			inner_stmtlist_node = inner_stmtlist_node -> child -> sibling;
		}
	}
	else if(first_child -> unit_code == RETURN){
		translate_exp(first_child -> sibling);
		printf("RETURN ~~\n");
	}
	else if(first_child -> unit_code == IF){
		
	}
	else if(first_child -> unit_code == WHILE){

	}
	else
		assert(0);
}

struct operand*
translate_exp(struct tree_node* exp_node){

	assert(exp_node -> unit_code == Exp);

	//printf("hint exp\n");

	struct tree_node* first_child = exp_node -> child;
	struct tree_node* second_child = first_child -> sibling;

	//Exp -> ID
	if(first_child -> unit_code == ID && second_child == NULL){	
		char* var_name = first_child -> unit_value;
		struct operand* op = create_operand(OP_VARIABLE);
		op -> value = find_var_seq(var_table_head, var_name);
		return op;
	}
	//Exp -> INT 
	else if(first_child -> unit_code == INT && second_child == NULL) {
		struct operand* op = create_operand(OP_CONST_INT);
		op -> value = *((int *)first_child -> unit_value);
		return op;
	}
	//Exp -> FLOAT
	else if(first_child -> unit_code == FLOAT && second_child == NULL) {
		struct operand* op = create_operand(OP_CONST_INT);
		op -> float_value = *((float *)first_child -> unit_value);
		return op;
	}
	//Exp -> Exp ASSIGNOP Exp
	else if(first_child -> unit_code == Exp && second_child -> unit_code == ASSIGNOP) {	// assignments
		struct operand *left_op = translate_exp(first_child);
		struct operand *right_op = translate_exp(second_child -> sibling);

		char left_op_buf[10], right_op_buf[10];
		operand_to_string(left_op_buf, left_op);
		operand_to_string(right_op_buf, right_op);

		printf("%s := %s\n", left_op_buf, right_op_buf);

	}
	//Exp -> Exp plus/minus/star/div Exp
	else if(first_child -> unit_code == Exp && (/*second_child -> unit_code == AND ||
												second_child -> unit_code == OR ||
												second_child -> unit_code == RELOP ||*/
												second_child -> unit_code == PLUS ||
												second_child -> unit_code == MINUS ||
												second_child -> unit_code == STAR ||
												second_child -> unit_code == DIV)) {	// 2-operand operator expressions
		
	}
	//1-operand operators
	//including "LP RP" "MINUS" "NOT"
	else if(second_child -> unit_code == Exp) {
		
	}
	//func call
	else if(first_child -> unit_code == ID && second_child -> unit_code == LP) {
		
		//todo args

		struct operand* op = create_operand(OP_TEMP);
		op -> value = used_temp_num++;
		char op_buf[10];
		operand_to_string(op_buf, op);

		printf("%s := CALL %s\n", op_buf, (char*)first_child -> unit_value);

		return op;
	}
	//array call
	else if(first_child -> unit_code == Exp && second_child -> unit_code == LB) {
		
	}
	//struct member call
	else if(first_child -> unit_code == Exp && second_child -> unit_code == DOT) {
		
	}


}





