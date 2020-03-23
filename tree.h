#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <math.h>
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
    char* func_arg;
    char* func_name;
    Dual *val = new Dual;
    node *bool_tree_root;
    node *bool_tree_leaf;
    Dual *left_bool_value = new Dual;
    Dual *right_bool_value = new Dual;
    int has_child = 0;
    node *arg1; 
    node *arg2;
    node *arg3;
    node *arg4;
    node *arg5;
    vector<Dual> dual_vec;
    map<char*, Dual*, StrCompare> *variables;//all nodes will point to sampe map.
    map<char*,node*,StrCompare> *functions;

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
        else if(node_type == "if"){
            bool_tree_root -> forward();
            if((*bool_tree_leaf).val -> value == 1){
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
        else if(node_type == "extrema2"){
            double x_val;
            double first_deriv;
            double second_deriv;
            double high = (*arg3).val -> value;
            double low = (*arg4).val -> value;
            double points = (*arg5).val -> value;
            double increment = (high - low)/points;
            for (double j = low; j<high; j = j + increment){
                (*variables)[(*arg1).look_up_var] -> value = j;
                for(int i = 0;i < 1000;i = i + 1){
                    arg2 -> forward();
                    first_deriv = (*bool_tree_leaf).val -> der_1;
                    second_deriv = (*bool_tree_leaf).val -> der_2;
                    x_val = (*variables)[(*arg1).look_up_var] -> value;
                    (*variables)[(*arg1).look_up_var] -> value = x_val - first_deriv/second_deriv;
                }
                cout<< x_val << endl;
            }
        }
        else if(node_type == "roots2"){
            double x_val;
            double first_deriv;
            double func_value;
            double high = (*arg3).val -> value;
            double low = (*arg4).val -> value;
            double points = (*arg5).val -> value;
            double increment = (high - low)/points;
            for (double j = low; j<high; j = j + increment){
                (*variables)[(*arg1).look_up_var] -> value = j;
                for(int i = 0;i < 1000;i = i + 1){
                    arg2 -> forward();
                    first_deriv = (*bool_tree_leaf).val -> der_1;
                    func_value = (*bool_tree_leaf).val -> value;
                    x_val = (*variables)[(*arg1).look_up_var] -> value;
                    (*variables)[(*arg1).look_up_var] -> value = x_val - func_value/first_deriv;
                }
                cout<< x_val << endl;
            }
        }
        else if (node_type == "exp"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = exp(func_val);
            double new_der_1 = exp(func_val)*first_deriv;
            double new_der_2 = secon_deriv*new_val + first_deriv*new_der_1;

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "log"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = log(func_val);
            double new_der_1 = (1/func_val)*first_deriv;
            double new_der_2 = secon_deriv*(1/func_val) - first_deriv/(func_val*func_val);

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "sin"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = sin(func_val);
            double new_der_1 = cos(func_val)*first_deriv;
            double new_der_2 = secon_deriv*cos(func_val) - first_deriv*new_val;

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "asin"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = asin(func_val);
            double new_der_1 = (1/sqrt(1-func_val*func_val))*first_deriv;
            double new_der_2 = secon_deriv*(1/sqrt(1-func_val*func_val)) + first_deriv*func_val/(pow(1-func_val*func_val,1.5));

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "cos"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = cos(func_val);
            double new_der_1 = (-sin(func_val))*first_deriv;
            double new_der_2 = -secon_deriv*(sin(func_val)) - first_deriv*cos(func_val);

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "acos"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = acos(func_val);
            double new_der_1 = -1/(sqrt(1-func_val*func_val))*first_deriv;
            double new_der_2 = -secon_deriv*1/(sqrt(1-func_val*func_val)) - first_deriv*func_val/pow(1-func_val*func_val,1.5);

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "tan"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = tan(func_val);
            double new_der_1 = (1/(cos(func_val)*cos(func_val)))*first_deriv;
            double new_der_2 = secon_deriv*(1/(cos(func_val)*cos(func_val))) + first_deriv*(new_val/(cos(func_val)*cos(func_val))); 

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "atan"){
            double func_val = (*arg1).val -> value;
            double first_deriv = (*arg1).val -> der_1;
            double secon_deriv = (*arg1).val -> der_2;
            
            double new_val = atan(func_val);
            double new_der_1 = (1/(1+func_val*func_val))*first_deriv;
            double new_der_2 = secon_deriv*(1/(1+func_val*func_val)) - 2*first_deriv*func_val/(pow(func_val*func_val + 1,2));

            val = new Dual;
            val -> value = new_val;
            val -> der_1 = new_der_1;
            val -> der_2 = new_der_2;
        }
        else if (node_type == "pow"){
            double g = (*arg1).val -> value;
            double gp = (*arg1).val -> der_1;
            double gpp = (*arg1).val -> der_2;

            double h = (*arg2).val -> value;
            double hp = (*arg2).val -> der_1;
            double hpp = (*arg2).val -> der_2;
            
            double f = pow(g,h);
            double fp = f*(h*gp/g + log(g)*hp);
            double fpp = fp*fp/f + f*(g*(2*gp*hp + g*log(g)*hpp) + h*(g*gpp - gp*gp))/(g*g); 
            val = new Dual;
            val -> value = f;
            val -> der_1 = fp;
            val -> der_2 = fpp;
        }
        else if (node_type == "raise"){
            double g = (*arg1).val -> value;
            double gp = (*arg1).val -> der_1;
            double gpp = (*arg1).val -> der_2;

            double h = (*arg2).val -> value;

            double f = pow(g,h);
            double fp;
            double fpp;
            if(h==0){
                fp = 0;
                fpp = 0;
            }
            else if(h==1){
                fp = gp;
                fpp = gpp;
            }
            else if(h==2){
                fp = 2*g*gp;
                fpp = 2*(gp*gp + g*gpp);
            } 
            else{
                fp = h*pow(g,h-1)*gp;
                fpp = h*((h-1)*pow(g,h-2)*gp + pow(g,h-1)*gpp);
            }
            val = new Dual;
            val -> value = f;
            val -> der_1 = fp;
            val -> der_2 = fpp;
        }
        else if(node_type == "func_call"){

            node *func_node = (*functions)[func_name];
            (*variables)[(*func_node).func_arg] = (*arg1).val;
            (*func_node).forward();
            val = (*variables)[(*func_node).func_arg];
        }
        else if(node_type == "print"){
            val = (*arg1).val;
            cout << "Value "<< val -> value <<endl;
            cout << "1st Derivative "<< val -> der_1 <<endl;
            cout << "2nd Derivative "<< val -> der_2 <<endl;
            cout <<" "<<endl;
        }
        else{
            //cout<<" "<<endl;
        }
        if(has_child == 1){
            next_node -> forward();
        }
    }

};