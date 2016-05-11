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


_Bool nondet_bool();
unsigned int nondet_uint();

typedef int bool;
typedef unsigned __CPROVER_bitvector[M] bitvector;  // define a bitvector of length N which will represent the comaprtment

unsigned int nondet(){  
    unsigned int num = nondet_uint();
    __CPROVER_assume( num >= 0 && num  <= 1);
    return num;
};

// Go from one transition state to updated tarnsition state using this function
bitvector transition(bitvector state, bitvector getRel[] , bitvector getAbs[]){    
    int ithofState , incState;  
    unsigned int  i , j;
    bitvector newState = 0b0 , retState;
    for ( i = 0 ; i < M; i++) {
        // Checking for present and then going to new state update abs
        // 0100 that means there is a compartment {1} present rather than starting from LSB
        if ((state >> i) & 1) {     // MOsT IMPORTANT POINT State has been represented as reverse of view. 
           newState = (newState | getAbs[i]) ;
        }

        if (i == (M - 1)){
            if (state & ( 1 << i)) {
                newState = (newState |  getAbs[i]) ;
                }
            if (newState & (1 >> M - 1)) {
				newState = newState & ~(1 >> M-1); 
                newState = newState | getRel[M - 1];
                }
            if (newState & (1 >> M - 2)) {
				newState = newState & ~(1 >> M-2);
                newState = newState | getRel[M - 2];
             }
             if(newState & ( 1 >> M- 3)) {
				newState = newState & ~(1 >> M-3);
                newState = newState | getRel[M - 3];
             }
        }
     }
     return newState;
}       


int  main() {
    
    _Bool save, saved, on_loop, looped;
    _Bool C1 , C2;

    bitvector state, l2s_s, next_state, updatevar, delvar ;
    unsigned int i , j , k , counter;
    unsigned int relCount = 0, absCount = 0;
   // Each table Will have a M * M dimention. M is total number of subsets the for given N. If N = 2 i.e total 2 ^ 2 = 4 subsets.
    bitvector rel[M][M], getRel[M];  // define release table M * M
    bitvector abs[M][M], getAbs[M];  // define Absorb table M * M  

   // Possible values at any place of 2-Dimentional table is either have 0 or 1. 
   // 0 means not possible to make a move. 1 Means move allowed.

    int* array1[M];
    
    bitvector s[M] = {0b0000 ,0b0001 , 0b0010 , 0b0100 , 0b0011, 0b0101 , 0b0110 , 0b1110};
   // bitvector n[M] = {0b0000 ,0b0001 , 0b0010 , 0b0100 , 0b0011, 0b0101 , 0b0110 , 0b1110};
   
    C1 = 0;
    C2 = 1;
    
    // CONFUSION POINT HAS TO BE CLEARED ! 
    //  Allow only those places that have subset to be released
    // Make absorb Absorb Correctly and Release to release correctly
    
    /* Valid Point 
     * Not All possible combinations are useful only 2 ^ N combination are 
     * usefull.
     * Sugested Solution Represent the all possible as a index and check 
     * for the condition to be satisfied . 
    */

 /* CONSTRAINTS ON RELEASE AND ABSORB TABLE 
  *
  * ON ABSORB TABLE 
    At most two release and at least one absorb ans at most two absorb
    Exactly One possiblity in a single row to make progress.

 * ON RELEASE tABLE 
 *  At least One Possiblity in a single row for the Deletion table.
 *      Allowing multiple possible deletes. 
    
 *   This might be used to model the situation based on non-deterministic choice.
 *   But in order to make this model deterministic only last choice will be taken into account.
 
 */

   for (i = 0; i < M; i++) {
       for (j = 0; j < M; j++) {
          if (i <= j) {
             abs[i][j] = 0;           // Make all Places to be 0 as u can't update and loose molecule. 
             if ( (s[i] & s[j]) == s[j]) {
                rel[i][j] = nondet();  // Allow arbitary 1's at any possible positions
             }
             else {
               rel[i][j] = 0;
             
             }  
          }

          else {
              rel[i][j] = 0;
              if( (s[i] & s[j]) == s[i]) {
                  abs[i][j] = nondet();
              }
              else {
                  abs[i][j] = 0;
              }
          }   

        relCount += (rel[i][j] ? 1 : 0);
        absCount += (abs[i][j] ? 1 : 0);
       assert(0);
       }
// Just to make model simple for today only  allowing only one update and delete config
       __CPROVER_assume(absCount == 1);
       __CPROVER_assume(relCount == 1);
   }
 
 
 // Create the bitvector versio of the 2d table we have 
  for  (i = 0; i < M; i++){
      getRel[i] = 0b0;
      getAbs[i] = 0b0;
      for(j = 0;j < M; j++){
          if(rel[i][j] == 1){
           getRel[i] =  (getRel[i] | (1 << i));
          }
          if(abs[i][j] == 1){
            getAbs[i] = (getAbs[i] | (1 << j)) ;
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
     next_state = transition(state , getRel , getAbs);
  
     counter++;
   
     state = next_state;
     saved = on_loop;
     
     looped = (saved && (state == l2s_s));
     if (on_loop && !looped) savecnt = counter;  
     __CPROVER_assert( (!(on_loop) ||  (!looped || savecnt < 10)), "every stable state is reached within 10 iterations");
  }
assert(0);
  return 0;
}

