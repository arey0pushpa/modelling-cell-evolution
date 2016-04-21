#include <stdio.h>

typedef unsigned __CPROVER_bitvector[32] bitvector;

typedef int bool;

#define true 1
#define false 0

//  Define the No of scalable Units 2 ^ N 

#define P 32

// Go from one transition state to updated tarnsition state using this function
bitvector transition(bitvector state, bitvector rel , bitvector abs , bitvector getRel , bitvector getAbs ){
    
    int ithofState , incState; 
    
    for(int i=0;i<P;i++){
        ithofState =  (state >> i) & 1;
        if(ithofState == 1) {
            incState = i - getRel[i];
            state = state & 1 << i ;
            state = state & 1 << incState ;
}


int  main() {

    bitvector state, l2s_s, next_state, updatevar, delvar ;
    _Bool save, saved, on_loop, looped;
    _Bool C1 , C2;
    unsigned int i , j , k , counter; 
    bitvector rel[P][P], getRel[P];
    bitvector abs[P][P], getAbs[P];
  
    C1 = 0;
    C2 = 1;
    
    //  Allow only those places that have subset to be released
    for(i=0;i<P;i++){
        for(j=0;j<P;j++){
            if(i<j && (i&j != 0 )){
            __CPROVER_assume(rel[i][j]);
            }
        }
    }  
    
    //Achive at most two release and at least one absorb
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

  return 1;
}

