/**************************************************
 *                  Includes                      *
 **************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**************************************************
 *                  Macros                        *
 **************************************************/

#define N               (1024U)          /* Matrix size (N x N ) */
#define BLOCK_SIZE      (64U)           /* Block size for cache-aware method */
#define ITER            (5U)            /* Number of iterations for averaging */
#define SYSTEM_SUCCESS  (0)
#define SYSTEM_FAILURE  (-1)

/**************************************************
 *                  Global Variables              *
 **************************************************/

uint64_t A[N][N] __attribute__((aligned(64)));
uint64_t B[N][N] __attribute__((aligned(64)));
uint64_t naiveMat[N][N] __attribute__((aligned(64)));
uint64_t blockMat[N][N] __attribute__((aligned(64)));
uint64_t transMat[N][N] __attribute__((aligned(64)));
uint64_t BT[N][N] __attribute__((aligned(64)));

/**************************************************
 *          Function Definitions                  *
 **************************************************/

/* Initialize matrix with random values */
void init_matrix(uint64_t mat[N][N]) 
{
    for (uint64_t row = 0; row < N; row++)
    {
        for (uint64_t col = 0; col < N; col++)
        {
            mat[row][col] = (uint64_t)(rand() % 1000);
        }
    }
}

/* Naive matrix multiplication */
void naive_MatMul(uint64_t A[N][N], uint64_t B[N][N], uint64_t C[N][N]) 
{
    for (uint64_t row = 0; row < N; row++)
    {
        for (uint64_t col = 0; col < N; col++) 
        {
            C[row][col] = 0;
            for (uint64_t dot = 0; dot < N; dot++)
            {
                C[row][col] += A[row][dot] * B[dot][col];
            }
        }
    }
}

/* Cache-aware Blocked method of matrix multiplication */
void blocked_MatMul(uint64_t A[N][N], uint64_t B[N][N], uint64_t C[N][N]) 
{
    for(uint64_t row = 0U; row < N; row+=BLOCK_SIZE)
    {
        for(uint64_t col = 0U; col < N; col+=BLOCK_SIZE)
        {
            for(uint64_t blockRow = row; blockRow < row + BLOCK_SIZE && blockRow < N; blockRow++)
            {
                for(uint64_t blockCol = col; blockCol < col + BLOCK_SIZE && blockCol <N; blockCol++)
                {
                    C[blockRow][blockCol] = 0;
                    for(uint64_t dot = 0U; dot < N; dot++)
                    {
                        C[blockRow][blockCol] += A[blockRow][dot] * B[dot][blockCol];
                    }
                }
            }
        }
    }
}

/* Transpose method of matrix multiplication */
void trans_MatMul(uint64_t A[N][N], uint64_t B[N][N], uint64_t C[N][N]) 
{
    for (uint64_t row = 0; row < N; row++)
    {
        for (uint64_t col = 0; col < N; col++) 
        {
            C[row][col] = 0;
            for (uint64_t dot = 0; dot < N; dot++)
            {
                C[row][col] += A[row][dot] * B[col][dot];
            }
        }
    }
}

void transpose(uint64_t A[N][N], uint64_t B[N][N]) 
{
    for (uint64_t row = 0; row < N; row++)
    {
        for (uint64_t col = 0; col < N; col++) 
        {
            B[col][row] = A[row][col];
        }
    }
}

int8_t comp_Result(uint64_t A[N][N], uint64_t B[N][N], uint64_t C[N][N]) 
{
    int8_t status = SYSTEM_SUCCESS;
    for (uint64_t row = 0; row < N; row++)
    {
        for (uint64_t col = 0; col < N; col++) 
        {
            /* Compare B & A*/
            if(B[row][col] != A[row][col])
            {
                printf("Error: Result mismatch at B[%lu][%lu]: %lu != %lu\n", row, col, B[row][col], A[row][col]);
                status = SYSTEM_FAILURE;
                break;
            }

            /* Compare C & A*/
            if(C[row][col] != A[row][col])
            {
                printf("Error: Result mismatch at C[%lu][%lu]: %lu != %lu\n", row, col, C[row][col], A[row][col]);
                status += SYSTEM_FAILURE;
            }
        }
    }

    return status;
}

int main() 
{
    int8_t status = SYSTEM_SUCCESS;
    uint64_t iter = 5U;
    double avgSum;
    double averageTime = 0.0;

    srand(time(NULL));

    /* Compute naive method */
    printf("Starting Naive Method multiplication...\n");
    avgSum = 0U;
    for(uint8_t iter = 0U; iter < ITER; iter++)
    {
        /* Reset matrices for each iter to flush already cached dataset */
        init_matrix(A);
        init_matrix(B);

        clock_t startNaive = clock();
        naive_MatMul(A, B, naiveMat);
        clock_t endNaive = clock();
        double timeNaive = ((double)(endNaive - startNaive)) / CLOCKS_PER_SEC;
        avgSum += timeNaive;
        printf("Naive Method Iter:%d, Time: %.4f seconds\n", iter, timeNaive);
    }
    averageTime = avgSum / ITER;
    printf("Average time for Naive method: %.4f seconds\n\n", averageTime);

    /* Compute blocked method */
    printf("Starting Blocked Method multiplication...\n");
    avgSum = 0U;
    for(uint8_t iter = 0U; iter < ITER; iter++)
    {
        /* Reset matrices for each iter to flush already cached dataset */
        init_matrix(A);
        init_matrix(B);

        clock_t startBlock = clock();
        blocked_MatMul(A, B, blockMat);
        clock_t endBlock = clock();
        double timeBlock = (double)(endBlock - startBlock) / CLOCKS_PER_SEC;
        printf("Blocked Method Iter:%d, Time: %.4f seconds\n", iter, timeBlock);
        avgSum += timeBlock;
    }
    averageTime = avgSum / ITER;
    printf("Average time for Blocked method: %.4f seconds\n\n", averageTime);

    /* Compute Transpose method */
    printf("Starting Transpose Method multiplication...\n");
    avgSum = 0U;
    for(uint8_t iter = 0U; iter < ITER; iter++)
    {
        /* Reset matrices for each iter to flush already cached dataset */
        init_matrix(A);
        init_matrix(B);

        /* Transpose B matrix */
        clock_t startTrans = clock();
        transpose(B, BT); 
        trans_MatMul(A, BT, transMat);
        clock_t endTrans = clock();
        double timeTrans = (double)(endTrans - startTrans) / CLOCKS_PER_SEC;
        printf("Transpose Method Iter:%d, Time: %.4f seconds\n", iter, timeTrans);
        avgSum += timeTrans;
    }
    averageTime = avgSum / ITER;
    printf("Average time for Transpose method: %.4f seconds\n\n", averageTime);

    return 0;
}
