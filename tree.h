#include <iostream>
#include <vector>
#include <map>
#include <cstring>
using namespace std;


 
class StrCompare {
public:
  bool operator ()(const char*a, const char*b) const {
	return strcmp(a,b) < 0;
  }
};


struct Dual{
    //Extended dual numbers that keep track of second derivatives
    // in addition to first derivatives. This will allow the 
    // use of newtons method to numerically find the extrema 
    // of univariate functions. 
    double value = 0; // function value 
    double der_1 = 0; // first derivative 
    double der_2 = 0; // second derivative 
    
};

struct node{
    node *next_node;
    node *sub_program;
    node *parent;
    int has_parent = 0;
    const char* node_type;
    char* assign_to;//variable you want to assign node value to 
    char* bool_type;
    char* look_up_var;
    Dual *val = new Dual;
    node *bool_tree_root;
    node *bool_tree_leaf;
    Dual *left_bool_value = new Dual;
    Dual *right_bool_value = new Dual;
    int has_child = 0;
    node *arg1; 
    node *arg2;
    vector<Dual> dual_vec;
    map<char*, Dual*, StrCompare> *variables;//all nodes will point to sampe map.

    Dual Add(Dual x,Dual y){
        //Additon rules for 1st and second derivatives
        Dual z; 
        z.value = x.value + y.value; 
        z.der_1 = x.der_1 + y.der_1;
        z.der_2 = x.der_2 + y.der_2;
        return z;
        }

    Dual Sub(Dual x,Dual y){
        //Subtraction rules for first and second derivatives
        Dual z; 
        z.value = x.value - y.value; 
        z.der_1 = x.der_1 - y.der_1;
        z.der_2 = x.der_2 - y.der_2;
        return z;
        }


    Dual Mult(Dual x,Dual y){
        //Product rules for first and second derivatives
        Dual z = *(new Dual); 
        z.value = x.value*y.value; 
        z.der_1 = x.value*y.der_1+x.der_1*y.value;
        z.der_2 = x.value*y.der_2+ 2*x.der_1*y.der_1 + x.der_2*y.value;
        return z;
        }

    Dual Div(Dual x,Dual y){
        //Quotient rules for first and second derivatives
        Dual z; 
        z.value = x.value/y.value; 
        z.der_1 = (x.der_1*y.value - x.value*y.der_1)/(y.value*y.value); 
        z.der_2 = (x.der_2 - 2*z.der_1*y.der_1 - z.value*y.der_2)/y.value;
        return z;
        }    

    void forward(){
        if(node_type == "add"){
            val = new Dual;
		    dual_vec.push_back(Add(*(arg1->val),*(arg2->val)));
		    val = &dual_vec.back();
        }
        else if(node_type == "sub"){
            val = new Dual;
		    dual_vec.push_back(Sub(*(arg1->val),*(arg2->val)));
		    val = &dual_vec.back();
        }
        else if(node_type == "mult"){
            val = new Dual;
		    dual_vec.push_back(Mult(*(arg1->val),*(arg2->val)));
		    val = &dual_vec.back();
        }
        else if(node_type == "div"){
            val = new Dual;
		    dual_vec.push_back(Div(*(arg1->val),*(arg2->val)));
		    val = &dual_vec.back();
        }
        else if(node_type == "assign"){
            val = (*arg1).val;
            (*variables)[assign_to] = (*arg1).val;
            //cout << "ASSIGNIMG" << endl;
            //cout << (*variables)[assign_to] -> value <<endl;


        }
        else if(node_type == "look up"){
            
            //cout<<"here 1"<<endl;
            //cout << (*variables)[look_up_var] -> value;
            val = (*variables)[look_up_var];
            //cout << "GGGGGGGGGGGGGG"<<endl;
            //cout << val -> value;
            //cout<<"here 3"<<endl;
        }
        else if(node_type == "boolG"){
            if((*arg1).val -> value > (*arg2).val -> value){
			val -> value = 1;
		}
            else{
                val -> value = 0;
            }
        }
        else if(node_type == "boolL"){
            if((*arg1).val -> value < (*arg2).val -> value){
			val -> value = 1;
		}
            else{
                val -> value = 0;
            }
        }
        else if(node_type == "boolE"){
            if((*arg1).val -> value == (*arg2).val -> value){
			val -> value = 1;
		}
            else{
                val -> value = 0;
            }
        }
        else if(node_type == "boolGE"){
            if((*arg1).val -> value >= (*arg2).val -> value){
			val -> value = 1;
		}
            else{
                val -> value = 0;
            }
        }
        else if(node_type == "boolLE"){
            if((*arg1).val -> value <= (*arg2).val -> value){
			val -> value = 1;
		}
            else{
                val -> value = 0;
            }
        }
        else if(node_type == "while"){
            while(true){
                bool_tree_root -> forward();
                if ((*bool_tree_leaf).val -> value == 0){
                    break;
                }
                sub_program -> forward();
            }
        }
        else if(node_type == "extrema"){
            double x_val;
            double first_deriv;
            double second_deriv;
            for(int i = 0;i < 1000;i = i + 1){
                arg2 -> forward();
                first_deriv = (*bool_tree_leaf).val -> der_1;
                second_deriv = (*bool_tree_leaf).val -> der_2;
                x_val = (*variables)[(*arg1).look_up_var] -> value;
                (*variables)[(*arg1).look_up_var] -> value = x_val - first_deriv/second_deriv;
            }
            cout<< x_val << endl;
        }
        else if(node_type == "roots"){
            double x_val;
            double first_deriv;
            double func_value;
            for(int i = 0;i < 1000;i = i + 1){
                arg2 -> forward();
                first_deriv = (*bool_tree_leaf).val -> der_1;
                func_value = (*bool_tree_leaf).val -> value;
                x_val = (*variables)[(*arg1).look_up_var] -> value;
                (*variables)[(*arg1).look_up_var] -> value = x_val - func_value/first_deriv;
            }
            cout<< x_val << endl;
        }
        else if(node_type == "print"){
            val = (*arg1).val;
            cout << "Value "<< val -> value <<endl;
            cout << "1st Derivative "<< val -> der_1 <<endl;
            cout << "2nd Derivative "<< val -> der_2 <<endl;
        }
        else{
            //cout<<" "<<endl;
        }
        if(has_child == 1){
            next_node -> forward();
        }
    }

};