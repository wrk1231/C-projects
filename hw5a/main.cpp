#include <stdio.h>
#include <stdlib.h> //For system("pause");
#include <string.h>
#include <time.h> //For generating random vectors
#include <math.h> //For calculating square roots
#define TOTALASSETS 204
#define TOTALDAYS 252
int readit(char *nameoffile, double **);
int algo(double *price, int numofassets, int numofdays, int movingtimes);
int powermethod(double *covariance, double *w, int numofassets);

int main(int argc, char **argv)
{
    int retcode = 0;
    int numofassets, numofdays,movingtimes;
    double *price;

    if (argc != 5) {
        printf("usage: main.exe filename numofassets numofdays movingtimes\n");  retcode = 1;
        goto BACK;
    }
    numofassets = atoi(argv[2]);
    if (numofassets > TOTALASSETS) {
        printf("numofassets out of range!"); retcode = 1;
        goto BACK;
    }
    numofdays = atoi(argv[3]);
    if (numofdays > TOTALDAYS) {
        printf("numofdays out of range!"); retcode = 1;
        goto BACK;
    }
    movingtimes = atoi(argv[4]);
    if (movingtimes + numofdays > TOTALDAYS) {
        printf("movingtimes out of range!"); retcode = 1;
        goto BACK;
    }
    retcode = readit(argv[1], &price);
    if (retcode) goto BACK;
    retcode = algo(price, numofassets, numofdays, movingtimes);
    BACK:
    return retcode;
}

int readit(char *filename, double **pprice)
{
    int readcode = 0;
    FILE *datafile = NULL;
    char buffer[100];
    double  *price = NULL;

    datafile = fopen(filename, "r");
    if (!datafile) {
        printf("cannot open file %s\n", filename);
        readcode = 2;  goto BACK;
    }
    printf("reading data file %s\n", filename);
    price = (double *)calloc(TOTALASSETS * TOTALDAYS, sizeof(double));
    if (!price) {
        printf("not enough memory for price\n"); readcode = 3; goto BACK;
    }
    *pprice = price;
    for (int i = 0; i < TOTALASSETS; i++) {
        for (int j = 0; j < TOTALDAYS; j++) {
            fscanf(datafile, "%s", buffer);
            //printf("%d %d\n",i,j);
            price[i * TOTALDAYS + j] = atof(buffer);
        }
    }
    printf("reading data file %s done!\n", filename);
    fclose(datafile);

    BACK:
    return readcode;
}

double getsumX(double *price, int numofdays) {
    double sum = 0.0;
    for (int k = 1; k < numofdays; k++) {
        // sum += price[k];
        sum += price[k]/price[k - 1] - 1;
    }
    return sum;
}

double getsumXY(double *price1, double *price2, int numofdays) {
    double sum = 0.0;
    for (int k = 1; k < numofdays; k++) {
        // sum += price1[k] * price2[k];
        sum += (price1[k]/price1[k - 1] - 1) * (price2[k]/price2[k - 1] - 1);
    }
    return sum;
}

void generand(double *w, int nrow) {
    srand(time(NULL));
    double  sum = 0.0;
    for (int i = 0; i < nrow; i++) {
        w[i] = rand();
        sum += w[i] * w[i];
    }
    sum = sqrt(sum);
    for (int i = 0; i < nrow; i++) {
        w[i] = w[i] / sum;
    }
}

double disofVec(double *vector1, double *vector2, int nrow) {
    double distance = 0.0;
    for (int i = 0; i < nrow; i++) {
        distance += (vector1[i] - vector2[i]) * (vector1[i] - vector2[i]);
    }
    return sqrt(distance);
}

