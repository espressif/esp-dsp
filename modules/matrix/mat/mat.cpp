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

#include <stdexcept>
#include <string.h>
#include "mat.h"
#include "esp_log.h"

#include "dspm_mult.h"


using std::ostream;
using std::istream;
using std::endl;

namespace dspm
{

float Mat::eps = 1e-10;

Mat::Mat(int rows, int cols)
{
    ESP_LOGD("Mat", "Mat(%i, %i)", rows, cols);
    this->rows = rows;
    this->cols = cols;
    allocate();
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            this->mdata[i * this->cols + j] = 0;
        }
    }
}

Mat::Mat(float *data, int rows, int cols)
{
    ESP_LOGD("Mat", "Mat(data, %i, %i)", rows, cols);
    this->ext_buff = true;
    this->rows = rows;
    this->cols = cols;
    this->mdata = data;
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            this->mdata[i * this->cols + j] = data[i * this->cols + j];
        }
    }
}


Mat::Mat()
{
    this->rows = 1;
    this->cols = 1;
    ESP_LOGD("Mat", "Mat()");

    allocate();
    this->mdata[0] = 0;
}

Mat::~Mat()
{
    ESP_LOGD("Mat", "~Mat(%i, %i), ext_buff=%i, mdata=0x%8.8x", this->rows, this->cols, this->ext_buff, (uint32_t)this->mdata);
    if (false == this->ext_buff) {
        delete mdata;
    }
}

Mat::Mat(const Mat &m)
{
    this->rows = m.rows;
    this->cols = m.cols;

    allocate();
    memcpy(this->mdata, m.mdata, this->rows * this->cols * sizeof(float));
}

Mat &Mat::operator=(const Mat &m)
{
    if (this == &m) {
        return *this;
    }

    if (this->rows != m.rows || this->cols != m.cols) {
        if (!this->ext_buff) {
            delete this->mdata;
        }
        this->ext_buff = false;
        this->rows = m.rows;
        this->cols = m.cols;
        allocate();
    }
    memcpy(this->mdata, m.mdata, this->rows * this->cols * sizeof(float));
    return *this;
}

Mat &Mat::operator+=(const Mat &m)
{
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols ; j++) {
            this->mdata[i * this->cols + j] += m(i, j);
        }
    }
    return *this;
}

Mat &Mat::operator+=(float C)
{
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols ; j++) {
            this->mdata[i * this->cols + j] += C;
        }
    }
    return *this;
}

Mat &Mat::operator-=(const Mat &m)
{
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->mdata[i * this->cols + j] -= m(i, j);
        }
    }
    return *this;
}

Mat &Mat::operator-=(float C)
{
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols ; j++) {
            this->mdata[i * this->cols + j] -= C;
        }
    }
    return *this;
}

Mat &Mat::operator*=(const Mat &m)
{
    Mat temp = *this;
    dspm_mult_f32(temp.mdata, m.mdata, this->mdata, temp.rows, temp.cols, m.cols);
    return (*this);
}

Mat &Mat::operator*=(float num)
{
    for (int i = 0; i < this->rows * this->cols; i++) {
        this->mdata[i] *= num;
    }
    return *this;
}

Mat &Mat::operator/=(float num)
{
    for (int i = 0; i < this->rows * this->cols; i++) {
        this->mdata[i] /= num;
    }
    return *this;
}

Mat Mat::operator^(int num)
{
    Mat temp(*this);
    return expHelper(temp, num);
}

void Mat::swapRows(int r1, int r2)
{
    for (int i = 0; i < this->cols; i++) {
        float temp = this->mdata[r1 * this->cols + i];
        this->mdata[r1 * this->cols + i] = this->mdata[r2 * this->cols + i];
        this->mdata[r2 * this->cols + i] = temp;
    }
}

Mat Mat::transpose()
{
    Mat ret(this->cols, this->rows);
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            ret(j, i) = this->mdata[i * this->cols + j];
        }
    }
    return ret;
}

Mat Mat::createIdentity(int size)
{
    Mat temp(size, size);
    for (int i = 0; i < temp.rows; ++i) {
        for (int j = 0; j < temp.cols; ++j) {
            if (i == j) {
                temp(i, j) = 1;
            } else {
                temp(i, j) = 0;
            }
        }
    }
    return temp;
}

