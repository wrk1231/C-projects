#include <iostream>
#include <stdio.h>
#include <stdlib.h> //For system("pause");
#include <string.h>
#include <time.h> //For generating random vectors
#include <math.h> //For calculating square roots
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>



#define lambdanum 10
#define totalassets 947
#define totaldays 252
#define filename  "data.csv"
#define nameslimit 1000
using namespace std;

int readweight(double **peach_weight);

int try_to_get_weight();

int main(int argc, char **argv) {
    int retcode = 0;
    int numofassets;
    double *weight;

    if (argc != 1) {
        printf("usage: main.exe  \n");
        retcode = 1;
        goto BACK;
    }
    numofassets = totalassets;

    retcode = try_to_get_weight();




    BACK:
    return retcode;
}

int try_to_get_weight(){
    double *true_weight;
    int retcode = 0;
    true_weight = (double *) calloc(totalassets, sizeof(double));
    retcode = readweight(&true_weight);
    for (int i = 0; i < totalassets; i++) {

        printf(" %lf ", true_weight[i]);

    }
    return retcode;
}
int readweight(double **peach_weight) {
    int readcode = 0;
    FILE *datafile = NULL;
    char buffer[1000];
//    int totalassets = totalassets, totaldays = 504;
    double *each_weight = NULL;

    datafile = fopen(filename, "r");
    if (!datafile) {
        printf("cannot open file %s\n", filename);
        readcode = 2;
        goto BACK;
    }
    printf("reading data file %s\n", filename);
    each_weight = (double *) calloc(totalassets, sizeof(double));
    if (!each_weight) {
        printf("not enough memory for weight\n");
        readcode = 3;
        goto BACK;
    }
    *peach_weight = each_weight;

    for (int i = 0; i < totalassets; i++) {
        fscanf(datafile, "%s", buffer);
        each_weight[i] = atof(buffer);

        //printf("%lf ", each_weight[i]);

    }
    printf("\n read data file %s\n finished", filename);

    fclose(datafile);

    BACK:
    return readcode;
}