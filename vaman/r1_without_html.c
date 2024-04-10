#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "matfun.h"
#include "geofun.h"

            
int main() {
    int p1, q1, p2, q2, p3, q3, row = 2, col = 1;
    
    printf("Enter point I's coordinates here:");
    scanf("%d %d", &p1, &q1);
    printf("Enter point J's coordinates here:");
    scanf("%d %d", &p2, &q2);
    printf("Enter point K's coordinates here:");
    scanf("%d %d", &p3, &q3);
    
    double **I, **J, **K, **m1, **m2, **m3, **L, **N, **m3_t, **m1_t, **R0, **R1, **R; 
    I = createMat(row, col);
    J = createMat(row, col);
    K = createMat(row, col);

    I[0][0] = p1;
    I[1][0] = q1;
    J[0][0] = p2;
    J[1][0] = q2;
    K[0][0] = p3;
    K[1][0] = q3;

    // Perform matrix operations
    m1 = Matsub(I, J, row, col);
    m2 = Matsub(J, K, row, col);
    m3 = Matsub(K, I, row, col);
    N = transposeMat(combineMat(m3, m1, row, col), row, row);
    m3_t = transposeMat(m3, row, col);
    m1_t = transposeMat(m1, row, col);
    R0 = Matmul(m3_t, J, col, row, col);
    R1 = Matmul(m1_t, K, col, row, col);
    R = createMat(2, 1);
    R[0][0] = R0[0][0];
    R[1][0] = R1[0][0];

    // Solving the linear equations
    L = createMat(2, 1);
    L[1][0] = (-1) * (R[0][0] * N[1][0] - N[0][0] * R[1][0]) / Matdet(N);
    L[0][0] = (R[0][0] * N[1][1] - N[0][1] * R[1][0]) / Matdet(N);
    printf("The sources of linear equation are L1,L2: %lf, %lf\n\n", L[1][0], L[0][0]);

    // Perpendicular bisector
    double **sij, **sjk, **ski, **midpoint_IJ, **midpoint_JK, **midpoint_KI;
    sij = Matsub(I, J, row, col);
    sjk = Matsub(J, K, row, col);
    ski = Matsub(K, I, row, col);
    midpoint_IJ = normVec(sij);
    midpoint_JK = normVec(sjk);
    midpoint_KI = normVec(ski);
    printf("Perpendicular Bisectors of IJ, JK, KI are : {%lf, %lf}, {%lf, %lf}, {%lf, %lf}\n\n", midpoint_IJ[0][0], midpoint_IJ[1][0], midpoint_JK[0][0], midpoint_JK[1][0], midpoint_KI[0][0], midpoint_KI[1][0]);
 
    // Medians
    double **A, **B, **C, **side_ij, **side_jk, **side_ki;
    A = Matsec(J, K, 2, 1.0);
    B = Matsec(K, I, 2, 1.0);
    C = Matsec(I, J, 2, 1.0);
    side_ij = Matsub(I, A, row, col);
    side_jk = Matsub(J, B, row, col); 
    side_ki = Matsub(K, C, row, col);  
    double sideIJ = Matnorm(side_ij, row);  
    double sideJK = Matnorm(side_jk, row);
    double sideKI = Matnorm(side_ki, row);
    printf("Length of Medians IA, JB, KC are : ");
    printf("%lf, %lf, %lf\n\n", sideIJ, sideJK, sideKI);

    // Angular bisector
    int rows = 2, cols = 1;
    double **G, **Aa, **Ba, **Ca, **difference_IJ, **difference_JK, **difference_IK;
    G = createMat(rows, cols);                           
    difference_IJ = Matsub(J, I, rows, cols); 
    difference_JK = Matsub(K, J, rows, cols); 
    difference_IK = Matsub(K, I, rows, cols);
    double distance_IJ = Matnorm(difference_IJ, rows);                        
    double distance_JK = Matnorm(difference_JK, rows);
    double distance_IK = Matnorm(difference_IK, rows); 
    double i = distance_IJ;
    double k = distance_JK;
    double j = distance_IK;
    printf("Dismensions of the sides IJ, JK, IK are : %lf, %lf, %lf\n\n", i, j, k);

    double l1 = (i + k - j) / 2;
    double l2 = (i + j - k) / 2;
    double l3 = (k + j - i) / 2;
    Aa = Matscale(Matadd(Matscale(K, 2, 1, l1), Matscale(J, 2, 1, l2), 2, 1), 2, 1, 1.0 / (l1 + l2));
    Ba = Matscale(Matadd(Matscale(I, 2, 1, l2), Matscale(K, 2, 1, l3), 2, 1), 2, 1, 1.0 / (l2 + l3));
    Ca = Matscale(Matadd(Matscale(J, 2, 1, l3), Matscale(I, 2, 1, l1), 2, 1), 2, 1, 1.0 / (l3 + l1));
    printf("Angular Bisectors  Aa, Ba, Ca : {%lf, %lf}, {%lf, %lf}, {%lf, %lf}\n\n", Aa[0][0], Aa[1][0], Ba[0][0], Ba[1][0], Ca[0][0], Ca[1][0]);

    double **temp_1 = Matscale(I, 2, 1, i);
    double **temp_2 = Matscale(J, 2, 1, j);
    double **temp_3 = Matscale(K, 2, 1, k);
    double **eigenvalues = Matadd(Matadd(temp_1, temp_2, 2, 1), temp_3, 2, 1);
    double eigendenominator = i + j + k;

    G[0][0] = eigenvalues[0][0] / eigendenominator;
    G[1][0] = eigenvalues[1][0] / eigendenominator;                                 
    printf("Incenter of Triangle IJK : ");       
    printf("%lf, %lf\n\n", G[0][0], G[1][0]);

    return 0;
}


