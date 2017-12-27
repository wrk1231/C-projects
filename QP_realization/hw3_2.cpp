
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bag {
    int n;
    double *x;
    double *y;
    double *lb;
    double *ub;
    double *mu;
    double *covariance;
    double *gradient;
    int *order_list;
    double lambda;
    double stepsize;
    int num_iter;
    int iter_counter;
    FILE *output;
};

//*************Function Declaration Begins**********//
int stepdirection(struct bag *pmybag);

int stepsize(struct bag *pmybag);

int move(struct bag *pmybag);

int gradient(struct bag *pmybag);

int readit(char *nameoffile, int *addressofn, double **, double **, double **, double *, double **);

int algo(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda);

int improve(struct bag *pmybag);

int feasible(int n, double *x, double *lb, double *ub);

int q_sort(double *num_list, int *index_list, int start, int endnum);

void quickSort(double *num_list, int *index_list, int start, int endnum);

void ChangeIntoOrderList(int n, int *index_list, int *order_list);
//*************Function Declaration Ends**********//


//*************Function Implementation Begins**********//
int main(int argc, char **argv) {
    int retcode = 0;
    int n;
    double *lb, *ub, *covariance, *mu, lambda, *x;

    if (argc != 2) {
        printf("usage: qp filename\n");
        retcode = 1;
        goto BACK;
    }

    retcode = readit(argv[1], &n, &lb, &ub, &mu, &lambda, &covariance);
    if (retcode) goto BACK;

    x = (double *) calloc(n, sizeof(double));

    if (x == NULL) {
        printf(" no memory for x\n");
        retcode = 1;
        goto BACK;
    }

    retcode = algo(n, x, lb, ub, mu, covariance, lambda);
    BACK:
    return retcode;
}

int readit(char *filename, int *address_of_n, double **address_of_lb, double **pub,
           double **pmu, double *plambda, double **pcovariance) {
    int readcode = 0, fscancode;
    FILE *datafile = NULL;
    char buffer[100];
    int n, i, j;
    double *lb = NULL, *ub = NULL, *mu = NULL, *covariance = NULL;

    datafile = fopen(filename, "r");
    if (!datafile) {
        printf("cannot open file %s\n", filename);
        readcode = 2;
        goto BACK;
    }

    printf("reading data file %s\n", filename);

    fscanf(datafile, "%s", buffer);
    fscancode = fscanf(datafile, "%s", buffer);
    if (fscancode == EOF) {
        printf("problem: premature file end at ...\n");
        readcode = 4;
        goto BACK;
    }

    n = *address_of_n = atoi(buffer);

    printf("n = %d\n", n);

    lb = (double *) calloc(n, sizeof(double));
    *address_of_lb = lb;
    ub = (double *) calloc(n, sizeof(double));
    *pub = ub;
    mu = (double *) calloc(n, sizeof(double));
    *pmu = mu;
    covariance = (double *) calloc(n * n, sizeof(double));
    *pcovariance = covariance;

    if (!lb || !ub || !mu || !covariance) {
        printf("not enough memory for lb ub mu covariance\n");
        readcode = 3;
        goto BACK;
    }

    fscanf(datafile, "%s", buffer);

    for (j = 0; j < n; j++) {
        fscanf(datafile, "%s", buffer);
        fscanf(datafile, "%s", buffer);
        lb[j] = atof(buffer);
        fscanf(datafile, "%s", buffer);
        ub[j] = atof(buffer);
        fscanf(datafile, "%s", buffer);
        mu[j] = atof(buffer);
        printf("j = %d lb = %g ub = %g mu = %g\n", j, lb[j], ub[j], mu[j]);
    }


    fscanf(datafile, "%s", buffer);
    fscanf(datafile, "%s", buffer);
    *plambda = atof(buffer);

    fscanf(datafile, "%s", buffer); /* reading 'covariance'*/

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fscanf(datafile, "%s", buffer);
            covariance[i * n + j] = atof(buffer);
        }
    }


    fscanf(datafile, "%s", buffer);
    if (strcmp(buffer, "END") != 0) {
        printf("possible error in data file: 'END' missing\n");
    }


    fclose(datafile);

    BACK:

    return readcode;
}

