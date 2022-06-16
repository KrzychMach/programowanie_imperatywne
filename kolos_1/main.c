#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _matrix{

    int rows;

    int cols;

    double*data;

}Matrix;


Matrix* create_matrix(int rows, int cols);


Matrix*add_scalar(const Matrix*pm, double v) {
    Matrix *new_matrix = create_matrix(pm->rows, pm->cols);

    for (int i = 0; i < pm->rows * pm->cols; ++i) {
        new_matrix->data[i] = pm->data[i] + v;
    }

    return new_matrix;
}


void print_vect(const double*vect, int size) {
    printf("[");
    for (int i = 0; i < size; ++i) {
        printf("%f,", vect[i]);
        if (i != size - 1) printf(",");
    }
    printf("]\n");
}


void print_matrix(const Matrix*pm) {
    for (int i = 0; i < pm->rows; ++i) {
        print_vect(&pm->data[i * pm->cols], pm->cols);
    }
}


void max_elements_in_cols(double*maxdata,const Matrix*pm) {
    for (int i = 0; i < pm->cols; ++i) {
        double max = 0;
        for (int j = 0; j < pm->rows; ++j) {
            if (pm->data[j * pm->rows + i] > max) {
                max = pm->data[j * pm->rows + i];
            }
        }
        maxdata[i] = max;
    }
}


void square_elements(double *num) {
    *num = *num * *num;
}


void apply_to_elements(const Matrix*pm, void (*elem_fun)(double*)) {
    for (int i = 0; i < pm->rows * pm->cols; ++i) {
        (*elem_fun)(&pm->data[i]);
    }
}
