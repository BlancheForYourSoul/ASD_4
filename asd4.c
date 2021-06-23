#include <stdio.h>
#include <stdlib.h>
#define n1 0
#define n2 3
#define n3 0
#define n4 4
#define n 10

int rand_int(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int* create_array(int size){
    int *A = (int *)calloc(size, sizeof(int));
    return A;
}

int** create_2Darray(int size){
    int **A = (int **)malloc(size * sizeof(int *));
    for(int i = 0; i < size; i++){
        A[i] = (int *)calloc(size, sizeof(int));
    }
    return A;
}

void print_array(int* A, int size){
    for (int i = 0; i < size; i++){
        printf("%d ", A[i]);
    }
}

void print_2Darray(int** A, int size){
    for (int i = 0; i <  size; i++){
        for (int j = 0; j < size; j++){
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

int** get_matrix(int size, double k){
    int **A = create_2Darray(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            A[i][j] = k * rand_int(0, 2);
            if (A[i][j] < 1.0){
                A[i][j] = 0;
            } else {
                A[i][j] = 1;
            }
        }
    }
    return A;
}

int** get_simetric_matrix(int size, double k){
    int **A = create_2Darray(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            A[i][j] = k * rand_int(0, 2);
            if (A[i][j] >= 1.0){
                A[i][j] = 1;
                A[j][i] = 1;
            } else {
                A[i][j] = 0;
                A[j][i] = 0;
            }
        }
    }
    return A;
}

int** get_not_oriented_matrix(int**A, int size){
    int **res = create_2Darray(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            res[j][i] = A[i][j] * A[j][i];
        }
    }
    return res;
}

int** multiply_matrix(int** A, int** B, int size){
    int **res = create_2Darray(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            res[i][j] = 0;
            for (int k = 0; k < size; k++){
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return res;
}

int** sum_matrix(int** A, int** B, int size){
    int **res = create_2Darray(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            res[i][j] = A[i][j] + B[i][j];
        }
    }
    return res;
}

int** transpone_matrix(int** A, int size){
    int **res = create_2Darray(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            res[i][j] = A[j][i];
        }
    }
    return res;
}

int* get_not_oriented_degree(int** A, int size){
    int *res = create_array(size);
    for (int i = 0; i < size; i++){
        res[i] = 0;
        for (int j = 0; j < size; j++){
            if (A[i][j] != 0){
                res[i]++;
            }
        }
    }
    return res;
}

int** get_oriented_degree(int** A, int size){
    int **res = (int **)malloc(2 * sizeof(int *));
    for (int i = 0; i < 2; i++){
        res[i] = create_array(size);
    }
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (A[i][j] != 0){
                res[0][i]++;
            }
            if (A[j][i] != 0){
                res[1][i]++;
            }
        }
    }
    return res;
}

void get_hanged_isolated(int** A, int size){
    int **a = get_oriented_degree(A, size);
    int *hanged = create_array(size), *isolated = create_array(size), h_count = 0, i_count = 0;
    for (int i = 0; i < size; i++){
        if(a[0][i] == a[1][i]){
            if (a[0][i] == 1){
                h_count++;
                hanged[h_count-1] = i;
            } else if (a[0][i] == 0){
                i_count++;
                isolated[i_count-1] = i;
            }
        }
    }
    if (h_count > 0){
        printf("\nHanged: ");
        print_array(hanged, h_count);
    } else {
        printf("\nThere's no hanged");
    }
    if (i_count > 0){
        printf("\nIsolated: ");
        print_array(isolated, i_count);
    } else {
        printf("\nThere's no isolated");
    }
}

int is_regular(int** A, int size){
    int *a = get_not_oriented_degree(A, size);
    for (int i = 0; i < size-1; i++){
        if(a[i] != a[i+1]){
            printf("\nThe graph is irregular");
            return -1;
        }
    }
    printf("\nThe graph is regular, degree of it: %d", a[0]);
    return a[0];
}