int algo(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda) {
    int returncode = 0;
    double *gradient = NULL;
    double *y = NULL;
    int *order_list = NULL;
    struct bag *pmybag = NULL;

    printf("\n running algorithm\n");

    returncode = feasible(n, x, lb, ub);
    if (returncode) {
        goto BACK;
    }

    gradient = (double *) calloc(n, sizeof(double));
    if (!gradient) {
        returncode = 100;
        goto BACK;
    }

    pmybag = (struct bag *) calloc(1, sizeof(struct bag));
    if (!pmybag) {
        returncode = 100;
        goto BACK;
    }

    y = (double *) calloc(n, sizeof(double));

    order_list = (int *) calloc(n, sizeof(int));

    pmybag->n = n;
    pmybag->covariance = covariance;
    pmybag->gradient = gradient;
    pmybag->order_list = order_list;
    pmybag->lb = lb;
    pmybag->ub = ub;
    pmybag->mu = mu;
    pmybag->x = x;
    pmybag->y = y;
    pmybag->lambda = lambda;

    pmybag->num_iter = 10000;

    pmybag->output = fopen("theoutput.dat", "w");
    if (!pmybag->output) {
        printf("could not open output file\n");
        returncode = 200;
        goto BACK;
    }


    returncode = improve(pmybag);

    BACK:
    return returncode;
}

int improve(struct bag *pmybag) {
    int counter, returncode = 0;

    for (counter = 0; counter < pmybag->num_iter; counter++) {
        fprintf(pmybag->output, "iteration # %d\n", counter);
        printf("iteration # %d\n", counter);

        pmybag->iter_counter = counter;

        /** compute gradient **/
        returncode = gradient(pmybag);
        if (returncode) goto BACK;

        /** compute step direction **/
        returncode = stepdirection(pmybag);
        if (returncode) goto BACK;

        /** compute stepsize **/
        returncode = stepsize(pmybag);
        if (returncode) goto BACK;

        /** move in the descent direction by stepsize **/
        returncode = move(pmybag);
        if (returncode) goto BACK;

        // print x (the weights of the portfolio)
        int n = pmybag->n;
        double lambda = pmybag->lambda, F = 0;
        for (int i = 0; i < n; i++) {
            printf("%f ", pmybag->x[i]);;
        }

        // print the value of the objective function F
        for (int i = 0; i < pmybag->n; ++i) {
            for (int j = 0; j < pmybag->n; ++j) {
                F += lambda * pmybag->covariance[i * n + j] * pmybag->x[i] * pmybag->x[j];
            }
            F -= pmybag->mu[i] * pmybag->x[i];
        }
        printf("\nF: %f\n", F);

//        fprintf(pmybag->output, "done with iteration # %d\n\n", counter);
        fprintf(pmybag->output, "done with iteration # %d, the value of F: %f\n\n", counter, F);
        printf("done with iteration # %d\n\n", counter);
    }
    BACK:
    return returncode;
}

int gradient(struct bag *pmybag) {
    int returncode = 0, j, n = pmybag->n, i;
    double lambda = pmybag->lambda, first, second, third;

    fprintf(pmybag->output, "gradient iteration # %d\n", pmybag->iter_counter);
    printf("gradient iteration # %d\n", pmybag->iter_counter);

    for (j = 0; j < n; j++) {
        first = 0;
        first = 2 * lambda * pmybag->x[j] * pmybag->covariance[j * n + j];

        second = 0;
        for (i = 0; i < n; i++)
            if (i != j) {
                second += pmybag->covariance[j * n + i] * pmybag->x[i];
            }
        second *= 2 * lambda;


        third = -pmybag->mu[j];

        pmybag->gradient[j] = first + second + third;


    }
    return returncode;
}

