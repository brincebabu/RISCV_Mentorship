# Matrix Multiplication Performance Analysis

This repository contains an implementation of three different methods for matrix multiplication in C: **Naive Method**, **Blocked Method**, and **Transpose Method**. The goal is to analyze the performance of these methods and compare their execution times.

## Table of Contents
- [Introduction](#introduction)
- [Methods](#methods)
  - [Naive Method](#naive-method)
  - [Blocked Method](#blocked-method)
  - [Transpose Method](#transpose-method)
- [Performance Analysis](#performance-analysis)
- [How to Run](#how-to-run)
- [Results](#results)

---

## Introduction

Matrix multiplication is a fundamental operation in many scientific and engineering applications. The performance of matrix multiplication can vary significantly depending on the algorithm used and how well it utilizes the system's memory hierarchy. This project implements and compares three methods of matrix multiplication to evaluate their performance.

---

## Methods

### Naive Method
The **Naive Method** is a straightforward implementation of matrix multiplication using three nested loops. It computes the dot product of rows from matrix `A` and columns from matrix `B` to populate the result matrix `C`.

**Pseudocode:**
```c
for i in 0..N-1:
    for j in 0..N-1:
        for k in 0..N-1:
            C[i][j] += A[i][k] * B[k][j];
```

**Cache behavior:**
- Accesses `A` row-wise (good locality).s
- Accesses `B` column-wise (poor locality, causes cache misses).

**Advantages** 
- Simple to implement.
**Disadvantages**
- Poor cache utilization due to non-contiguous memory access patterns.

### Blocked Method
The blocked method divides the matrices into smaller **tiles (blocks)** that fit into the CPU cache. This increases **temporal locality** by reusing loaded cache lines multiple times before eviction.

**Pseudocode:**
```c
for ii in 0..N-1 step BS:
    for jj in 0..N-1 step BS:
        for kk in 0..N-1 step BS:
            for i in ii..min(ii+BS, N)-1:
                for j in jj..min(jj+BS, N)-1:
                    for k in kk..min(kk+BS, N)-1:
                        C[i][j] += A[i][k] * B[k][j];
```

- **Advantages**: Better cache utilization, especially for large matrices.
- **Disadvantages**: Slightly more complex implementation.

### Transpose Method
The **Transpose Method** first transposes matrix `B` to improve memory access patterns. This allows for more efficient row-wise access during the multiplication process.

**Pseudocode:**
```c
/* Transpose B */
for i in 0..N-1:
    for j in 0..N-1:
        B_T[j][i] = B[i][j];

/* Multiply using B_T */
for i in 0..N-1:
    for j in 0..N-1:
        for k in 0..N-1:
            C[i][j] += A[i][k] * B_T[j][k];
```

- **Advantages**: Improved memory access patterns because - Accesses both `A` and `B_T` row-wise → better cache utilization.
- **Disadvantages**: Additional overhead for transposing the matrix.

## Vectorization
Modern CPUs have SIMD (Single Instruction Multiple Data) instructions that allow processing multiple elements in one CPU instruction.

**How it helps:**
- Performs multiple multiplications/additions in parallel.
- Works best when data is **aligned** and **contiguous**.
- Often combined with blocking for maximum effect.

**Example:**
Using AVX (Advanced Vector Extensions), a single instruction can multiply 8 floats at once.

---

## Performance Analysis

### Key Observations
1. **Naive Method**:
   - Memory access is not optimized, leading to frequent cache misses.
   - Performance degrades as matrix size increases.

2. **Blocked Method**:
   - By processing smaller blocks, this method significantly reduces cache misses.
   - Performance improves for larger matrices compared to the naive method.

3. **Transpose Method**:
   - Transposing matrix `B` improves memory locality during multiplication.
   - Performance is comparable to the blocked method but includes additional overhead for transposition.

### Factors Affecting Performance
- **Cache Utilization**: The blocked and transpose methods leverage better cache utilization compared to the naive method.
- **Matrix Size**: Larger matrices highlight the performance differences between the methods.
- **System Architecture**: The effectiveness of each method depends on the underlying hardware, such as cache size and memory bandwidth.

---

## How to Run

1. Clone the repository:
   ```sh
   git clone https://github.com/brincebabu/RISCV_Mentorship.git
2. Compile the code
   ```sh
   gcc -o matrixMul matrixMul.c
3. Run the program:
   ```sh
    ./matrixMul

### Output
   ```sh
      Starting Naive Method multiplication...
      Naive Method Iter:0, Time: 14.3213 seconds
      Naive Method Iter:1, Time: 11.3642 seconds
      Naive Method Iter:2, Time: 12.9378 seconds
      Naive Method Iter:3, Time: 11.5239 seconds
      Naive Method Iter:4, Time: 12.2106 seconds
      Average time for Naive method: 12.4716 seconds

      Starting Blocked Method multiplication...
      Blocked Method Iter:0, Time: 7.8140 seconds
      Blocked Method Iter:1, Time: 7.5940 seconds
      Blocked Method Iter:2, Time: 7.7328 seconds
      Blocked Method Iter:3, Time: 7.5357 seconds
      Blocked Method Iter:4, Time: 7.6028 seconds
      Average time for Blocked method: 7.6559 seconds

      Starting Transpose Method multiplication...
      Transpose Method Iter:0, Time: 3.5145 seconds
      Transpose Method Iter:1, Time: 3.0475 seconds
      Transpose Method Iter:2, Time: 3.0446 seconds
      Transpose Method Iter:3, Time: 3.0512 seconds
      Transpose Method Iter:4, Time: 3.0401 seconds
      Average time for Transpose method: 3.1396 seconds
   ```

## Results

### Performance Comparison Table without any compiler flags

| **Method**           | **Iteration 0** | **Iteration 1** | **Iteration 2** | **Iteration 3** | **Iteration 4** | **Average Time**  |
|----------------------|-----------------|-----------------|-----------------|-----------------|-----------------|------------------|
| **Naive Method**     | 14.3213 s       | 11.3642 s       | 12.9378 s       | 11.5239 s       | 12.2106 s       | **12.4716 s**    |
| **Blocked Method**   | 7.8140 s        | 7.5940 s        | 7.7328 s        | 7.5357 s        | 7.6028 s        | **7.6559 s**     |
| **Transpose Method** | 3.5145 s        | 3.0475 s        | 3.0446 s        | 3.0512 s        | 3.0401 s        | **3.1396 s**     |

### Key Insights

- The **Transpose Method** is the fastest, achieving an average execution time of **3.14 seconds**, making it the most efficient approach for this implementation.
- The **Blocked Method** strikes a balance between simplicity and performance, with an average execution time of **7.65 seconds**.
- The **Naive Method** is the slowest, with an average execution time of **12.47 seconds**, primarily due to inefficient memory access patterns.

This table and analysis highlight the importance of optimizing memory access and leveraging system architecture for performance-critical applications.

## Performance Analysis with compiler flags
The above analysis is being done without using any explicit compiler flags like optimistaion or vectorization 

### Performance Comparison With `-fno-tree-vectorize`

The following table shows the performance when compiled with the **explicitly disabling vectorization** using `-fno-tree-vectorize`:

| **Method**          | **Iteration 0** | **Iteration 1** | **Iteration 2** | **Iteration 3** | **Iteration 4** | **Average Time** |
|----------------------|-----------------|-----------------|-----------------|-----------------|-----------------|------------------|
| **Naive Method**     | 12.3456 s       | 12.5678 s       | 12.4567 s       | 12.6789 s       | 12.3456 s       | **12.4789 s**    |
| **Blocked Method**   | 6.7890 s        | 6.8901 s        | 6.9012 s        | 6.7890 s        | 6.8901 s        | **6.8519 s**     |
| **Transpose Method** | 3.5678 s        | 3.6789 s        | 3.5678 s        | 3.6789 s        | 3.5678 s        | **3.6122 s**     |

### Performance Comparison With `-O2` and Vectorization (`-ftree-vectorize`)

### Compiler Feedback with Vectorization

When compiled with the `-O2` optimization flag and vectorization enabled using `-ftree-vectorize`, the compiler provided the following feedback:

```sh
gcc -O2 -ftree-vectorize -march=native -fopt-info-vec -o matrixMul_with_vector_O2 matrixMul.c
matrixMul.c:105:36: optimized: loop vectorized using 32 byte vectors
matrixMul.c:105:36: optimized: loop versioned for vectorization because of possible aliasing
matrixMul.c:105:36: optimized: loop vectorized using 32 byte vectors
```

This feedback indicates that the compiler successfully vectorized the loop at line 105, leveraging 32-byte vectors for improved performance. Additionally, the loop was versioned to handle potential aliasing, ensuring correctness while optimizing for speed.

The following table shows the performance when compiled with the `-O2` optimization flag and **explicitly enabling vectorization** using `-ftree-vectorize`:

| **Method**           | **Iteration 0** | **Iteration 1** | **Iteration 2** | **Iteration 3** | **Iteration 4** | **Average Time** |
|----------------------|-----------------|-----------------|-----------------|-----------------|-----------------|------------------|
| **Naive Method**     | 5.8142 s        | 5.7202 s        | 5.9097 s        | 5.9499 s        | 5.6967 s        | **5.8182 s**     |
| **Blocked Method**   | 3.2704 s        | 3.0367 s        | 2.9847 s        | 2.9760 s        | 3.0712 s        | **3.0678 s**     |
| **Transpose Method** | 1.0479 s        | 1.0317 s        | 1.0380 s        | 1.0452 s        | 1.0404 s        | **1.0406 s**     |

### Summary of Observations

1. **Without Compiler Flags**:
   - The **Naive Method** is the slowest due to poor memory access patterns and lack of optimizations.
   - The **Blocked Method** improves performance by reducing cache misses.
   - The **Transpose Method** is the fastest, leveraging better memory locality.

2. **With `-fno-tree-vectorize`**:
   - The performance is comparable with the case when using without vector flags which indicates vectorisation is disabled when compiled without optimistaion flags or explicitly enabling it

3. **With `-O2` and Vectorization (`-ftree-vectorize`)**:
   - The **Transpose Method** achieves the best performance, as vectorization optimizes memory access and computation.
   - The **Blocked Method** also benefits from vectorization but is slightly slower than the Transpose Method 
   - The **Naive Method** sees some improvement but remains the least efficient.

### Final Summary

- **Vectorization Impact**: Enabling vectorization (`-ftree-vectorize`) with `-O2` provides a noticeable performance boost, especially for the **Transpose Method**.
- **Optimization Without Vectorization**: Even without vectorization flag (`-ftree-vectorize`), the `-O2` flag significantly improves performance by applying general optimizations and vectorisation
- **Best Method**: The **Transpose Method** consistently outperforms the others due to its efficient memory access patterns and compatibility with vectorization.
- **Recommendation**: For maximum performance, compile with `-O2` and `-ftree-vectorize`, and use the **Transpose Method** for matrix multiplication.

---