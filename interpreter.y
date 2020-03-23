/***
 Purpose: Assign expression values to variables.
 Expressions can either be addition, subtraction,
 multiplication, or division between integers or
 previously assigned variables. The expressions
 should be in a hierarchy that interprets the 
 order of operations correctly. Be able to return
 the stored value of an assigned variable by calling
 the name as a single line command.
**/

%{
#include <iostream>
#include <map>
#include "tree.h"
#include <cstring>
#include <vector>

extern "C" int yylex();
extern "C" int yyparse();

void yyerror(const char *s);

// Helper function to compare const char * for map iterator

/*
class StrCompare {
public:
  bool operator ()(const char*a, const char*b) const {
	return strcmp(a,b) < 0;
  }
};
*/

node* find_root(node *n){
	node *curr_node = n;
	while(true){
		//cout << curr_node<<endl;
		if (curr_node -> has_parent == 0){
			return curr_node;
		}
		else{
					
			curr_node = (*curr_node).parent;

		}
		
	}
}

map<char*, Dual*, StrCompare> var;
map<char*, node*,StrCompare> function_var;

vector<Dual> dual_vec;
vector<Dual*> pointer_vec;

int start = 1;
node *program;
node *found_root;

%}

/*** union of all possible data return types from grammar ***/
%union {
	double iVal;
	char* sVal;
	Dual *dVal;
	int lVal;//logical values
	node *nVal;

}

/*** Define token identifiers for flex regex matches ***/
%token FLOAT
%token SEPERATOR
%token SHORTHAND
%token NUM_F
%token VARNAME
%token ADD SUB MULT DIV EQUALS
%token LPAREN RPAREN
%token WS
%token EOL
%token GREATER
%token LESS
%token LOGEQUAL 
%token GEQUAL
%token LEQUAL 
%token EOP
%token BOL
%token BOP
%token WHILE IF
%token END
%token RCURLY
%token LCURLY
%token PRINT 
%token MAX
%token ROOTS
%token HIGH LOW POINTS
%token EXP LOG POW SIN ASIN COS ACOS TAN ATAN RAISE
%token DEF FUNCNAME 

/*** Define return type for grammar rules ***/

%type<nVal> expr
%type<dVal> DUAL
%type<iVal> FLOAT
%type<sVal> VARNAME
%type<sVal> FUNCNAME
%type<nVal> line 
%type<nVal> MLINE 
// operator precdence 
%left  ADD SUB 
%right MULT DIV 
%right RAISE
%%

start: MLINE  {
				program = new node;
				program = find_root($1);
				program -> forward();
				//cout << "here3"<<endl;
				//cout << $1  <<endl;
				//cout << (*$1).val ->  value <<endl;
				//cout << "here4"<<endl;
			 } start
	 | /* NULL */
	 ;

MLINE:BOP line EOP{
	$$ = $2;
	
}

line: BOL expr EOL{
	$$ = $2;
	

	}
	|line line{
		//want to connect the last node of the first line 
		//to the first node of the second line. 
	$$ = $2;
	found_root = find_root($2);
	$1 -> next_node = found_root;
	$1 -> has_child = 1;
	found_root->has_parent = 1;
	found_root -> parent = $1;

	}
	|WHILE expr LCURLY line RCURLY {
		$$ = new node;
		//cout << "here" << endl;
		$$ -> node_type = "while";
		found_root = find_root($2);
		$$ -> bool_tree_leaf = $2;
		$$ -> bool_tree_root = found_root;
		found_root = find_root($4);
		$$ -> sub_program = found_root;

	}
	|IF expr LCURLY line RCURLY {
		$$ = new node;
		$$ -> node_type = "if";
		found_root = find_root($2);
		$$ -> bool_tree_leaf = $2;
		$$ -> bool_tree_root = found_root;
		found_root = find_root($4);
		$$ -> sub_program = found_root;

	}
	|MAX expr LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "extrema";
		found_root = find_root($4);
		$$ -> arg1 = $2;
		$$ -> arg2 = found_root;
		$$ -> bool_tree_leaf = $4;
		$$ -> variables = &var;
	}
	|ROOTS expr LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "roots";
		found_root = find_root($4);
		$$ -> arg1 = $2;
		$$ -> arg2 = found_root;
		$$ -> bool_tree_leaf = $4;
		$$ -> variables = &var;
	}
	|MAX expr HIGH expr LOW expr POINTS expr LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "extrema2";
		found_root = find_root($10);
		$$ -> arg1 = $2;
		$$ -> arg2 = found_root;
		$$ -> arg3 = $4;
		$$ -> arg4 = $6;
		$$ -> arg5 = $8;
		$$ -> bool_tree_leaf = $10;
		$$ -> variables = &var;
	}
	|ROOTS expr HIGH expr LOW expr POINTS expr LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "roots2";
		found_root = find_root($10);
		$$ -> arg1 = $2;
		$$ -> arg2 = found_root;
		$$ -> arg3 = $4;
		$$ -> arg4 = $6;
		$$ -> arg5 = $8;
		$$ -> bool_tree_leaf = $10;
		$$ -> variables = &var;
	}
	|PRINT LCURLY expr RCURLY{
		$$ = new node;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;
		$$ -> node_type = "print";
		$$ -> arg1 = $3;
	}
	|DEF FUNCNAME expr LCURLY line RCURLY{
		$$ = new node;
		found_root = find_root($5);
		found_root -> func_arg = $3 -> look_up_var;
		function_var[$2] = found_root;
		
	}
	|BOL line EOL{
		$$ = $2;
	}

	