int stepdirection(struct bag *pmybag) {
    int returncode = 0;

    /** sort gradient **/
    int *index_list;
    double *tmp_gradient;

    index_list = (int *) calloc(pmybag->n, sizeof(int));
    tmp_gradient = (double *) calloc(pmybag->n, sizeof(double));
    for (int i = 0; i < pmybag->n; i++) {
        index_list[i] = i;
        tmp_gradient[i] = pmybag->gradient[i];
    }

    quickSort(tmp_gradient, index_list, 0, pmybag->n - 1);
    ChangeIntoOrderList(pmybag->n, index_list, pmybag->order_list);
    // order_list returns the indices of every element if sorted

    /** perform enumeration **/
    double sum = 0;

    int m;
    int position;
    for (m = 1; m <= pmybag->n; ++m) {
        sum = 0;
        for (int i = 0; i < pmybag->n; ++i) {
            if (pmybag->order_list[i] + 1 < m) sum += pmybag->lb[i] - pmybag->x[i];
            else if (pmybag->order_list[i] + 1 == m) position = i;
            else if (pmybag->order_list[i] + 1 > m) sum += pmybag->ub[i] - pmybag->x[i];
        }

        if (-sum >= pmybag->lb[position] - pmybag->x[position] &
            -sum <= pmybag->ub[position] - pmybag->x[position]) {
            // the index is found
            break;
        }

    }

    // generate y
    for (int i = 0; i < pmybag->n; ++i) {
        if (pmybag->order_list[i] + 1 < m) pmybag->y[i] = pmybag->lb[i] - pmybag->x[i];
        else if (pmybag->order_list[i] + 1 == m) pmybag->y[i] = -sum;
        else if (pmybag->order_list[i] + 1 > m) pmybag->y[i] = pmybag->ub[i] - pmybag->x[i];
    }

    // Check if grad.*y < 0
    double inner_product = 0;
    for (int j = 0; j < pmybag->n; ++j) {
        inner_product += pmybag->gradient[j] * pmybag->y[j];
    }

    if (inner_product >= 0) {
        returncode = -1;
    } else {
        returncode = 0;
    }

    return returncode;
}

int stepsize(struct bag *pmybag) {
    int returncode = 0;
    double numerator = 0;
    double denominator = 0;
    for (int i = 0; i < pmybag->n; ++i) {
        numerator += pmybag->mu[i] * pmybag->y[i];
        for (int j = 0; j < pmybag->n; ++j) {
            numerator -= 2 * pmybag->lambda * pmybag->covariance[i * pmybag->n + j] * pmybag->x[i] * pmybag->y[j];
            denominator += 2 * pmybag->lambda * pmybag->covariance[i * pmybag->n + j] * pmybag->y[i] * pmybag->y[j];
        }
    }

    if (denominator == 0) {
        returncode = -2;
        goto BACK;
    }

    pmybag->stepsize = numerator / denominator;

    BACK:
    return returncode;
}

int move(struct bag *pmybag) {
    int returncode = 0;

    for (int i = 0; i < pmybag->n; ++i) {
        pmybag->x[i] = pmybag->x[i] + pmybag->stepsize * pmybag->y[i];
    }

    return returncode;
}

int feasible(int n, double *x, double *lb, double *ub) {
    int returncode = 0, j;
    double sum, increase;

    printf(" computing first feasible solution\n");

    sum = 0;
    /* set variables to lower bounds */
    for (j = 0; j < n; j++) {
        x[j] = lb[j];
        sum += lb[j];
    }
    printf("after first pass, sum = %g\n", sum);

    // No feasible solution, returncode: -1
    if (sum > 1.0) return -1;

    for (j = 0; j < n; j++) {
        increase = 1.0 - sum;
        if (increase > ub[j] - x[j])
            increase = ub[j] - x[j];
        x[j] += increase;
        sum += increase;
        printf("after j = %d, sum = %g\n", j, sum);
        if (sum >= 1.0)
            break;
    }

    return returncode;
}

int q_sort(double *num_list, int *index_list, int start, int endnum) {
    double flag = num_list[endnum];
    int i = start - 1;
    int j = start;

    double temp, temp2;
    int index_temp, index_temp2;

    while (j < endnum) {
        if (num_list[j] > flag) {
            i += 1;

            temp = num_list[i];
            num_list[i] = num_list[j];
            num_list[j] = temp;

            index_temp = index_list[i];
            index_list[i] = index_list[j];
            index_list[j] = index_temp;

        }
        j++;
    }
    temp2 = num_list[i + 1];
    num_list[i + 1] = num_list[endnum];
    num_list[endnum] = temp2;

    index_temp2 = index_list[i + 1];
    index_list[i + 1] = index_list[endnum];
    index_list[endnum] = index_temp2;

    return i + 1;


}

void quickSort(double *num_list, int *index_list, int start, int endnum) {

    if (start >= endnum) {
        return;
    }
    int mid;
    mid = q_sort(num_list, index_list, start, endnum);
    quickSort(num_list, index_list, start, mid - 1);
    quickSort(num_list, index_list, mid + 1, endnum);
}

void ChangeIntoOrderList(int n, int *index_list, int *order_list) {
    for (int k = 0; k < n; k++) {
        order_list[index_list[k]] = k;
    }
}
//*************Function Implementation Ends**********//