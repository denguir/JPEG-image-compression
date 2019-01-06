// Lab 3

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include "image_lab1.h"
#include "image_lab2.h"

#define PI 3.14159265
using namespace std;


float* DCTBasis(int size) {
  float* DCTbasis = new float [size*size];
  float a = sqrt(2.0/size);
  for(int k=0; k < size; k++) {
    for(int i=0; i < size; i++) {
      float lamda = 1.0;
      if (k == 0) {
        lamda = 1.0/sqrt(2);
      }
      DCTbasis[k * size + i] = a * lamda * cos(((PI*k)/(2.0*size))*(2.0*i + 1));
    }
  }
    return DCTbasis;
}

float* transpose(float* matrix, int size) {
  float* matrixT = new float [size*size];
  for (int i=0; i < size; i++) {
    for (int j=0; j < size; j++) {
      matrixT[j * size + i] = matrix[i * size + j];
    }
  }
  return matrixT;
}

float* matmul(float* matrix1, float* matrix2, int size) {
  float* matrixRes = new float [size*size]();
  for (int k=0; k < size; k++) {
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
        matrixRes[i* size + j] += matrix1[i*size + k] * matrix2[k*size + j];
      }
    }
  }
  return matrixRes;
}

float* transform(float* image, float* basis, int size) {
  float* m1 = transpose(image, size);
  float* m2 = matmul(basis, m1, size);
  delete[] m1;
  float* m3 = transpose(m2, size);
  delete[] m2;
  float* coeff = matmul(basis, m3, size);
  delete[] m3;
  return coeff;
}

float* threshold(float* matrix, int size, int th) {
  float* matOut = new float [size*size];
  for (int i=0; i < size*size; i++) {
    if (abs(matrix[i]) < th) {
      matOut[i] = 0;
    } else {
      matOut[i] = matrix[i];
    }
  }
  return matOut;
}
