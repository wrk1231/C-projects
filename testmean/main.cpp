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

int main() {

//    double mat[2][3] = {{1,2,3},{4,5,6}};
    double tmp[3] = {1,2,3};
    double fuck = mean_price(tmp, 3);
//    double fuck = mean_price(mat[0],3);
    printf("%e",fuck);

    return 0;
}