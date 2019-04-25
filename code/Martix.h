#ifndef __LIGHT_MATRIX__
#define __LIGHT_MATRIX__

typedef struct  {
    int row, col;
    float **element;
    unsigned char init;
}Mat;

Mat* MatCreate(Mat* mat, int row, int col);
void MatDelete(Mat* mat);
Mat* MatSetVal(Mat* mat, float* val);
Mat* MatAdd(Mat* src1, Mat* src2, Mat* dst);
Mat* MatSub(Mat* src1, Mat* src2, Mat* dst);
Mat* MatMul(Mat* src1, Mat* src2, Mat* dst);
float MatDet(Mat* mat);
Mat* MatInv(Mat* src, Mat* dst);
