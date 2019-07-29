// Copyright 2018-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _dspm_mat_h_
#define _dspm_mat_h_
#include <iostream>

/**
 * @brief   DSP matrix namespace
 *
 * DSP library matrix namespace.
 */
namespace dspm
{
/**
 * @brief   Matrix
 *
 * The Mat class provides basic matrix operations on single-precision floating point values.
 */
class Mat
{
public:
    /**
     * Constructor allocate internal buffer.
     * @param[in] rows: amount of matrix rows
     * @param[in] cols: amount of matrix columns
     */
    Mat(int rows, int cols);
    /**
    * Constructor use external buffer.
     * @param[in] data: external buffer with row-major matrix data
     * @param[in] rows: amount of matrix rows
     * @param[in] cols: amount of matrix columns
    */
    Mat(float *data, int rows, int cols);
    /**
     * Allocate matrix with undefined size.
     */
    Mat();
    virtual ~Mat();
    /**
     * Make copy of matrix.
     * @param[in] src: source matrix
     */
    Mat(const Mat &src);
    /**
     * Copy operator
     * 
     * @param[in] src: source matrix
     * 
     * @return
     *      - matrix copy
     */
    Mat &operator=(const Mat &src);

    bool ext_buff; /*!< Flag indicates that matrix use external buffer*/

    /**
     * Access to the matrix elements.
     * @param[in] row: row position
     * @param[in] col: column position
     * 
     * @return
     *      - element of matrix M[row][col]
     */
    inline float &operator()(int row, int col)
    {
        return data[row * this->cols + col];
    }
    /**
     * Access to the matrix elements.
     * @param[in] row: row position
     * @param[in] col: column position
     * 
     * @return
     *      - element of matrix M[row][col]
     */
    inline const float &operator()(int row, int col) const
    {
        return data[row * this->cols + col];
    }

    /**
     * += operator
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: source matrix
     * 
     * @return
     *      - result matrix: result += A
     */
    Mat &operator+=(const Mat &A);

    /**
     * += operator
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] C: constant
     * 
     * @return
     *      - result matrix: result += C
     */
    Mat &operator+=(float C);
    /**
     * -= operator
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: source matrix
     * 
     * @return
     *      - result matrix: result -= A
     */
    Mat &operator-=(const Mat &A);

    /**
     * -= operator
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] C: constant
     * 
     * @return
     *      - result matrix: result -= C
     */
    Mat &operator-=(float C);

    /**
     * *= operator
     * The operator use DSP optimized implementation of multiplication.
     * @param[in] A: source matrix
     * 
     * @return
     *      - result matrix: result -= A
     */
    Mat &operator*=(const Mat &A);
    /**
     * += with constant operator
     * The operator use DSP optimized implementation of multiplication.
     * @param[in] C: constant value
     * 
     * @return
     *      - result matrix: result *= C
     */
    Mat &operator*=(float C);
    /**
     * /= with constant operator
     * The operator use DSP optimized implementation of multiplication.
     * @param[in] C: constant value
     * 
     * @return
     *      - result matrix: result /= C
     */
    Mat &operator/=(float C);
    /**
     * ^= xor with constant operator
     * The operator use DSP optimized implementation of multiplication.
     * @param[in] C: constant value
     * 
     * @return
     *      - result matrix: result ^= C
     */
    Mat  operator^(int C);

    /**
     * Print matrix to the standard iostream.
     * @param[in] os: output stream
     * @param[in] m: matrix to print
     * 
     * @return
     *      - output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const Mat &m);
    /**
     * Fill the matrix from iostream.
     * @param[in] is: input stream
     * @param[in] m: matrix to fill
     */
    friend std::istream &operator>>(std::istream &is, Mat &m);

    /**
     * Swap two rows between each other.
     * @param[in] row1: position of first row
     * @param[in] row2: position of second row
     */
    void swapRows(int row1, int row2);
    /**
     * Matrix transpose.
     * Change rows and columns between each other.
     * 
     * @return
     *      - transposed matrix
     */
    Mat transpose();

