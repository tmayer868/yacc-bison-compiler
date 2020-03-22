#include<iostream>
#include<vector>
#include "tree.h"

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
 Dual z; 
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

