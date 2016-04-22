#include <stdio.h>

#define N 3   
#define M 8   

_Bool nondet_bool();
unsigned int nondet_uint();

typedef int bool;
typedef unsigned __CPROVER_bitvector[M] bitvector;  // define a bitvector of length N which will represent the comaprtment

unsigned int nondet(){  
    unsigned int num = nondet_uint();
    __CPROVER_assume( num>= 0 && num  <= 1);
    return num;
};

int  main() {
    
    _Bool C1 , C2;
    bitvector b1 , b2, b3, b4, b5, b6;

    b1 = 0b1111;
    b2 = 0b0011;
    b5 = 0b0001;

    b3 = b1 - b2;
    b4 = b2 - b5;

    assert(0);

    return 0;

}
