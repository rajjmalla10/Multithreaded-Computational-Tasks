#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

//defining float and structure variable composed of long long type variable
//structre for splitting the threads
float Value=0;
struct pi_Value
{
	long long start;
	long long end;
};

//----------------------------------------------------------------------
//decaring new function with void return type
//using general purpose pointer to store address of a structure
//this function is to calculate the value of pi
void *pie(void *ptr) { 
	struct pi_Value *values = (struct pi_Value *)ptr;
	long long start_limit = values->start;
	long long end_limit = values->end;
	long long num;

	for(num=start_limit;num<=end_limit;num++) {
	Value += (((pow(-1,num))*4)/((2*num)+1));
	}
	pthread_exit(0);
	}
//-----------------------------------------------------------------------

//main function with void return type
void main(int argc, char **argv) {
	//declaring variables
	int num;
	long long comp, p;
	int i, j, k, l, m, n;
	//taking input from user
	printf("Enter the number of computation:");
	scanf("%d",&comp);
	printf("Enter the number of thread:");
	scanf("%d",&num);
	
	int remainder= comp%num;
    
    //slicing numbers in equal part
	int sliceList[num];
    for(i=0;i<num;i++) {
	sliceList[i]=comp/num;
	}
    for(j=0;j<remainder;j++){
    	
    // for the remainder
	sliceList[j]=sliceList[j]+1;
	}
    for (i = 0; i < num; i++) {
    printf("%dth thread : %d\n", i+1, sliceList[i]);
	}

    int startList[num];
    int endList[num];
    for(l=0;l<num;l++) {
	if(l==0) {
	startList[l]=0;
    } else {
	startList[l]=endList[l-1]+1;
    }
    endList[l]=startList[l]+sliceList[l]-1;
	}
	 
    //creating pthread type t
	pthread_t threadIds[num];
	//declearing the structure based on num of thread
    struct pi_Value pinum[num];
    for (m= 0; m< num; m++) {
    pinum[m].start=startList[m];
    pinum[m].end=endList[m];
    pthread_create(&threadIds[m],NULL,pie,&pinum[m]);
	}
	     
    //joining threads here
    for(n=0;n<num;n++) {
    pthread_join(threadIds[n],NULL);
    }       
    printf("\n\n  pie:\t%f\n",Value);
 }
