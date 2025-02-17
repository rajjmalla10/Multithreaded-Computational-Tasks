#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lodepng.h"

unsigned char *output;
unsigned char *Image;
unsigned w, h;

//defining structure for multithreading
struct variables
{
	unsigned int start;
	unsigned int end;
};

//creating a function that take void pointer parameter and returns void pointer for gaussian blur
void *GaussianBlur(void *vars)
{
	int i, j;
	int n = 0;
	struct variables *val = (struct variables *)vars;
	unsigned int startLimit = val->start;
	unsigned int endLimit = val->end;

//determing edge, corner and middle for every pixels
	for (i = startLimit; i < endLimit; i += 4)
	{
		//this is for pixels on left
		if ((i % (w * 4)) == 0)
		{

			//this is for pixels on top left corner
			if (i == 0)
			{
				n = 4;
				for (j = 0; j < 3; j++)
				{
					*(output + i + j) = (Image[i + j] + Image[i + 4 + j] + Image[4 * w + j] + Image[4 * w + 4 + j]) / n;
				}
				*(output + i + 3) = Image[i + 3];
			}

			//this is for pixels on bottom left corner
			else if (i == (w * 4 * (h - 1)))
			{
				n = 4;
				for (j = 0; j < 3; j++)
				{
					*(output + i + j) = (Image[i + j] + Image[i + 4 + j] + Image[i - (4 * w) + j] + Image[i - (4 * w) + 4 + j]) / n;
				}
				*(output + i + 3) = Image[i + 3];
			}

			//this is for pixels on middle left
			else
			{
				n = 6;
				for (j = 0; j < 3; j++)
				{
					*(output + i + j) = (Image[i + j] + Image[i + 4 + j] + Image[(4 * w) + i + j] + Image[(4 * w) + i + 4 + j] + Image[i - (4 * w) + j] + Image[i - (4 * w) + 4 + j]) / n;
				}
				*(output + i + 3) = Image[i + 3];
			}
		}

		//this is for pixels at right
		else if ((i % (w * 4)) == (w * 4) - 4)
		{

			//this is for pixels at top right corner
			if (i == ((w * 4) - 4))
			{
				n = 4;
				for (j = 0; j < 3; j++)
				{
					*(output + i + j) = (Image[i + j] + Image[i - 4 + j] + Image[(4 * w) + i + j] + Image[(4 * w) + i - 4 + j]) / n;
				}
				*(output + i + 3) = Image[i + 3];
			}

			//this is for pixels at bottom right corner
			else if (i == ((w * h * 4) - 4))
			{
				n = 4;
				for (j = 0; j < 3; j++)
				{
					*(output + i + j) = (Image[i + j] + Image[i - 4 + j] + Image[i - (4 * w) + j] + Image[i - (4 * w) - 4 + j]) / n;
				}
				*(output + i + 3) = Image[i + 3];
			}

			//this is for pixels at middle right
			else
			{
				n = 6;
				for (j = 0; j < 3; j++)
				{
					*(output + i + j) = (Image[i + j] + Image[i - 4 + j] + Image[(4 * w) + i + j] + Image[(4 * w) + i - 4 + j] + Image[i - (4 * w) + j] + Image[i - (4 * w) - 4] + j) / n;
				}
				*(output + i + 3) = Image[i + 3];
			}
		}

		//this is for pixels at top
		else if (i > 0 && i < ((w * 4) - 4))
		{
			n = 6;
			for (j = 0; j < 3; j++)
			{
				*(output + i + j) = (Image[i + j] + Image[i - 4 + j] + Image[i + 4 + j] + Image[(4 * w) + i + j] + Image[(4 * w) + i + 4 + j] + Image[(4 * w) + i - 4 + j]) / n;
			}
			*(output + i + 3) = Image[i + 3];
		}

		//this is for the bottom
		else if (i > (w * 4 * (h - 1)) && i < ((w * h * 4) - 4))
		{
			n = 6;
			for (j = 0; j < 3; j++)
			{
				*(output + i + j) = (Image[i + j] + Image[i - 4 + j] + Image[i + 4 + j] + Image[i - (4 * w) + j] + Image[i - (4 * w) + 4 + j] + Image[i - (4 * w) - 4 + j]) / n;
			}
			*(output + i + 3) = Image[i + 3];
		}

		//this is for the pixels in middle
		else
		{
			n = 9;
			for (j = 0; j < 3; j++)
			{
				*(output + i + j) = (Image[i + j] + Image[i + 4 + j] + Image[i - 4 + j] + Image[(4 * w) + i + j] + Image[(4 * w) + i + 4 + j] + Image[(4 * w) + i - 4 + j] + Image[i - (4 * w) + j] + Image[i - (4 * w) + 4 + j] + Image[i - (4 * w) - 4 + j]) / n;
			}
			*(output + i + 3) = Image[i + 3];
		}
	}
}

//-------------------------------------------------------------------
//main function
void main()
{
	int i, j;
	unsigned error;
	char filename[20];
	char output_filename[20];

//-------------------------------------------------------------------
//taking input from user
	printf("Enter input file name: \n");
	scanf("%s", filename);

	error = lodepng_decode32_file(&Image, &w, &h, filename);
	if (error)
	{
		printf("Error!!! %u : %s\n", error, lodepng_error_text(error));
	}
	else
	{
		printf("Height of image is: %d pixels and width of image is: %d pixels.\n\n", h, w);
	}

	//allocating memory for output image
	output = (char *)malloc(w * h * 4 * sizeof(char));

	//this divides task among the threads equally
	long iterations = w * h * 4;
	int threads;
	
	//asking for number of threads to use from user

	printf("Enter the number of threads: ");
	scanf("%d", &threads);

	int sliceList[threads];
	int remainder = iterations % threads;

	for (i = 0; i < threads; i++)
	{
		sliceList[i] = iterations / threads;
	}

	for (j = 0; j < remainder; j++)
	{
		sliceList[j] += 1;
	}

	int startList[threads];
	int endList[threads];

	int l;
	for (l = 0; l < threads; l++)
	{
		if (l == 0)
			startList[l] = 0;
		else
			startList[l] = endList[l - 1] + 1;

		endList[l] = startList[l] + sliceList[l] - 1;
	}

	struct variables arr1[threads];

	int k;
	for (k = 0; k < threads; k++)
	{
		arr1[k].start = startList[k];
		arr1[k].end = endList[k];
	}

	pthread_t threadId[threads];

	int m;
	for (m = 0; m < threads; m++)
	{
		pthread_create(&threadId[m], NULL, GaussianBlur, &arr1[m]);
	}

	int nn;
	for (nn = 0; nn < threads; nn++)
	{
		pthread_join(threadId[nn], NULL);
	}

	unsigned char *png;
	size_t pngsize;
	error = lodepng_encode32(&png, &pngsize, output, w, h);

	if (!error)
	{
		lodepng_save_file(png, pngsize, "Output.png");
		printf("Gaussian blur filter has been applied to provided image and saved as Output.png :).");
	}

	free(output); //frees allocated memory
}