int algo(double *price, int numofassets, int numofdays, int movingtimes) {
    int retcode = 0;
    double *sumX = NULL, *sumXY = NULL, *covariance = NULL, *w = NULL;
    sumX = (double *)calloc(numofassets, sizeof(double));
    sumXY = (double *)calloc(numofassets * numofassets, sizeof(double));
    covariance = (double *)calloc(numofassets * numofassets, sizeof(double));
    if (!sumX || !sumXY ||!covariance) {
        printf("not enough memory for sumX or sumXY or covariance\n"); retcode = 3; goto BACK;
    }
    for (int i = 0; i < numofassets; i++) {
        sumX[i] = getsumX(price + TOTALDAYS * i, numofdays);
        for (int j = 0; j < numofassets; j++) {
            if (i > j) {
                sumXY[i * numofassets + j] = sumXY[j * numofassets + i];
            }
            else
            {
                sumXY[i * numofassets + j] = getsumXY(price + TOTALDAYS * i, price + TOTALDAYS * j, numofdays);
            }
        }
    }
    for (int k = 0; k <= movingtimes; k++) {
        if (k > 0) {
            for (int i = 0; i < numofassets; i++) {
                //sumX[i] += price[i * TOTALDAYS + numofdays + k - 1];
                //sumX[i] -= price[i * TOTALDAYS + k - 1];
                sumX[i] += price[i * TOTALDAYS + numofdays + k - 1] / price[i * TOTALDAYS + numofdays + k - 2] - 1;
                sumX[i]	-= price[i * TOTALDAYS + k] / price[i * TOTALDAYS + k - 1] - 1;
                for (int j = 0; j < numofassets; j++) {
                    if (i > j) {
                        sumXY[i * numofassets + j] = sumXY[j * numofassets + i];
                    }
                    else
                    {
                        //sumXY[i * numofassets + j] += price[i * 250 + numofdays + k - 1] * price[j * 250 + numofdays + k - 1];
                        //sumXY[i * numofassets + i] -= price[i * 250 + k - 1] * price[j * 250 + k - 1];
                        sumXY[i * numofassets + j] += (price[i * TOTALDAYS + numofdays + k - 1] / price[i * TOTALDAYS + numofdays + k - 2] - 1) * (price[j * TOTALDAYS + numofdays + k - 1]/price[j * TOTALDAYS + numofdays + k - 2] - 1);
                        sumXY[i * numofassets + j] -= (price[i * TOTALDAYS + k] / price[i * TOTALDAYS + k - 1] - 1) * (price[j * TOTALDAYS + k]/price[j * TOTALDAYS + k - 1] - 1);
                    }
                }
            }
        }
        for (int i = 0; i < numofassets; i++) {
            for (int j = 0; j < numofassets; j++) {
                //covariance[i * numofassets + j] = (sumXY[i * numofassets + j] - sumX[i] * sumX[j] / numofdays)/(numofdays - 1);
                covariance[i * numofassets + j] = (sumXY[i * numofassets + j] - sumX[i] * sumX[j] / (numofdays - 1))/(numofdays - 2);
            }
        }
        //Next generate a random vector w for the power method
        if (k == 0) {
            w = (double *)calloc(numofassets, sizeof(double));
            if (!w) {
                printf("not enough memory for w\n"); retcode = 3; goto BACK;
            }
            generand(w, numofassets);
        }


        printf("Now start computing the largest eigenvalue from D%d to D%d of the first %d assets\n", 1 + k, numofdays + k, numofassets);
        retcode = powermethod(covariance, w, numofassets);
        if (!retcode) {
            printf("Power method done.\n");
        }
    }
    BACK:
    return retcode;
}

int powermethod(double *covariance, double *w, int numofassets) {
    int retcode = 0;
    double epsilon = 1e-12, distance, sum, result;
    double *temp = NULL;
    temp = (double *)calloc(numofassets, sizeof(double));
    if (!temp) {
        printf("not enough memory for temp\n"); retcode = 3; goto BACK;
    }
    while(1)
    {
        sum = 0.0;
        for (int i = 0; i < numofassets; i++) {
            temp[i] = 0.0;
            for (int j = 0; j < numofassets; j++) {
                temp[i] += covariance[i * numofassets + j] * w[j];
            }
            sum += temp[i] * temp[i];
        }
        sum = sqrt(sum);
        for (int i = 0; i < numofassets; i++) {
            temp[i] = temp[i] / sum;
        }
        if (disofVec(temp, w, numofassets) >= epsilon) {
            for (int i = 0; i < numofassets; i++) {
                w[i] = temp[i];
            }
        }
        else {
            sum = 0.0;
            for (int i = 0; i < numofassets; i++) {
                temp[i] = 0.0;
                for (int j = 0; j < numofassets; j++) {
                    temp[i] += covariance[i * numofassets + j] * w[j];
                }
                sum += temp[i] * temp[i];
            }
            sum = sqrt(sum);
            //printf("%f %f", covariance[0], covariance[numofassets * numofassets - 1]);
            printf("The largest eigenvalue is %f.\n", sum);
            break;
        }
    } ;
    BACK:
    return retcode;
}