    /**
     * Create identity matrix.
     * Create a square matrix and fill diagonal with 1.
     * 
     * @param[in] size: matrix size
     * 
     * @return
     *      - matrix [N]x[N] with 1 in diagonal
     */
    static Mat createIdentity(int size);
    /**
     * @brief   Solve the matrix
     *
     * Solve matrix. Find roots for the matrix A*x = b
     *
     * @param[in] A: matrix [N]x[N] with input coefficients
     * @param[in] b: vector [N]x[1] with result values
     *
     * @return
     *      - matrix [N]x[1] with roots
     */
    static Mat solve(Mat A, Mat b);
    /**
     * @brief   Band solve the matrix
     *
     * Solve band matrix. Find roots for the matrix A*x = b with bandwidth k.
     *
     * @param[in] A: matrix [N]x[N] with input coefficients
     * @param[in] b: vector [N]x[1] with result values
     * @param[in] k: upper bandwidth value
     *
     * @return
     *      - matrix [N]x[1] with roots
     */
    static Mat bandSolve(Mat A, Mat b, int k);
    /**
     * @brief   Solve the matrix
     *
     * Different way to solve the matrix. Find roots for the matrix A*x = y
     *
     * @param[in] A: matrix [N]x[N] with input coefficients
     * @param[in] y: vector [N]x[1] with result values
     *
     * @return
     *      - matrix [N]x[1] with roots
     */
    static Mat roots(Mat A, Mat y);

    /**
     * @brief   Dotproduct of two vectors
     *
     * The method returns dotproduct of two vectors
     *
     * @param[in] A: Input vector A Nx1
     * @param[in] B: Input vector B Nx1
     *
     * @return
     *      - dotproduct value
     */
    static float dotProduct(Mat A, Mat B);

    /**
     * @brief   Augmented matrices
     *
     * Augmented matrices
     *
     * @param[in] A: Input vector A MxN
     * @param[in] B: Input vector B MxK
     *
     * @return
     *      - Augmented matrix Mx(N+K)
     */
    static Mat augment(Mat A, Mat B);
    /**
     * @brief   Gaussian Elimination
     *
     * Gaussian Elimination of matrix
     *
     * @return
     *      - result matrix
     */
    Mat gaussianEliminate();

    /**
     * Row reduction for Gaussian elimination
     *
     * @return
     *      - result matrix
     */
    Mat rowReduceFromGaussian();

    /**
     * Find the inverse matrix
     *
     * @return
     *      - inverse matrix
     */
    Mat inverse();

    int rows; /*!< Amount of rows*/
    int cols; /*!< Amount of columns*/
    float *data; /*!< Buffer with matrix data*/
    int length; /*!< Total amount of data in data array*/

    static float eps; /*!< Max acceptable error epsilon*/
private:

    void allocate(); // Allocate buffer
    Mat expHelper(const Mat &m, int num);

public:    
    /**
     * + operator, sum of two matrices
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: Input matrix A
     * @param[in] B: Input matrix B
     * 
     * @return
     *     - result matrix A+B
    */
    friend Mat operator+(const Mat &A, const Mat &B);
    /**
     * + operator, sum of matrix with constant
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: Input matrix A
     * @param[in] C: Input constant
     * 
     * @return
     *     - result matrix A+C
    */
    friend Mat operator+(const Mat &A, float C);

    /**
     * - operator, subtraction of two matrices
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: Input matrix A
     * @param[in] B: Input matrix B
     * 
     * @return
     *     - result matrix A-B
    */
    friend Mat operator-(const Mat &A, const Mat &B);
    /**
     * - operator, sum of matrix with constant
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: Input matrix A
     * @param[in] C: Input constant
     * 
     * @return
     *     - result matrix A+C
    */
    friend Mat operator-(const Mat &A, float C);

    /**
     * * operator, multiplication of two matrices. 
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: Input matrix A
     * @param[in] B: Input matrix B
     * 
     * @return
     *     - result matrix A*B
    */
    friend Mat operator*(const Mat &A, const Mat &B);

    /**
     * * operator, multiplication of matrix with constant
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: Input matrix A
     * @param[in] C: floating point value
     * 
     * @return
     *     - result matrix A*B
    */
    friend Mat operator*(const Mat &A, float C);

    /**
     * * operator, multiplication of matrix with constant 
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] C: floating point value
     * @param[in] A: Input matrix A
     * 
     * @return
     *     - result matrix A*B
    */
    friend Mat operator*(float C, const Mat &A);

    /**
     * / operator, divide of matrix by constant 
     * The operator use DSP optimized implementation of multiplication.
     * 
     * @param[in] A: Input matrix A
     * @param[in] C: floating point value
     * 
     * @return
     *     - result matrix A*B
    */
    friend Mat operator/(const Mat &A, float C);

    /**
     * == operator, compare two matrices 
     * 
     * @param[in] A: Input matrix A
     * @param[in] B: Input matrix B
     * 
     * @return
     *      - true if matrices are the same
     *      - false if matrices are different 
    */
    friend bool operator==(const Mat &A, const Mat &B);
};


}
#endif //_dspm_mat_h_