int** accessibility(int** A, int size){
    int **res = create_2Darray(size);
    int **tmp = create_2Darray(size);
    res = sum_matrix(res, A, size);
    for (int i = 0; i < size-2; i++){
        tmp = multiply_matrix(res, A, size);
        res = sum_matrix(res, tmp, size);
    }
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (res[i][j] != 0){
                res[i][j] = 1;
            }
        }
    }
    return res;
}

int** get_strong_connection(int** A, int size){
    int **tmp = accessibility(A, size);
    int **res = create_2Darray(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            res[i][j] = tmp[i][j] * tmp[j][i];
        }
    }
    return res;
}

int main(){
    double k1 = 1.0 - n3*0.01 - n4*0.01 - 0.3;
    double k2 = 1.0 - n3*0.005 - n4*0.005 - 0.27;
    printf("k1: %lf\nk2: %lf\n", k1, k2);
    srand(0304);

    int **A = get_matrix(n, k1);
    printf("\nOriented graph#1:\n");
    print_2Darray(A, n);
    int **A1 = get_not_oriented_matrix(A, n);
    printf("\nNot oriented graph#1:\n");
    print_2Darray(A1, n);

    int **A_degree = get_oriented_degree(A, n);
    int *A1_degree = get_not_oriented_degree(A1, n);
    printf("\nDegrees of oriented graph:\n");
    printf("Out:\n");
    print_array(A_degree[0], n);
    printf("\nIn:\n");
    print_array(A_degree[1], n);
    printf("\nDegrees of not oriented graph:\n");
    print_array(A1_degree, n);

    printf("\nOriented:");
    int A_reg = is_regular(A, n);
    get_hanged_isolated(A, n);
    printf("\n\nAccessibility matrix:\n");
    int **A_acc = accessibility(A, n);
    print_2Darray(A_acc, n);
    printf("\n\nStrong connections matrix:\n");
    int **A_strcon = get_strong_connection(A, n);
    print_2Darray(A_strcon, n);
    printf("\n");
    printf("\n");

    printf("\nNot oriented:");
    int A1_reg = is_regular(A1, n);
    get_hanged_isolated(A1, n);
    printf("\n\nAccessibility matrix:\n");
    int **A1_acc = accessibility(A1, n);
    print_2Darray(A1_acc, n);
    printf("\n\nStrong connections matrix:\n");
    int **A1_strcon = get_strong_connection(A1, n);
    print_2Darray(A1_strcon, n);

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

    printf("\n2222222222222222222222222222222222222222222222222:\n");

    int **B = get_matrix(n, k2);
    printf("\nOriented graph#2:\n");
    print_2Darray(B, n);
    int **B1 = get_not_oriented_matrix(B, n);
    printf("\nNot oriented graph#2:\n");
    print_2Darray(B1, n);

    int **B_degree = get_oriented_degree(B, n);
    int *B1_degree = get_not_oriented_degree(B1, n);
    printf("\nDegrees of oriented graph:\n");
    printf("Out:\n");
    print_array(B_degree[0], n);
    printf("\nIn:\n");
    print_array(B_degree[1], n);
    printf("\nDegrees of not oriented graph:\n");
    print_array(B1_degree, n);

    printf("\nOriented:");
    int B_reg = is_regular(B, n);
    get_hanged_isolated(B, n);
    printf("\n\nAccessibility matrix:\n");
    int **B_acc = accessibility(B, n);
    print_2Darray(B_acc, n);
    printf("\n\nStrong connections matrix:\n");
    int **B_strcon = get_strong_connection(B, n);
    print_2Darray(B_strcon, n);
    printf("\n");
    printf("\n");

    printf("\nNot oriented:");
    int B1_reg = is_regular(B1, n);
    get_hanged_isolated(B1, n);
    printf("\n\nAccessibility matrix:\n");
    int **B1_acc = accessibility(B1, n);
    print_2Darray(B1_acc, n);
    printf("\n\nStrong connections matrix:\n");
    int **B1_strcon = get_strong_connection(B1, n);
    print_2Darray(B1_strcon, n);



    return 0;
}