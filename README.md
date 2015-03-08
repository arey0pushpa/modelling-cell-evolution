# modelling-cell-evolution
CBMC based modelling of cell evolution

Modeling a Biology Cell evolution using CBMC . Kindly follow the Github Link :
https://github.com/arey0pushpa/modelling-cell-evolution
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
 *

