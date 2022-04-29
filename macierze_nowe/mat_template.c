#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A) {
    int address = row * cols + col;
    return A[address];
}

void set(int cols, int row, int col, double *A, double value) {
    int address = row * cols + col;
    A[address] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB) {
    int rowsB = colsA, rowsAB = rowsA, colsAB = colsB;
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            double temp_element = 0;
            for (int k = 0; k < colsA; ++k) {
                double a = get(colsA, i, k, A);
                double b = get(colsB, k, j, B);
                temp_element += a * b;
            }
            set(colsAB, i, j, AB, temp_element);
        }
    }
}

void read_mat(int rows, int cols, double *t) {
    double a;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            scanf("%lf", &a);
            set(cols, i, j, t, a);
        }
    }
}

void print_mat(int rows, int cols, double *t) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%lf", get(cols, i, j, t));
            if (j != cols - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int read_char_lines(char *tab[]);

void write_char_line(char *tab[], int n);

void delete_lines(char *tab[], int line_count);

int read_dbl_lines_v1(double *ptr_tab[]) {
    char curr;
    char string[TAB_SIZE]; string[0] = '\0';
    char *num;
    double arr[TAB_SIZE][TAB_SIZE];
    int arr_row = 0, arr_index = 0, tab_index = 0;

    while ((curr = (char)getchar()) != EOF) {
        if (curr == '\n') {
            num = strtok(string, " ");
            while (num != NULL) {
                arr[arr_row][arr_index++] = atof(num);
                num = strtok(NULL, " ");
            }
            arr[arr_row][TAB_SIZE - 1] = arr_index;
            ptr_tab[tab_index++] = arr[arr_row++];
            string[0] = '\0';
        } else {
            strncat(string, &curr, 1);
        }
    }
}

void write_dbl_line_v1(double *ptr_tab[], int n) {
    for (int i = 0; i < ptr_tab[n - 1][TAB_SIZE - 1]; ++i) {
        printf("%lf ", ptr_tab[n - 1][i]);
    }
}

int main(void) {
	int to_do;

	scanf ("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA,colsA,rowsB,colsB;
	char *char_lines_table[TAB_SIZE];
	double series_table[TAB_SIZE];
	double *ptr_table[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d",&rowsA,&colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d",&rowsB,&colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA,colsA,colsB,A,B,C);
			print_mat(rowsA,colsB,C);
			break;
		case 2:
			scanf("%d",&n);
			ptr_table[0] = series_table;
			lines_counter = read_dbl_lines_v1(ptr_table);
			write_dbl_line_v1(ptr_table,n);
			break;
//		case 3:
//			scanf("%d", &n);
//			lines_counter = read_char_lines(char_lines_table);
//			write_char_line(char_lines_table,n);
//			delete_lines(char_lines_table,lines_counter);
//			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
