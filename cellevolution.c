#include <stdio.h>

/* N : Total Number of different Molecules.
 *
 * M : Total number of different combinations of molecule. i.e Powerset of N.
 *
 * 1. We consider distinct combination as differnt compartment.
 *    
 * // A BIG Assumption : Subtle Point :    
 * 2. Doesn't care if two/more copies of the same compartment present.
 *    Will consider It as only One Copy. i.e we are not calculating the total number of Similar compartments present.
 *    We are taking account only total number of differnt compartments present.*/

#define N 3   // Total Number of molecules
// State is going to be represented by M 
#define M 8   // Total Number of different combinations i.e  
#define true 1
#define false 0

typedef int bool;
typedef unsigned __CPROVER_bitvector[M] bitvector;  // define a bitvector of length N which will represent the comaprtment

// Go from one transition state to updated tarnsition state using this function
bitvector transition(bitvector state, bitvector rel , bitvector abs , bitvector getRel , bitvector getAbs ){    
    int ithofState , incState;  
    for  (int i = 0; i < P; i++){
         ithofState =  (state >> i) & 1;
         if( ithofState == 1) {
             incState = i - getRel[i];
             state = state & 1 << i ;
             state = state & 1 << incState ;
         }
     }
}


int  main() {

    bitvector state, l2s_s, next_state, updatevar, delvar ;
    _Bool save, saved, on_loop, looped;
    _Bool C1 , C2;
    unsigned int i , j , k , counter;
   // Each table Will have a M * M dimention. M is total number of subsets the for given N. If N = 2 i.e total 2 ^ 2 = 4 subsets.
    bitvector rel[M][M], getRel[M];  // define release table M * M
    bitvector abs[M][M], getAbs[M];  // define Absorb table M * M  

   // Possible values at any place of 2-Dimentional table is either have 0 or 1. 
   // 0 means not possible to make a move. 1 Means move allowed.

    int* array1[P];
    
    bitvector subset[P] = {0b 
    
    C1 = 0;
    C2 = 1;
    

    // CONFUSION POINT HAS TO BE CLEARED ! 
    //  Allow only those places that have subset to be released
    for(i=0;i<P;i++){
        for(j=0;j<P;j++){
            if(i<j && (i&j != 0 )){
            __CPROVER_assume(rel[i][j]);
            }
        }
    }  
    
 // Achive at most two release and at least one absorb
 
   for (i = 0; i < P; i++) {
     for (j = 0; j < P; j++) {
        C1 += (rel[i][j] ? 1 : 0);
        C2 += (abs[i][j] ? 1 : 0);
     }

     __CPROVER_assume(C1 <= 2 && C1 >=1);
     __CPROVER_assume(C2 >= 1); 
   }
 


 //Allow only subset release
  

  for(i=0;i<P;i++){
      getRel[i] = 0;
      getAbs[i] = 0;
      for(j=0;j<P;j++){
          if(rel[i][j] == 1){
           getRel[i] =  getRel[i] || j;
          }
          if(abs[i][j] == 1){
            getAbs[i] = getAbs[i] || j ;
          }
      }
  }

 // state = 0b10111000;   //0111
  saved = 0;
  looped = 0; 						 									
  unsigned int savecnt = 0;
   
while(1){	
					   		 
    save = nondet_bool(); 			 
    printf(" \nthe value of state is : %d \n" , state); 
    
    if (save && (!saved)) {
        l2s_s = state ;           
    }	
     on_loop = (save || saved);			
     next_state = transition(state , rel , abs , getRel , getAbs);
  
     counter++;
   
     state = next_state;
     saved = on_loop;
     
     looped = (saved && (state == l2s_s));
     if (on_loop && !looped) savecnt = counter;  
     __CPROVER_assert( (!(on_loop) ||  (!looped || savecnt < 10)), "every stable state is reached within 10 iterations");
  }

  return 0;
}

