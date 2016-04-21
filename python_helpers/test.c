#include<stdio.h>

#define N 3
int main() {
    unsigned int i;
    int *myArray[N];
    for (i = 0; i < N; i++){
        myArray[i] = (int*) malloc((i + 1) * 3 * sizeof(int));
    }
   
  int a1[] = {0};
  int a2[] = {1};
  int a3[] = {0,1};

        myArray[0] = a1;
        myArray[1] = a2;
        myArray[2] = a3;
   
   for (i = 0; i < 3; i++) {
           for(int j = 0; j < 3; j++) {
                       printf("%d ", array[i][j]);
                           }
               printf("\n");
   } (
  // assert(0);
return 1;
}