Mat Mat::solve(Mat A, Mat b)
{
    // Gaussian elimination
    for (int i = 0; i < A.rows; ++i) {
        if (A(i, i) == 0) {
            // pivot 0 - error
            ESP_LOGW("Mat", "Error: the coefficient matrix has 0 as a pivot. Please fix the input and try again.");
            Mat err_result(0, 0);
            return err_result;
        }
        for (int j = i + 1; j < A.rows; ++j) {
            for (int k = i + 1; k < A.cols; ++k) {
                A(j, k) -= A(i, k) * (A(j, i) / A(i, i));
                if ((A(j, k) < eps) && (A(j, k) > -1 * eps)) {
                    A(j, k) = 0;
                }
            }
            b(j, 0) -= b(i, 0) * (A(j, i) / A(i, i));
            if (A(j, 0) < eps && A(j, 0) > -1 * eps) {
                A(j, 0) = 0;
            }
            A(j, i) = 0;
        }
    }

    // Back substitution
    Mat x(b.rows, 1);
    x((x.rows - 1), 0) = b((x.rows - 1), 0) / A((x.rows - 1), (x.rows - 1));
    if (x((x.rows - 1), 0) < eps && x((x.rows - 1), 0) > -1 * eps) {
        x((x.rows - 1), 0) = 0;
    }
    for (int i = x.rows - 2; i >= 0; --i) {
        float sum = 0;
        for (int j = i + 1; j < x.rows; ++j) {
            sum += A(i, j) * x(j, 0);
        }
        x(i, 0) = (b(i, 0) - sum) / A(i, i);
        if (x(i, 0) < eps && x(i, 0) > -1 * eps) {
            x(i, 0) = 0;
        }
    }
    return x;
}

Mat Mat::bandSolve(Mat A, Mat b, int k)
{
    // optimized Gaussian elimination
    int bandsBelow = (k - 1) / 2;
    for (int i = 0; i < A.rows; ++i) {
        if (A(i,i) == 0) {
            // pivot 0 - error
            ESP_LOGW("Mat", "Error: the coefficient matrix has 0 as a pivot. Please fix the input and try again.");
         Mat err_result(b.rows, 1);
         memset(err_result.mdata, 0, b.rows * sizeof(float));
         return err_result;
        }
        for (int j = i + 1; j < A.rows && j <= i + bandsBelow; ++j) {
            int k = i + 1;
            while (k < A.cols && A(j,k)) {
                A(j,k) -= A(i,k) * (A(j,i) / A(i,i));
                k++;
            }
            b(j,0) -= b(i,0) * (A(j,i) / A(i,i));
            A(j,i) = 0;
        }
    }

    // Back substitution
    Mat x(b.rows, 1);
    x((x.rows - 1), 0) = b((x.rows - 1), 0) / A((x.rows - 1), (x.rows - 1));
    for (int i = x.rows - 2; i >= 0; --i) {
        float sum = 0;
        for (int j = i + 1; j < x.rows; ++j) {
            sum += A(i, j) * x(j, 0);
        }
        x(i, 0) = (b(i, 0) - sum) / A(i, i);
    }

    return x;
}

Mat Mat::roots(Mat A, Mat y)
{
    int n = A.cols + 1;

    Mat result(y.rows, 1);

    Mat g_m = Mat::augment(A, y);
    for (int j = 0; j < A.cols; j++) {
        for (int i = 0; i < A.cols; i++) {
            if (i != j) {
                float c = g_m(i, j) / g_m(j, j);
                for (int k = 0; k < n; k++) {
                    g_m(i, k) = g_m(i, k) - c * g_m(j, k);
                }
            }
        }
    }
    for (int i = 0; i < A.rows; i++) {
        result(i, 0) = g_m(i, A.cols) / g_m(i, i);
    }
    return result;
}

float Mat::dotProduct(Mat a, Mat b)
{
    float sum = 0;
    for (int i = 0; i < a.rows; ++i) {
        sum += (a(i, 0) * b(i, 0));
    }
    return sum;
}

Mat Mat::augment(Mat A, Mat B)
{
    Mat AB(A.rows, A.cols + B.cols);
    for (int i = 0; i < AB.rows; ++i) {
        for (int j = 0; j < AB.cols; ++j) {
            if (j < A.cols) {
                AB(i, j) = A(i, j);
            } else {
                AB(i, j) = B(i, j - A.cols);
            }
        }
    }
    return AB;
}

Mat Mat::gaussianEliminate()
{
    Mat Ab(*this);
    int rows = Ab.rows;
    int cols = Ab.cols;
    int Acols = cols - 1;

    int i = 0; // row tracker
    int j = 0; // column tracker

    // iterate through the rows
    while (i < rows) {
        // find a pivot for the row
        bool pivot_found = false;
        while (j < Acols && !pivot_found) {
            if (Ab(i, j) != 0) { // pivot not equal to 0
                pivot_found = true;
            } else { // check for a possible swap
                int max_row = i;
                float max_val = 0;
                for (int k = i + 1; k < rows; ++k) {
                    float cur_abs = Ab(k, j) >= 0 ? Ab(k, j) : -1 * Ab(k, j);
                    if (cur_abs > max_val) {
                        max_row = k;
                        max_val = cur_abs;
                    }
                }
                if (max_row != i) {
                    Ab.swapRows(max_row, i);
                    pivot_found = true;
                } else {
                    j++;
                }
            }
        }

        // perform elimination as normal if pivot was found
        if (pivot_found) {
            for (int t = i + 1; t < rows; ++t) {
                for (int s = j + 1; s < cols; ++s) {
                    Ab(t, s) = Ab(t, s) - Ab(i, s) * (Ab(t, j) / Ab(i, j));
                    if (Ab(t, s) < eps && Ab(t, s) > -1 * eps) {
                        Ab(t, s) = 0;
                    }
                }
                Ab(t, j) = 0;
            }
        }

        i++;
        j++;
    }

    return Ab;
}

