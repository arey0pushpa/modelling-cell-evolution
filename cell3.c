/*  Modeling a Biology Cell evolution using CBMC . Kindly follow the Github Link :
 *
 *  Basic CBMC cell evolution code starting from an initial Given / Arbitary state and 
 *   evolving by making a transition  based on two Tables one update :  which add some material to the current compartmets  
 *   of the cell or adding new compartment (yes possible via {0} :dead: -> {1,2} :cell with a compartment having two materials:)
 *   and a deletion table which removes the materials from the cell compartments or completely delete a compartment (equivalent to a
 *   empty compartment).
 *
 *  
 *   Table Will have a M * M dimention. M is total number of subsets the for given N. here N = 3 i.e total 2 ^ 3 = 8 subsets.
 *   Total number of  Compartments possible are also M. 
 *   Possible values at any place of 2-Dimentional table is either have 0 or 1. 0 means not possible to make a move.1 Means move allowed.
 *
 *   Rules for Update and Deletion Tables del[M}[M] , update[M][M]:
 *   1. For Update Table : 
 *      Exactly One possiblity in a single row to make progress. Progress might be just staying there also. Like {1} -> {1} after
 *      an update state means compartmet remained the same.
 *   2. For Deletion Table :
 *      At least One Possiblity in a single row for the Deletion table. Allowing multiple possible deletes. This might be used to
 *      model the situation based on non-deterministic choice but in order to make the behaviour consistent Only last / most weighted
 *      deletion will be carried out.eg: from {1,2,3} -> Two possiblity Either delete {1} and hence new compartment will have {2,3} 
 *      remaining or delete {2} and hence remains to {1,3} In this case {2} will be taken as the choice. This decision is Temporary and 
 *      will be changed to Non-deterministic in future.
 *
 *  Encoding used :
 *    I used encoding as kind'Of Go-to table that means if * Lets row 3 (representing {1,2}) and column 2 (representing {2}) have a value
 *    " 1 " i.e delete[i][j] == 1 ->  means we have from a comprtment with {1,2} deleted {1} and now Left with a compartment with a only
 *    one element/material {2}. Which is kind of abstract meaning rather than a explicit form. 
 *   Intuitively you might have want delete[3][2] == 1 means from {1,2} and {2} you delete {2} and somehow figure out (via
 *   some calculation  here trivial ) to delete {2} from this compartment and transform this compartment to a single element
 *   compartment {1}. 
 *
 *   States representation :
 *   States are represented using a Bitvector of length M. That means lets say N = 2 which have total possible 2 ^ 2 = 4 those are 
 *   {0} , {1} , {2} , {1,2} . state : 0100  (Please Read from left side i.e from MSB ) represent in  meaning only one 
 *   comprtment present i.e with element / material 1 ({1)}.Similarly 0111 means three compartment currently present 
 *   which are {1} , {2} and a {1,2} these are represented just by position (MSB rather than LSB yes It's unituitive but will allow 
 *   us to use table representation as a bit vector as not possible in any form to tell computer what {1,2} really mean). 
 *   first zero represnt compartment with no element present ? {0} not mean much. First  1 represent a compartment with single
 *   elemt 1 ({1}) .etc last one i.e LSB 1 rightmost one represent presence of a compartment with two elements {1,2}.
 *
 *   State will make transition based on the transition function which will take help from delete and update to make a transition 
 *   to next state.
 *
 *   As It is possible to have a multiple one's in a row in Deletion table we have to use some means to implement our most weighted 
 *   delete a choice ,here used that facility as getlastone() function. Asthe state representation is reverse i.e from MSB to LSB
 *   getlastone will mean implement most weighted delete. Which make the encoding consitent, for the time being.
 *
 *
 */

#include <stdio.h>

typedef int bool;
#define true  1
#define false 0

#define M 8
#define N 2

//Define type for the bitvector : uniformly used 
typedef unsigned __CPROVER_bitvector[8] bitvector; 

_Bool nondet_bool();
bitvector updatetable[M], deltable[M];
 
// Get reprenstation will fetch the current 2-D table in a Single dimentional bitvector.
bitvector getrepresent(int num , int i , _Bool del[M][M] ,_Bool update[M][M]){
	   bitvector b1 , b2 , k = 0;
		if (num == 1) {
          for(int j=0; j<M ; j++){
                if (update[i][j] != 0){
						k = k | 0b00000001; 
				 }
				 else if(update[i][j] == 0){
					 k = k | 0b00000000;
				 }
				
				if(j != M-1) {
                   k = k << 1; 
	              }    
		}							
		   return  k;
		}
		
		else if(num == 2) {
			for(int j=0; j<M ; j++){
                if (del[i][j] != 0){
						k = k | 0b00000001; 
				 }
				 else if(del[i][j] == 0){
					 k = k | 0b00000000;
				 }
				
				if(j != M-1) {
                   k = k << 1; 
	              }    
     		}							
		   return  k;
	   }
}


//Get Last one representation ensuring heviest delete choice.
bitvector getlastone(bitvector l){
   bitvector k = 0b00000001;
   for(int i=0;i<M;i++){
     if (l & k == k){ 
       return k ;
   }
   else {
	    k = k << 1;
    }
  }
}

