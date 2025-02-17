#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

// total
int totalPrimeCount = 0;

//defining mutex variable mu
pthread_mutex_t mu;

struct var{
    FILE *f;
    FILE *fp;
    int length;
    int start;
    int end;
};

//--------------------------------------------------------------------------------------
// this function checks whether the provided number is prime or not by returning 0 and 1
int check(int x) {
    int i,count=1;
    for(i=2;i<=sqrt(x);i++) {
    if(x%i==0) {
    count = 0;
    break;
    }
    } 
	if(x<=1) {
    count=0; 
    } else if(x==2) {
        count=1;
    } 
    if (count == 1) { 
    return 1; 
    } 
    else { 
    return 0;
    } 
}
//-----------------------------------------------

// main function for thread
void *primefunction(void *arg) {
    struct var *val = (struct var*) arg;
    FILE *alldata = val->f;
    FILE *newfile = val->fp;
    int start = val->start;
    int end = val->end;
    int totalLength = val->length;
    int i;
    int *num = malloc(totalLength*sizeof(int));
    pthread_mutex_lock(&mu); //blocks the thread until the mutex is unlocked again
    for(i =start; i<=end;i++) {
        fscanf(alldata,"%d",&num[i]);
        if(check(num[i])==1) //scanning all the numbers whether they are prime or not
        {
            fprintf(newfile, "%d\n", num[i]); // writing the prime numbers into PrimeNumbers.txt
            totalPrimeCount++;
        }
    }
    pthread_mutex_unlock(&mu); //unlocking mutex
    
}
//-----------------------------------------------------------------------
//main function
void main() {
    
    int count = 0,x, threadc;
    
    //file pointers
    FILE *f1, *f2, *f3, *f4, *f5;
    
    // creating a new file which stores the numbers from all three files
    f4 = fopen("mergedfile.txt","w+"); 
    
    //creating new file to store all prime numbers
    f5 = fopen("PrimeNumbers.txt","w");
    
    //------------------------------------------------------------------------
    //reads and determines the number of lines
    f1 = fopen("PrimeData1.txt","r");
    if(f1 == NULL) {
        printf("File not found!!!");
        exit(EXIT_FAILURE);
    }
    while(fscanf(f1,"%d",&x)!=EOF) {
        fprintf(f4, "%d\n",x);
        count++;
	}

    f2 = fopen("PrimeData2.txt","r");
    if(f2 == NULL) {
        printf("File not found!!!");
        exit(EXIT_FAILURE);
    }
    while(fscanf(f2,"%d",&x)!=EOF) {
        fprintf(f4, "%d\n",x);
        count++;
    }

    f3 = fopen("PrimeData3.txt","r");
    if(f3 == NULL){
        printf("File not found!!!");
        exit(EXIT_FAILURE);
    }
    while(fscanf(f3,"%d",&x)!=EOF){
        fprintf(f4, "%d\n",x);
        count++;
    }
    
    rewind(f4); //rewind the merged file
    
    printf("Total lines in all files is: %d",count);
    //----------------------------------------------------------------------------------
    printf("\n\nEnter the number of thread counts : ");
    scanf("%d",&threadc);
    
    //now dynamically allocating the address as per user request
    int *sliceList = malloc(threadc*sizeof(int));
    
    int rem = count%threadc;
    
    //now slicing the threads
    int m,n,o,p,q,i;
    for(i=0;i<threadc;i++){
        sliceList[i] = count / threadc;
    }
    
    // equally distributing rem 
    for(m=0;m<rem;m++){
        sliceList[m] = sliceList[m]+1;
    }
    
    int *startList = malloc(threadc*sizeof(int));
    int *endList= malloc(threadc*sizeof(int));
    
    //------------------------------------------------
    // determining start and end values
    for(n=0;n<threadc;n++){
        if(n==0){
            startList[n]=0;
        }else{
            startList[n]= endList[n-1]+1;
        }
        endList[n]= startList[n]+sliceList[n]-1;
    }

    struct var *mainStruct= malloc(threadc*sizeof(struct var));
    for(o=0;o<threadc;o++) {
        mainStruct[o].f = f4;
        mainStruct[o].length = count;
        mainStruct[o].start = startList[o];
        mainStruct[o].end = endList[o];
        mainStruct[o].fp = f5;
    }
    
    //-----------------------------------------------------------------
    //creating threads and initilizing mutex 
    pthread_mutex_init(&mu,NULL);
    pthread_t *threads = malloc(threadc*sizeof(pthread_t));
    for(p=0;p<threadc;p++){
        pthread_create(&threads[p],NULL,primefunction,&mainStruct[p]);
    }
    
    for(q=0;q<threadc;q++){
        pthread_join(threads[q],NULL); //joins the threads in loop
    }
    
    //destroying mutex after joining threads which will destroy object refrenced by mutex
    pthread_mutex_destroy(&mu);
    
    fprintf(f5,"The total prime numbers are: %d\n",totalPrimeCount); //adds primecount in PrimeNumbers.txt at end of the file
	printf("\nTotal numbers of Prime numbers in 3 files is = %d",totalPrimeCount); //displays total prime numbers in console
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
    fclose(f5);   
}