Mat Mat::rowReduceFromGaussian()
{
    Mat R(*this);
    int rows = R.rows;
    int cols = R.cols;

    int i = rows - 1; // row tracker
    int j = cols - 2; // column tracker

    // iterate through every row
    while (i >= 0) {
        // find the pivot column
        int k = j - 1;
        while (k >= 0) {
            if (R(i, k) != 0) {
                j = k;
            }
            k--;
        }

        // zero out elements above pivots if pivot not 0
        if (R(i, j) != 0) {
            for (int t = i - 1; t >= 0; --t) {
                for (int s = 0; s < cols; ++s) {
                    if (s != j) {
                        R(t, s) = R(t, s) - R(i, s) * (R(t, j) / R(i, j));
                        if (R(t, s) < eps && R(t, s) > -1 * eps) {
                            R(t, s) = 0;
                        }
                    }
                }
                R(t, j) = 0;
            }

            // divide row by pivot
            for (int k = j + 1; k < cols; ++k) {
                R(i, k) = R(i, k) / R(i, j);
                if (R(i, k) < eps && R(i, k) > -1 * eps) {
                    R(i, k) = 0;
                }
            }
            R(i, j) = 1;
        }

        i--;
        j--;
    }

    return R;
}

Mat Mat::inverse()
{
    Mat I = Mat::createIdentity(this->rows);
    Mat AI = Mat::augment(*this, I);
    Mat U = AI.gaussianEliminate();
    Mat IAInverse = U.rowReduceFromGaussian();
    Mat AInverse(this->rows, this->cols);
    for (int i = 0; i < AInverse.rows; ++i) {
        for (int j = 0; j < AInverse.cols; ++j) {
            AInverse(i, j) = IAInverse(i, j + this->cols);
        }
    }
    return AInverse;
}

void Mat::allocate()
{
    this->ext_buff = false;
    mdata = new float[this->rows * this->cols];
    ESP_LOGD("Mat", "allocate(%i) = 0x%8.8x", this->rows * this->cols, (uint32_t)this->mdata);
}

Mat Mat::expHelper(const Mat &m, int num)
{
    if (num == 0) {
        return createIdentity(m.rows);
    } else if (num == 1) {
        return m;
    } else if (num % 2 == 0) {  // num is even
        return expHelper(m * m, num / 2);
    } else {                    // num is odd
        return m * expHelper(m * m, (num - 1) / 2);
    }
}

Mat operator+(const Mat &m1, const Mat &m2)
{
    Mat temp(m1);
    return (temp += m2);
}

Mat operator+(const Mat &m1, float C)
{
    Mat temp(m1);
    return (temp += C);
}

bool operator==(const Mat &m1, const Mat &m2)
{
    if ((m1.cols != m2.cols) || (m1.rows != m2.rows)) {
        return false;
    }
    for (int i = 0 ; i < (m1.cols * m1.rows) ; i++) {
        if (m1.mdata[i] != m2.mdata[i]) {
            printf("Error: %i, m1.mdata=%f, m2.mdata=%f \n", i, m1.mdata[i], m2.mdata[i]);
            return false;
        }
    }
    return true;
}

Mat operator-(const Mat &m1, const Mat &m2)
{
    Mat temp(m1);
    return (temp -= m2);
}

Mat operator-(const Mat &m1, float C)
{
    Mat temp(m1);
    return (temp -= C);
}

Mat operator*(const Mat &m1, const Mat &m2)
{
    Mat temp(m1.rows, m2.cols);
    dspm_mult_f32(m1.mdata, m2.mdata, temp.mdata, m1.rows, m1.cols, m2.cols);
    return temp;
}

Mat operator*(const Mat &m, float num)
{
    Mat temp(m);
    return (temp *= num);
}

Mat operator*(float num, const Mat &m)
{
    return (m * num);
}

Mat operator/(const Mat &m, float num)
{
    Mat temp(m);
    return (temp /= num);
}

ostream &operator<<(ostream &os, const Mat &m)
{
    for (int i = 0; i < m.rows; ++i) {
        os << m(i, 0);
        for (int j = 1; j < m.cols; ++j) {
            os << " " << m(i, j);
        }
        os << endl;
    }
    return os;
}

istream &operator>>(istream &is, Mat &m)
{
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            is >> m(i, j);
        }
    }
    return is;
}

}