#include<stdio.h>
#include<stdlib.h>

void main(){
	
	//initalizing file pointers and variables
	FILE *fptr1, *fptr2, *fptr3, *fptr4;
	float x, y, xsum, ysum, xx_sum, yy_sum, xy_sum, a, b, x_value, final;
	int n = 0;
	
//==============================================================================
	
	/*Exception handeling by reading files, if not found 
	  prints the message and exit the program
	*/
	fptr1 = fopen("datasetLR1.txt","r");  
	if(fptr1 == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
	fptr2 = fopen("datasetLR2.txt","r");
	if(fptr2 == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
	fptr3 = fopen("datasetLR3.txt","r");
	if(fptr3 == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
	fptr4 = fopen("datasetLR4.txt","r");
	if(fptr4 == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
//------------------------------------------------------------------------------------------
	
	// calcuating the necessary values to find a & b from the all the files ,  coefficients of the regression line equation
	while(fscanf(fptr1,"%f,%f",&x,&y) != EOF){
		xsum = xsum + x;
		ysum = ysum + y;
		xy_sum = xy_sum + (x * y);
		xx_sum = xx_sum+ (x * x);
		yy_sum = yy_sum + (y * y);
		n += 1;
	}
	
	while(fscanf(fptr2,"%f,%f",&x,&y) != EOF){
		xsum = xsum + x;
		ysum = ysum + y;
		xy_sum = xy_sum + (x * y);
		xx_sum = xx_sum + (x * x);
		yy_sum = yy_sum + (y * y);
		n += 1;
	}
	
	while(fscanf(fptr3,"%f,%f",&x,&y) != EOF){
		xsum= xsum+ x;
		ysum= ysum+ y;
		xy_sum= xy_sum+ (x * y);
		xx_sum= xx_sum+ (x * x);
		yy_sum= yy_sum+ (y * y);
		n += 1;
	}
	
	while(fscanf(fptr4,"%f,%f",&x,&y) != EOF){
		xsum= xsum+ x; 
		ysum= ysum+ y;
		xy_sum= xy_sum+ (x * y);
		xx_sum= xx_sum+ (x * x);
		yy_sum= yy_sum+ (y * y);
		n += 1;
	}
	
	//closing file pointer as it has no use ahead
	fclose(fptr1);
	fclose(fptr2);
	fclose(fptr3);  
	fclose(fptr4);
	
	
	// formula to calculate values of a & b 
	a = ((ysum* xx_sum)-(xsum* xy_sum))/((n * xx_sum)-((xsum) * (xsum)));
	b = ((n * xy_sum)-((xsum) * (ysum)))/((n * xx_sum)-((xsum) * (xsum)));
	
	printf("The values of a & b are: \nA = %f and B = %f\n", a, b);
	
	printf("-------------------------------------\n");
	
	//y = bx+c
	printf("The regression line equation is: ");
	printf("y = %.2f*x + %.2f\n\n", b, a);
	
	//user input x into the equation
	printf("Enter value for x: ");
	scanf("%f",&x_value);
	final = b * x_value + a;
	printf("\nThe value of y = %.2f",final);	
}