/* =================================TRANSITION FUNCTION =============================================================*/
/*   Based on the table used / chossen by the SAT solver. Used the converted one dimentional Bitvector table. 
 *   Will consist of One Block for the Update transition and one for the delete transition.      */

bitvector transition(bitvector state) {
      bitvector dummyState = 0 , b , last;
      int ithofState;    
    //Update Block  
      for(int i = 0;i < M; i++){
         ithofState =  (state >> i) & 1;
         if(ithofState == 1) { 
                 b = updatetable[7- i]; 
				 dummyState = dummyState | b;
                }
         }
         
      state = dummyState;      
      dummyState = 0;
   
   //Delete Block 
   for(i = 0 ; i < M ; i++){
         ithofState = (state >> i) & 1;
         if(ithofState == 1) {
                b = deltable[7 - i];
                last = getlastone(b);
                dummyState = dummyState | last;      
         }
      }
      
      state = dummyState;
      return state;
}


void main() {
	
   bitvector state , l2s_s, k , next_state ,updatevar , delvar ;
   _Bool save, saved, on_loop, looped;
   unsigned int  i , j , counter = 0;
   _Bool del[M][M];  
   _Bool update[M][M];
      
  //Avoidance of bad set happening : Future use :  
   
   bitvector stored[M]  = {0b1111, 0b1110, 0b1101, 0b1011, 0b1100 , 0b1010, 0b1001 ,0b0000};

 // Specify the constrints At least one in a row . 
   for(i=0;i<M;i++){
         __CPROVER_assume( (del[i][0]) || (del[i][1]) || (del[i][2]) || (del[i][3]) || (del[i][4]) || 
                   (del[i][5]) ||  (del[i][6]) || (del[i][7]) ) ;
	 }
  
// Exactly one in a row is true for update.
   for(i=0;i<M;i++){
	   __CPROVER_assume( (update[i][0]) ?  ( !(update[i][1]) && !(update[i][2])  && !(update[i][3]) && (update[i][4]) && !(update[i][5])  && !(update[i][6]) && !(update[i][7]) )  :
	           ((update[i][1]) ?  (!(update[i][2])  && !(update[i][3]) && !(update[i][4]) && !(update[i][5])  && !(update[i][6]) && !(update[i][7]) ) :
	             ((update[i][2]) ? ((!update[i][3]) && !(update[i][4]) && !(update[i][5])  && !(update[i][6]) && !(update[i][7]))  :
	                   ((update[i][3]) ? (!(update[i][4]) && !(update[i][5])  && !(update[i][6]) && !(update[i][7]))  : 
	                   ((update[i][4]) ? (!(update[i][5])  && !(update[i][6]) && !(update[i][7])) : 
	                      ((update[i][5]) ? (!(update[i][6]) && !(update[i][7])) :  
	                         ((update[i][6]) ? !(update[i][7]) : (update[i][7])))))))) ;
   }
  
/*       Condition for the table to follow as bad cases like {1} -> {2} In encoding means
 *       from a comprtment with {1} go to a compartment with {2} which is not a valid charectersitic
 *       both for delete and for the update.
 *       Yes Its a future use of the wandering bitvector array above.                          */      

  for(i=0;i<M;i++){
	  for(j=0;j<M;j++){
	   	 __CPROVER_assume( (del[i][j]) ? ( (i>=j) && ((stored[i] & stored[j]) == (stored[i])) ) : 1);  
	   	 __CPROVER_assume( ( update[i][j])?((i<=j) && ((stored[i] & stored[j]) == (stored[j])) ) : 1);
	  }
   }
  
  /*         Change a 2-D table to a 1D bitvector array not required and will go in next update but 
                        currently not toucjing it as evrythings is working      */
  for(i=0;i<M;i++){
	   for(j=0;j<M;j++){
		     updatevar = getrepresent(1,i ,del , update);
		     delvar = getrepresent(2,i , del , update);		     
		     updatetable[i] = updatevar;
		     deltable[i] = delvar;
		 }
	 }
  			
  //state = 0b10111000;   //0111 Lets Leave It to be arbitary yes more work for Sat to come up with a table to generate a 
  //counterexaple

   saved = 0; looped = 0; 						 									
   unsigned int savecnt = 0;
  /*================================================== INFINITE BLOCK ===============================================*/
  /* Contains inside the whole logic of the program  : 
   +++++++++++++++++ Subpart of block contains code for checking for whether we Looped or not +++++++++++++++++++ */


while(1){					   		 
    save = nondet_bool(); 			 
    printf(" \nthe value of state is : %d \n" , state); 
    
    if (save && (!saved)) {
        l2s_s = state ;           
    }	
     on_loop = (save || saved);			
     next_state = transition(state);
  
     counter++;
   
     state = next_state;
     saved = on_loop;
     
     looped = (saved && (state == l2s_s));
     if (on_loop && !looped) savecnt = counter;
     
__CPROVER_assert( (!(on_loop) ||  (!looped || savecnt < 9)), "every stable state is reached within 9  iterations");
  } 
}


