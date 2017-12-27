#include <stdio.h>
#include <stdlib.h> // For system("pause");
#include <string.h>

double mean_price(double input[], int ncol) {
    double sum = 0.0;
    double meanp;
    for (int k = 0; k < ncol; ++k) {
        sum += input[k];
    }
    meanp = sum / ncol;
    return meanp;
}
/*
double** return_cov(double** data,int size_n,int size_m)
{
    double covmat[size_n][size_n];
    for (int k = 0; k < size_n; ++k) {
        for (int l = k; l < size_n; ++l) {

            double temp_sum = 0.0;
            for (int m = 0; m < size_m; ++m) {
                temp_sum += data[k][m]* data[l][m];
            }

            temp_sum = temp_sum/(size_n-1);
            covmat[k][l] = temp_sum;
            covmat[l][k] = temp_sum;

        }
    }
    return covmat;

}*/

int main() {
    int size_n = 1000;
    int size_m = 250;


    int i = 0, j = 0;
    double mat[size_n][size_m];
    double covmat[size_n][size_n];
    double meanprice_vector[size_n];
    FILE *fstream;

    fstream = fopen("dump2.csv", "r");

    float temp;
    char comma;
    for (i = 0; i < 1000; i++) {
        for (j = 0; j < 250; j++) {
            fscanf(fstream, "%e", &temp);
            fscanf(fstream, "%s", &comma);

            mat[i][j] = temp;
        }
    }
    printf("%d", 1);
    double tmp[3] = {1,2,3};
    meanprice_vector[0] = mean_price(tmp, 3);
//    meanprice_vector[0] = mean_price(mat[0], size_m);
//    printf("%e ",meanprice_vector[0]);
//    for (int i = 0; i < 2; i++) {
//        meanprice_vector[i] = mean_price(mat[i], size_m);
//        printf("%e ",meanprice_vector[i]);
//    }
/*
    for (i = 0; i < size_n; i++) {
        for (j = 0; j < size_m; j++) {

            mat[i][j] = mat[i][j] - meanprice_vector[i];

        }
    }

    for (int k = 0; k < size_n; ++k) {
        for (int l = k; l < size_n; ++l) {

            double temp_sum = 0.0;
            for (int m = 0; m < size_m; ++m) {
                temp_sum += mat[k][m]* mat[l][m];
            }

            temp_sum = temp_sum/(size_n-1);
            covmat[k][l] = temp_sum;
            covmat[l][k] = temp_sum;
            printf("%e",temp_sum);
        }
    }
*/

    fclose(fstream);
    return 0;
}