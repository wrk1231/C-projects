
#include <stdio.h>
#include <stdlib.h> //For system("pause");
#include <string.h>
#include <time.h> //For generating random vectors
#include <math.h> //For calculating square roots

int readit(char *nameoffile, double **);
double _stdcall algo(double *price, int numofassets, int numofdays, int movingtimes);
double powermethod(double *covariance, double *w, int numofassets);

int readit(char *filename, double **pprice)
{
	int readcode = 0;
	FILE *datafile = NULL;
	char buffer[100];
	int totalassets = 1000, totaldays = 250;
	double  *price = NULL;

	datafile = fopen(filename, "r");
	if (!datafile) {
		printf("cannot open file %s\n", filename);
		readcode = 2;  goto BACK;
	}
	printf("reading data file %s\n", filename);
	price = (double *)calloc(totalassets * totaldays, sizeof(double));
	if (!price) {
		printf("not enough memory for price\n"); readcode = 3; goto BACK;
	}
	*pprice = price;
	for (int i = 0; i < totalassets; i++) {
		for (int j = 0; j < totaldays; j++) {
			fscanf(datafile, "%s", buffer);
			//printf("%d %d\n",i,j);
			price[i * totaldays + j] = atof(buffer);
		}
	}
	printf("reading data file %s done!\n", filename);
	fclose(datafile);

BACK:
	return readcode;
}

double getsumX(double *price, int numofdays) {
	double sum = 0.0;
	for (int k = 0; k < numofdays; k++) {
		sum += *(price+k);
	}
	return sum;
}

double getsumXY(double *price1, double *price2, int numofdays) {
	double sum = 0.0;
	for (int k = 0; k < numofdays; k++) {
		sum += *(price1+k) * *(price2+k);
	}
	return sum;
}

void generand(double *w, int nrow) {
	srand(time(NULL));
	double  sum = 0.0;
	for (int i = 0; i < nrow; i++) {
		*(w + i) = rand();
		sum += *(w+i) * *(w + i);
	}
	sum = sqrt(sum);
	for (int i = 0; i < nrow; i++) {
		*(w + i) = *(w + i) / sum;
	}
}

double disofVec(double *vector1, double *vector2, int nrow) {
	double distance = 0.0;
	for (int i = 0; i < nrow; i++) {
		distance += (*(vector1+i) - *(vector2 + i)) * (*(vector1 + i) - *(vector2 + i));
	}
	return sqrt(distance);
}

double _stdcall algo(double *price, int numofassets, int numofdays, int movingtimes) {
	int retcode = 0;
	double *w, *sumX, *sumXY, *covariance,*res;
	sumX = (double *)calloc(numofassets, sizeof(double));
	sumXY = (double *)calloc(numofassets * numofassets, sizeof(double));
	covariance = (double *)calloc(numofassets * numofassets, sizeof(double));
	res = (double *)calloc(movingtimes + 1, sizeof(double));
	double result;
	if (!sumX ) {
		retcode = -3;
		goto BACK;
	}
	if (!sumXY) {
		retcode = -4;
		goto BACK;
	}
	if (!covariance) {
		retcode = -5;
		goto BACK;
	}
	if (!res) {
		retcode = -6;
		goto BACK;
	}
	for (int i = 0; i < numofassets; i++) {
		*(sumX+i) = getsumX(price + 250 * i, numofdays);
		for (int j = 0; j < numofassets; j++) {
			if (i > j) {
				*(sumXY+i * numofassets + j) = *(sumXY+j * numofassets + i);
			}
			else
			{
				*(sumXY + i * numofassets + j) = getsumXY(price + 250 * i, price + 250 * j, numofdays);
			}
		}
	}

	for (int k = 0; k <= movingtimes; k++) {
		if (k > 0) {
			for (int i = 0; i < numofassets; i++) {
				*(sumX + i) = *(sumX + i) + *(price+i * 250 + numofdays + k - 1) - *(price+i * 250 + k - 1);
				for (int j = 0; j < numofassets; j++) {
					if (i > j) {
						*(sumXY + i * numofassets + j) = *(sumXY + j * numofassets + i);
					}
					else
					{
						*(sumXY + i * numofassets + j) += *(price + i * 250 + numofdays + k - 1) * *(price + j * 250 + numofdays + k - 1);
						*(sumXY + i * numofassets + j) -= *(price + i * 250 + k - 1) * *(price + j * 250 + k - 1);
					}
				}
			}
		}
		for (int i = 0; i < numofassets; i++) {
			for (int j = 0; j < numofassets; j++) {
				*(covariance+i * numofassets + j) = (*(sumXY + i * numofassets + j) - *(sumX + i) * *(sumX + j) / numofdays) / (numofdays - 1);
			}
		}
		//Next generate a random vector w for the power method
		if (k == 0) {
			w = (double *)calloc(numofassets, sizeof(double));
			if (!w) {
				retcode = -2;
				goto BACK;
			}
			generand(w, numofassets);
		}


		//printf("Now start computing the largest eigenvalue from D%d to D%d of the first %d assets\n", 1 + k, numofdays + k, numofassets);
		*(res+k) = powermethod(covariance, w, numofassets);
		if (*(res + k) == -1){
			retcode = -1;
			goto BACK;
		}
		printf("%f\n", *(res + k));
	}

	//return *res;
	result = *res;
	return result;
BACK:
	return retcode;
}

double powermethod(double *covariance, double *w, int numofassets) {
	double retcode = 0.0;
	double epsilon = 1e-12, distance, sum, result;
	double *temp = NULL;
	temp = (double *)calloc(numofassets, sizeof(double));
	if (!temp) {
		retcode = -1; goto BACK;
	}
	while (1)
	{
		sum = 0.0;
		for (int i = 0; i < numofassets; i++) {
			*(temp+i) = 0.0;
			for (int j = 0; j < numofassets; j++) {
				*(temp + i) += *(covariance+i * numofassets + j) * *(w + j);
			}
			sum += *(temp + i) * *(temp + i);
		}
		sum = sqrt(sum);
		for (int i = 0; i < numofassets; i++) {
			*(temp + i) = *(temp + i) / sum;
		}
		if (disofVec(temp, w, numofassets) >= epsilon) {
			for (int i = 0; i < numofassets; i++) {
				*(w+i) = *(temp + i);
			}
		}
		else {
			return sum;
		}
	};
BACK:
	return retcode;
}
