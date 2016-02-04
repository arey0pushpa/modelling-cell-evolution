# modelling-cell-evolution
CBMC based modelling of cell evolution

Modeling a Biology Cell evolution using CBMC . Kindly follow the Github Link :
https://github.com/arey0pushpa/modelling-cell-evolution
 
![](http://lowres.jantoo.com/science-evolves-evolution_of_man-primate-cartoon-evolution-36532397_low.jpg)
 
 N : Total Number of different Molecules.
 
 M : Total number of different combinations of molecule. i.e Powerset of N. 
 
        == Total Number of compartments Possible.
 
        If 
           1. We consider distinct combination as differnt compartment.
 
               // A BIG Assumption : Subtle Point :    
           2. Doesn't care if two/more copies of the same compartment present.
              Will consider It as only One Copy.
              
 ** A Cell State : Represented by how many compartments are present now and there complete configuration. 

***************************************************************************                  
                         Basic CBMC cell evolution Code :
****************************************************************************

============================================
 1. BASIC REPRESENTATION :
============================================
 Lets Fix N = 2. Total number of molecules are two. And Lets represent them as number 1,2.

 Total number of possible combination = Power set of N  
              
              := 2 ^ 2  = 4 will be repesented as 

         {0} : Represents Nothing is present. Comapartment is empty.     // Representation assumption.
         {1} : Represents A compartment with only one molecule 1 is present.
         {2} : Represents A cmpartment with only one molecule 2 is present. 
         {1,2} : Represents a compatment with both the molecules are presnet.

=============================================================
2. TRANSITION :
================================================================

Starting from an initial Arbitary state and evolving (making progress from inital state) by using a transition function based on two Tables 

     1. Update Table :  Which  a# Add some material to the current compartments  
                        of the cell or b# Adding new compartment

         a# Prev State -> {{1} , {2}}  Next State ->  {{1,2}, {2}}  
               Added molecule {2} to first compartment.    

         b# Prev State -> {0} :dead:   Next State -> {1,2} :
               Cell with a compartment having two materials molecule 1 and 2.

     2. Deletion Table : Which removes the materials from the cell compartments or completely delete a it.
                          (equivalent to a empty compartment).

 
 Another Subtle Point 
 
 Adding same molecule to the current compartment where already that molecule is present would have no change in configuration of the molecule. For example if we add molecule 1 to a compatment with configuration {1,2} its still be {1,2}. That means we are not taking care of count of #Same Molecules present. 



=============================================================
2. TABLE REPRESENTATION :
================================================================
 
*   Each table Will have a M * M dimention. M is total number of subsets the for given N. 
     If N = 2 i.e total 2 ^ 2 = 4 subsets.
     
 *   Possible values at any place of 2-Dimentional table is either have 0 or 1. 
               0 means not possible to make a move. 1 Means move allowed.

-------------------------------------------------------------------------------------
 Rules for Update and Deletion Tables del[M}[M] , update[M][M]:
 
     1. For Update Table : 
     
         Exactly One possiblity in a single row to make progress.
         Basic Example : Update caused a compartment {1} to add molecule 2 .
                         Hence become compartment {1,2} in next state. 
       
         Progress might be just staying there also.
           Like {1} -> {1} after an update state means compartmet remained the same.
  
     2. For Deletion Table :

        At least One Possiblity in a single row for the Deletion table.
        Allowing multiple possible deletes. 
        
        This might be used to model the situation based on non-deterministic choice.


      One possible Update Table :   
             {0}   {1}   {2}  {1,2}
       {0}    0     1     0     0
       {1}    0     0     0     1
       {2}    0     0     1     0
       {1,2}  0     0     0     1
       

update[1][3] = 1 : Means second row and fouth column = 1 and hence a valid move.
  
     As the rules are deterministic and there is only one choice if a compartment has
     this configuration {1} in next state molecule 2 will be added to this compartment 
     and hence it'll become {1,2}. 
  
update[2][2] = 1 : Means third row and third column has 1.
     
     Means in an update phase a molecule with configuration {2} will remains {2}.
     Nothing is going to be added. Stay on.
   
      One possible Delete Table :   
             {0}   {1}   {2}  {1,2}
       {0}    1     0     0     0
       {1}    1     1     0     0
       {2}    1     0     0     0 
       {1,2}  1     0     1     0
 
 delete[0][0] = 1 : If molecule is dead nothing can be deleted from it,
 
 delete[3][0] = 1 , delete[3][2] = 1 : Means if compartment has this configuration {1,2} in deletion phase there will be a non-deterministic choice between these two options. 
 
 If choosen :
 
 delete[3][0] will give an effect (means) if compartment has configuration {1,2} before a delete phase in deletion phase this compartment will loose both elements and will become dead. i.e {0}
 
 Similarly if choosen :
 
 delete[3][2] = 1 : ill give an effect (means) if compartment has configuration {1,2} before a delete phase in deletion phase this compartment will loose one element {1} and will become a compartment with only one molecule 2 ,  {2}.

--------------------------------------------------------- -------------------------
 Another assumption :
 
   ** But in order to make the behaviour consistent Only last choice will be taken.
        
     Deletion will be carried out.eg: from {1,2} -> Two possiblity Either
 
       a. delete {1} and hence new compartment will have {2} remaining. 
 
       b. delete Both molecule and hence cell becomes dead to {0}. 
 
     In this case b will be taken as the choice. 
     This decision is Temporary and will be changed to Non-deterministic in future versions.
 -----------------------------------------------------------------------------------

======================================================
2.b Encoding used :
======================================================

  I used encoding as kind'Of Go-to table that means if * Lets row 3 (representing {1,2}) and column 2 (representing {2}) have a value   " 1 " i.e delete[i][j] == 1 ->  means we have from a comprtment with {1,2} deleted {1} and now Left with a compartment with a only one element/material {2}. Which is kind of abstract meaning rather than a explicit form. 
 
  Intuitively you might have encode this as delete[3][2] == 1 means from {1,2} and {2} you delete {2} and somehow figure out (via some calculation  here trivial ) to delete {2} from this compartment and transform this compartment to a single element      compartment {1}. 
 
 *   States representation :
   
   States are represented using a Bitvector of length M. That means lets say N = 2 which have total possible 2 ^ 2 = 4 those are  {0} , {1} , {2} , {1,2} . state : 0100  (Please Read from left side i.e from MSB ) represent in  meaning only one  comprtment present i.e with element / material 1 ({1)}. 

   Similarly 0111 means three compartment currently present  which are {1} , {2} and a {1,2} these are represented just by position (MSB rather than LSB yes It's unituitive but will allow  us to use table representation as a bit vector as not possible in any form to tell computer what {1,2} really mean). 

 First zero represnt compartment with no element present ? {0} not mean much. First  1 represent a compartment with single
  elemt 1 ({1}) .etc last one i.e LSB 1 rightmost one represent presence of a compartment with two elements {1,2}.
  
 *   State will make transition based on the transition function which will take help from delete and update to make a transition  to next state. As It is possible to have a multiple one's in a row in Deletion table we have to use some means to implement our most weighted delete a choice ,here used that facility as getlastone() function. Asthe state representation is reverse i.e from MSB to LSB getlastone will mean implement most weighted delete. Which make the encoding consitent, for the time being.
 

Found a bug?
------------
Just Take authority and change. 

Contributing
------------

We welcome pull requests from Human , Aliens , Advanced Computers , Dumb circuits, Toys , Animals , Virus , Bacterias so feel ashamed for any contribution. 


### Usage

You can create an instance of ParseServer, and mount it on a new or existing Express website:


```js

// For Ubuntu / Debians diretly just 
# apt-get install cbmc

// Others Follow Cprover Link

http://www.cprover.org/cbmc/

# In Order To Run :

# cbmc filename.c

// Provide This in case to achieve enough unwindings
# cbmc filename.c --no-unwinding-assertions
    

```

License
-------

The LSD Clause

Copyright (c) 2016, @GogoHaskell)+

All rights UN-reserved.

1. Redistributions of source code may be injurious to health.

2. Redistributions in binary form may expose you to radioactive radiations. 

THIS SOFTWARE IS FREE OF COST LIABILITY TO WHOLE UNIVERSE. THE ONLY POSSIBLE WAY TO CONTIBUTE TO IMPRVE THE CURRENT SITUATION IS TO TAKE RESPONSIBILTY OF ABOVE AND CHANGE IT AS MUCH IS POSSIBLE. 

