#include <stdio.h>
#include <stdlib.h>

struct my_matrix_struct {
	int numRow;
	int numCol;
	double ** ptr_arr;
};

void allocateMatrix(struct my_matrix_struct * mms, int nR, int nC) {
	mms->numRow = nR;
	mms->numCol = nC; 
	int i;
	mms->ptr_arr = (double **)malloc(nR * sizeof(double *));
	for (i = 0; i < nR; i++) {
		mms->ptr_arr[i] = (double *)malloc(nC * sizeof(double));
	}
}

// return 0 if OK, -1 if failed
int fillMatrixFromStdin(struct my_matrix_struct * mms) {
	int nR = mms->numRow;
	int nC = mms->numCol;
	int count = 0;
	double input;
	int i, j;
	for (i = 0; i < nR; i++) {
		for (j = 0; j < nC && scanf("%lf", &input) == 1; j++) {
			mms->ptr_arr[i][j] = input;
			count++;
		}
	}
	if (count != nR * nC) return -1;
	else return 0;
}

// return 0 if OK, -1 if failed
int matrixProduct(struct my_matrix_struct * mmsC, struct my_matrix_struct * mmsA, struct my_matrix_struct * mmsB) {
	int nR_C = mmsC->numRow;
	int nC_C = mmsC->numCol;
	int nR_B = mmsB->numRow;
	int nC_B = mmsB->numCol;
	int nR_A = mmsA->numRow;
	int nC_A = mmsA->numCol;

	if (nR_C != nR_A || nC_C != nC_B || nC_A != nR_B) return -1;

	int i, j, k;
	double sum;
	double ** A = mmsA->ptr_arr;
	double ** B = mmsB->ptr_arr;
	double ** C = mmsC->ptr_arr;

	for (i = 0; i < nR_C; i++) {
		for (k = 0; k < nC_C; k++) {
			sum = 0;
			for (j = 0; j < nC_A; j++) {
				sum += A[i][j] * B[j][k];
			}
			C[i][k] = sum;
		}
	}

	return 0;
}

void printMatrix(struct my_matrix_struct * mms) {
	int nR = mms->numRow;
	int nC = mms->numCol;
	int i, j;
	for (i = 0; i < nR; i++) {
		for (j = 0; j < nC; j++) {
			printf("%.2f ", mms->ptr_arr[i][j]);
		}
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	int m = (int)strtol(argv[1], NULL, 10);
	int n = (int)strtol(argv[2], NULL, 10);
	int p = (int)strtol(argv[3], NULL, 10);

	// allocate spaces for A, B and C
	struct my_matrix_struct A;
	struct my_matrix_struct B;
	struct my_matrix_struct C;

	allocateMatrix(&A, m, n);
	allocateMatrix(&B, n, p);
	allocateMatrix(&C, m, p);

	// read in data to A and B
	int countA = fillMatrixFromStdin(&A);
	if (countA == -1) {
		fprintf(stderr, "fill A failed\n");
		exit(1);
	}
	int countB = fillMatrixFromStdin(&B);
	if (countB == -1) {
		fprintf(stderr, "fill B failed\n");
		exit(1);
	}

	// calculate C
	int ret_mp = matrixProduct(&C, &A, &B);
	if (ret_mp == -1) {
		fprintf(stderr, "matrix structure does not match to calculate product\n");
		exit(1);
	}
	printMatrix(&C);

	return 0;
}
