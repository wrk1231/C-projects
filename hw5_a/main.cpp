//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h> //For system("pause");
#include <string.h>
#include <time.h> //For generating random vectors
#include <math.h> //For calculating square roots
#include "gurobi_c++.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>



#define lambdanum 10
#define totalassets 947
#define totaldays 504
#define nameslimit 1000
using namespace std;

int readit(char *nameoffile, double **);

int algo(double *price, int numofassets, int numofdays);

int powermethod(double *covariance, double *w, double *eigenvalue, int numofassets);

int factormodel(double *covariance, double *w, double *averageX, double *eigenvalue, double lambda);

int main(int argc, char **argv) {
    int retcode = 0;
    int numofassets, numofdays;
    double *price;
    if (argc != 2) {
        printf("usage: main.exe filename \n");
        retcode = 1;
        goto BACK;
    }
    numofassets = totalassets;
    numofdays = 252;
    retcode = readit(argv[1], &price);
    if (retcode) goto BACK;
    retcode = algo(price, numofassets, numofdays);




BACK:
    return retcode;
}

int readit(char *filename, double **pprice) {
    int readcode = 0;
    FILE *datafile = NULL;
    char buffer[1000];
//    int totalassets = totalassets, totaldays = 504;
    double *price = NULL;

    datafile = fopen(filename, "r");
    if (!datafile) {
        printf("cannot open file %s\n", filename);
        readcode = 2;
        goto BACK;
    }
    printf("reading data file %s\n", filename);
    price = (double *) calloc(totalassets * totaldays, sizeof(double));
    if (!price) {
        printf("not enough memory for price\n");
        readcode = 3;
        goto BACK;
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
    for (int k = 1; k < numofdays; k++) {
        // sum += price[k];
        sum += price[k] / price[k - 1] - 1;
    }
    return sum;
}

double getsumXY(double *price1, double *price2, int numofdays) {
    double sum = 0.0;
    for (int k = 1; k < numofdays; k++) {
        // sum += price1[k] * price2[k];
        sum += (price1[k] / price1[k - 1] - 1) * (price2[k] / price2[k - 1] - 1);
    }
    return sum;
}

void generand(double *w, int nrow) {
    srand(time(NULL));
    double sum = 0.0;
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

int algo(double *price, int numofassets, int numofdays) {
    int retcode = 0;
    double lambda = 0;
    double *sumX = NULL, *sumXY = NULL, *covariance = NULL, *w = NULL, *averageX = NULL, *eigenvalue = NULL;
    sumX = (double *) calloc(numofassets, sizeof(double));
    averageX = (double *) calloc(numofassets, sizeof(double));
    sumXY = (double *) calloc(numofassets * numofassets, sizeof(double));
    covariance = (double *) calloc(numofassets * numofassets, sizeof(double));
    eigenvalue = (double *) calloc(lambdanum, sizeof(double));
    if (!sumX || !sumXY || !covariance) {
        printf("not enough memory for sumX or sumXY or covariance\n");
        retcode = 3;
        goto BACK;
    }
    //Compute average return

    for (int i = 0; i < numofassets; i++) {
        sumX[i] = getsumX(price + totaldays * i, numofdays);
        averageX[i] = getsumX(price + totaldays * i, numofdays) / (numofdays - 1);
        for (int j = 0; j < numofassets; j++) {
            if (i > j) {
                sumXY[i * numofassets + j] = sumXY[j * numofassets + i];
            } else {
                sumXY[i * numofassets + j] = getsumXY(price + totaldays * i, price + totaldays * j, numofdays);
            }
        }
    }
    //Generate covariance matrix
    for (int i = 0; i < numofassets; i++) {
        for (int j = 0; j < numofassets; j++) {
            //covariance[i * numofassets + j] = (sumXY[i * numofassets + j] - sumX[i] * sumX[j] / numofdays)/(numofdays - 1);
            covariance[i * numofassets + j] =
                    (sumXY[i * numofassets + j] - sumX[i] * sumX[j] / (numofdays - 1)) / (numofdays - 2);
        }
    }
    //Next generate a random vector w for the power method

    w = (double *) calloc(numofassets * lambdanum, sizeof(double));
    if (!w) {
        printf("not enough memory for w\n");
        retcode = 3;
        goto BACK;
    }
    generand(w, numofassets * lambdanum);


    printf("Now start computing the largest eigenvalues for %d days of the %d assets\n", numofdays, numofassets);
    retcode = powermethod(covariance, w, eigenvalue, numofassets);
    if (!retcode) {
        printf("Power method done.\n");
    }
//    for (int i=1;i<10;i++) {
//        lambda = i * 10;
//    lambda = 5;
    lambda=300;
    printf("lambda is %f.\n", lambda);
    retcode = factormodel(covariance, w, averageX, eigenvalue, lambda);
//    }
    if (!retcode) {
        printf("QP model done.\n");
    }
//    }
    BACK:
    return retcode;
}

int powermethod(double *covariance, double *w, double *eigenvalue, int numofassets) {
    int retcode = 0;
    double epsilon = 1e-12, distance, sum, result;
    double *temp = NULL;
    temp = (double *) calloc(numofassets, sizeof(double));
    if (!temp) {
        printf("not enough memory for temp\n");
        retcode = 3;
        goto BACK;
    }
    for (int k = 0; k < lambdanum; k++) {
        while (1) {
            sum = 0.0;
            for (int i = 0; i < numofassets; i++) {
                temp[i] = 0.0;
                for (int j = 0; j < numofassets; j++) {
                    temp[i] += covariance[i * numofassets + j] * w[j + lambdanum * k];
                }
                sum += temp[i] * temp[i];
            }
            sum = sqrt(sum);
            for (int i = 0; i < numofassets; i++) {
                temp[i] = temp[i] / sum;
            }
            if (disofVec(temp, w + lambdanum * k, numofassets) >= epsilon) {
                for (int i = 0; i < numofassets; i++) {
                    w[i + lambdanum * k] = temp[i];
                }
            } else {
                sum = 0.0;
                for (int i = 0; i < numofassets; i++) {
                    temp[i] = 0.0;
                    for (int j = 0; j < numofassets; j++) {
                        temp[i] += covariance[i * numofassets + j] * w[j + lambdanum * k];
                    }
                    sum += temp[i] * temp[i];
                }
                sum = sqrt(sum);
                eigenvalue[k] = sum;
                printf("The %dth eigenvalue is %f.\n", k + 1, sum);
                break;
            }
        }
        for (int i = 0; i < numofassets; i++) {
            for (int j = 0; j < numofassets; j++) {
                //covariance[i * numofassets + j] = (sumXY[i * numofassets + j] - sumX[i] * sumX[j] / numofdays)/(numofdays - 1);
                covariance[i * numofassets + j] =
                        covariance[i * numofassets + j] - sum * w[i + lambdanum * k] * w[j + lambdanum * k];
            }
        }

    };
    BACK:
    return retcode;
}

// covariance-> sigma, w->V, averageX->mu, eigenvalue->F
int factormodel(double *covariance, double *w, double *averageX, double *eigenvalue, double lambda) {
    int retcode = 0;
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    int n, j;
    double *obj = NULL;
    double *lb = NULL;
    double *ub = NULL;
    double *x;
    int *qrow, *qcol, Nq;
    double *qval;
    int *cind;
    double rhs;
    char sense;
    double *cval;
    int numnonz;
    int numofname = 0;

    char **names, *vartype;
    ofstream outFile;
    outFile.open("data.csv", ios::out);


    n = totalassets + lambdanum + totalassets; /** 947 'x' variables, 10- factor variables, 947 binary variables **/


    retcode = GRBloadenv(&env, "factormodel.log");
    if (retcode) goto BACK;

    /* Create initial model */
    retcode = GRBnewmodel(env, &model, "second", n,
                          NULL, NULL, NULL, NULL, NULL);

    names = (char **) calloc(n, sizeof(char *));

    /** next we create the remaining attributes for the n columns **/
    obj = (double *) calloc(n, sizeof(double));
    ub = (double *) calloc(n, sizeof(double));
    lb = (double *) calloc(n, sizeof(double));
    x = (double *) calloc(n, sizeof(double));
    vartype = (char *) calloc(n, sizeof(char));

    // set x
    for (j = 0; j < totalassets; j++) {
        names[j] = (char *) calloc(3, sizeof(char));
        if (names[j] == NULL) {
            retcode = 1;
            goto BACK;
        }
        sprintf(names[j], "x%d", j);

        /** calloc initializes memory to zero, so all other obj[j] are zero **/
        obj[j] = -averageX[j];
        /**next, the upper bounds on the x variables **/
        ub[j] = 0.02;
        lb[j] = 0.0;
    }

    // set y
    for (j = totalassets; j < totalassets+lambdanum; j++) {
        names[j] = (char *) calloc(3, sizeof(char));
        if (names[j] == NULL) {
            retcode = 1;
            goto BACK;
        }
        sprintf(names[j], "y%d", j - totalassets);
        obj[j] = 0.0;
        ub[j] = 1000;
        lb[j] = -1000;
    }

    //set z
    for (j = totalassets+lambdanum; j < n; j++) {
        names[j] = (char *) calloc(3, sizeof(char));
        if (names[j] == NULL) {
            retcode = 1;
            goto BACK;
        }
        sprintf(names[j], "z%d", j - totalassets-lambdanum);
        obj[j] = 0.0;
        ub[j] = 1.0;
        lb[j] = 0.0;
    }


    for (int i = totalassets; i < lambdanum+totalassets; i++) {
        ub[i] = 1000;
        lb[i] = -1000;
    }


    /* initialize variables */
    for (j = 0; j < n; j++) {
        retcode = GRBsetstrattrelement(model, "VarName", j, names[j]);
        if (retcode) goto BACK;

        retcode = GRBsetdblattrelement(model, "Obj", j, obj[j]);
        if (retcode) goto BACK;

        retcode = GRBsetdblattrelement(model, "LB", j, lb[j]);
        if (retcode) goto BACK;

        retcode = GRBsetdblattrelement(model, "UB", j, ub[j]);
        if (retcode) goto BACK;

        if (j < lambdanum + totalassets) vartype[j] = GRB_CONTINUOUS;
        else vartype[j] = GRB_BINARY;

        retcode = GRBsetcharattrelement(model, "VTYPE", j, vartype[j]);
        if (retcode) goto BACK;
    }

    /** next, the quadratic -- there are 957 nonzeroes: 947 residual variances plus the 10
                                      factor covariance matrix**/

    Nq = lambdanum + totalassets;
    qrow = (int *) calloc(Nq, sizeof(int));  /** row indices **/
    qcol = (int *) calloc(Nq, sizeof(int));  /** column indices **/
    qval = (double *) calloc(Nq, sizeof(double));  /** values **/

    if ((qrow == NULL) || (qcol == NULL) || (qval == NULL)) {
        printf("could not create quadratic\n");
        retcode = 1;
        goto BACK;
    }
    // residual variances
    for (int i = 0; i < totalassets; i++) {
        qval[i] = lambda * covariance[i * totalassets + i];
        qrow[i] = i;
        qcol[i] = i;
    }

    // factor covariance
    for (int i = totalassets; i < Nq; i++) {
        qval[i] = lambda * eigenvalue[i - totalassets];
        qrow[i] = i;
        qcol[i] = i;
    }
    retcode = GRBaddqpterms(model, Nq, qrow, qcol, qval);
    if (retcode) goto BACK;

    /** now we will add one constraint at a time **/
    /** we need to have a couple of auxiliary arrays **/

    cind = (int *) calloc(n, sizeof(int));  /** n is over the top since no constraint is totally dense;
					     but it's not too bad here **/
    cval = (double *) calloc(n, sizeof(double));

    /** ten factor constraints, first one is next**/

    for (int i = 0; i < lambdanum; i++) {
        for (int j = 0; j < totalassets; j++) {
            cval[j] = w[i * lambdanum + j];
            cind[j] = j;
        }
        cval[totalassets] = -1;
        cind[totalassets] = i + totalassets;
        numnonz = totalassets + 1;
        rhs = 0;
        sense = GRB_EQUAL;
        char buffer[100];
        sprintf(buffer, "%dth constraint", i);
//        string temp(buffer);
        retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, buffer);
        if (retcode) goto BACK;

    }

    /** sum of x variables = 1 **/
    for (j = 0; j < totalassets; j++) {
        cind[j] = j;
        cval[j] = 1.0;
    }

    numnonz = totalassets;
    rhs = 1.0;
    sense = GRB_EQUAL;

    retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "Total weight equals one");
    if (retcode) goto BACK;

    /** sum of z variables <= nameslimit **/
    for (j = 0; j < totalassets; j++) {
        cval[j] = 1.0;
        cind[j] = j + totalassets + lambdanum;
    }

    numnonz = totalassets;
    rhs = nameslimit;
    sense = GRB_LESS_EQUAL;

    retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "limit of names");
    if (retcode) goto BACK;


    /** now say xj = 0 unless zj = 1 **/

    for (j = 0; j < totalassets; j++) {
        cval[0] = 1.0;
        cind[0] = j;
        cval[1] = -1.0; // in case there is short position
        cind[1] = totalassets + lambdanum + j;

        numnonz = 2;
        rhs = 0.00001;
        sense = GRB_LESS_EQUAL;

        char buffer[100];
        sprintf(buffer, "control%d", j);

        retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, buffer);
        if (retcode) goto BACK;
    }

    retcode = GRBupdatemodel(model);
    if (retcode) goto BACK;

    /** optional: write the problem **/

    retcode = GRBwrite(model, "factorqp.lp");
    if (retcode) goto BACK;


    retcode = GRBoptimize(model);
    if (retcode) goto BACK;


    /** get solution **/
    retcode = GRBgetdblattrarray(model,
                                 GRB_DBL_ATTR_X, 0, n,
                                 x);
    if (retcode) goto BACK;

    /** now let's see the values **/
    for (j = 0; j < n-totalassets-lambdanum; j++) {
        if (x[j] > 0.00001) {
            printf("%s = %g\n", names[j], x[j]);
            numofname += 1;
        }
    }
    printf("number of names is %d\n", numofname);

    for (int k = 0; k < totalassets; ++k) {
        outFile<< x[k]<<',';

    }
    outFile<<endl;

    for (int k = totalassets + lambdanum; k < totalassets + lambdanum + totalassets; ++k) {
        outFile<< x[k]<<',';

    }
    outFile<<endl;


    outFile.close();

//    numofname = 0;
//    for (j = 0; j < totalassets; j++) {
//        numofname += x[totalassets + lambdanum + j];
//        // If the corresponding binary variable is 1, print the position
//        if (x[totalassets + lambdanum + j] > 0.5) printf("%s = %g\n", names[j], x[j]);
//    }
//    printf("number of names by binary variables is %d\n", numofname);

    GRBfreeenv(env);





    return 0;



    BACK:
    printf("\nexiting with retcode %d\n", retcode);
    return retcode;
}