expr:expr ADD expr{
		//node type add
		$$ = new node;
		$$ -> variables = &var;
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		$$ -> node_type = "add";
		$$ -> has_parent = 1;
		found_root = find_root($3);
		(*found_root).has_parent = 1;
		(*found_root).parent = $1;
		$1 -> next_node = found_root;
		$1 -> has_child = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;
		$$ -> parent = $3;

	}

	|expr SUB expr{
		//node type sub
		$$ = new node;
		$$ -> variables = &var;
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		$$ -> node_type = "sub";
		$$ -> has_parent = 1;
		found_root = find_root($3);
		(*found_root).has_parent = 1;
		(*found_root).parent = $1;
		$1 -> next_node = found_root;
		$1 -> has_child = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;
		$$ -> parent = $3;
	}
	|expr MULT expr{
		//node type mult
		$$ = new node;
		$$ -> variables = &var;
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		$$ -> node_type = "mult";
		$$ -> has_parent = 1;
		found_root = find_root($3);
		(*found_root).has_parent = 1;
		(*found_root).parent = $1;
		$1 -> next_node = found_root;
		$1 -> has_child = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;
		$$ -> parent = $3;
	}
	|expr DIV expr{
		//node type div
		$$ = new node;
		$$ -> variables = &var;
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		$$ -> node_type = "div";
		$$ -> has_parent = 1;
		found_root = find_root($3);
		(*found_root).has_parent = 1;
		(*found_root).parent = $1;
		$1 -> next_node = found_root;
		$1 -> has_child = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;
		$$ -> parent = $3;

	}
	|expr RAISE expr{
		//node type div
		$$ = new node;
		$$ -> variables = &var;
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		$$ -> node_type = "raise";
		$$ -> has_parent = 1;
		found_root = find_root($3);
		(*found_root).has_parent = 1;
		(*found_root).parent = $1;
		$1 -> next_node = found_root;
		$1 -> has_child = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;
		$$ -> parent = $3;

	}
	|DUAL{
		$$ = new node;
		$$ -> variables = &var;
		$$ -> val = $1;
	}
	|VARNAME{
		//node type look up
		$$ = new node;
		$$ -> variables = &var;
		$$ -> look_up_var = $1;
		$$ -> node_type = "look up";


	}
	|LPAREN expr RPAREN{
		$$ = new node;
		$$ = $2;
	}
	|VARNAME EQUALS expr{
		//node type assign
		$$ = new node;
		$$ -> variables = &var;
		$$ -> assign_to = $1;
		$$ -> arg1 = $3;
		$$ -> node_type = "assign";
		$$ -> has_parent =1;
		$$ -> parent = $3;
		$3 -> has_child = 1;
		$3 -> next_node = $$;
	}
	|expr GREATER expr{
		$$ = new node;
		$$ -> node_type = "boolG";
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		found_root = find_root($3);
		found_root -> has_parent = 1;
		found_root -> parent = $1;
		$1 -> has_child = 1;
		$1 -> next_node = found_root;
		$3 -> has_child = 1;
		$3 -> next_node = $$;
		$$ -> has_parent = 1;
		$$ -> parent = $3;
		
	}
	|expr LESS expr{
		$$ = new node;
		$$ -> node_type = "boolL";
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		found_root = find_root($3);
		found_root -> has_parent = 1;
		found_root -> parent = $1;
		$1 -> has_child = 1;
		$1 -> next_node = found_root;
		$3 -> has_child = 1;
		$3 -> next_node = $$;
		$$ -> has_parent = 1;
		$$ -> parent = $3;
		
	}
	|expr LOGEQUAL expr{
		$$ = new node;
		$$ -> node_type = "boolE";
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		found_root = find_root($3);
		found_root -> has_parent = 1;
		found_root -> parent = $1;
		$1 -> has_child = 1;
		$1 -> next_node = found_root;
		$3 -> has_child = 1;
		$3 -> next_node = $$;
		$$ -> has_parent = 1;
		$$ -> parent = $3;
		
	}
	|expr GEQUAL expr{
		$$ = new node;
		$$ -> node_type = "boolGE";
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		found_root = find_root($3);
		found_root -> has_parent = 1;
		found_root -> parent = $1;
		$1 -> has_child = 1;
		$1 -> next_node = found_root;
		$3 -> has_child = 1;
		$3 -> next_node = $$;
		$$ -> has_parent = 1;
		$$ -> parent = $3;
		
	}
	|expr LEQUAL expr{
		$$ = new node;
		$$ -> node_type = "boolLE";
		$$ -> arg1 = $1;
		$$ -> arg2 = $3;
		found_root = find_root($3);
		found_root -> has_parent = 1;
		found_root -> parent = $1;
		$1 -> has_child = 1;
		$1 -> next_node = found_root;
		$3 -> has_child = 1;
		$3 -> next_node = $$;
		$$ -> has_parent = 1;
		$$ -> parent = $3;
		
	}
	|EXP LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "exp";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|LOG LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "log";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|SIN LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "sin";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|ASIN LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "asin";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|COS LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "cos";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|ACOS LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "acos";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|TAN LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "tan";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|ATAN LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "atan";
		$$ -> arg1 = $3;
		$$ -> parent = $3;
		$$ -> has_parent = 1;
		$3 -> next_node = $$;
		$3 -> has_child = 1;

	}
	|POW LCURLY expr SEPERATOR expr RCURLY{
		$$ = new node;
		$$ -> node_type = "pow";
		$$ -> arg1 = $3;
		$$ -> arg2 = $5;
		$3 -> next_node = $5;
		$3 -> has_child = 1;
		$5 -> parent = $3;
		$5 -> has_parent = 1;
		$5 -> next_node = $$;
		$5 -> has_child = 1;
		$$ -> parent = $5;
		$$ -> has_parent = 1;
		

	}
	|FUNCNAME LCURLY expr RCURLY{
		$$ = new node;
		$$ -> node_type = "func_call";
		$$ -> func_name = $1;
		$$ -> has_parent = 1;
		$$ -> parent = $3;
		$$ -> arg1 = $3;
		$3 -> has_child = 1;
		$3 -> next_node = $$;
		$$ -> functions = &function_var;
		$$ -> variables = &var;
	}
	
DUAL:FLOAT SHORTHAND{
	
	pointer_vec.push_back(new Dual);
	$$ = pointer_vec.back();
	$$ -> value = $1;
	$$ -> der_1 = 0.0;
	$$ -> der_2 = 0.0;
	}
	|FLOAT SEPERATOR FLOAT SHORTHAND{
	
	pointer_vec.push_back(new Dual);
	$$ = pointer_vec.back();
	$$ -> value = $1;

	$$ -> der_1 = $3;
	$$ -> der_2 = 0.0;
	}
	|FLOAT SEPERATOR FLOAT SEPERATOR FLOAT{
	
	pointer_vec.push_back(new Dual);
	$$ = pointer_vec.back();
	$$ -> value = $1;
	$$ -> der_1 = $3;
	$$ -> der_2 = $5;
	}
	
	
	




%%

int main(int argc, char **argv) {
	yyparse();
}

/* Display error messages */
void yyerror(const char *s) {
	printf("ERROR: %s\n", s);
}
