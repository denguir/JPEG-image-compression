// Lab 4

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include "image_lab1.h"
#include "image_lab2.h"
#include "image_lab3.h"


#define PI 3.14159265
using namespace std;


float* quantize(float* image, int imageSize, float* block, int blockSize) {
  // quantize the image block per block using a quantization block matrix
  float* quantized = new float [imageSize*imageSize];
  int N = imageSize/blockSize; // number of blocks
  for (int n=0; n<N; n++) {
    for (int m=0; m<N; m++) {
      for (int i=0; i<blockSize; i++) {
        for (int j=0; j<blockSize; j++) {
          quantized[(n*blockSize + i)*imageSize + m*blockSize + j] =
          round(image[(n*blockSize + i)*imageSize + m*blockSize + j]/block[i*blockSize +j]);
        }
      }
    }
  }
  return quantized;
}

float* Iquantize(float* qImage, int imageSize, float* block, int blockSize) {
  // Retrieve partially the quantized image (qImage)
  float* Iquantized = new float [imageSize*imageSize];
  int N = imageSize/blockSize; // number of blocks
  for (int n=0; n<N; n++) {
    for (int m=0; m<N; m++) {
      for (int i=0; i<blockSize; i++) {
        for (int j=0; j<blockSize; j++) {
          Iquantized[(n*blockSize + i)*imageSize + m*blockSize + j] =
          qImage[(n*blockSize + i)*imageSize + m*blockSize + j] * block[i*blockSize +j];
        }
      }
    }
  }
  return Iquantized;
}

float* blockTransform(float* image, int imageSize, float* basis, int blockSize) {
  // apply DCT block per block in image
  float* coeff = new float [imageSize*imageSize];
  int N = imageSize/blockSize;
  for (int n=0; n<N; n++) {
    for (int m=0; m<N; m++) {
      float* blockImg = new float [blockSize*blockSize];
      for (int i=0; i<blockSize; i++) {
        for (int j=0; j<blockSize; j++) {
          blockImg[i*blockSize + j] = image[(n*blockSize + i)*imageSize + m*blockSize + j];
        }
      }
      float* blockCoeff = transform(blockImg, basis, blockSize);
      for (int i=0; i<blockSize; i++) {
        for (int j=0; j<blockSize; j++) {
          coeff[(n*blockSize + i)*imageSize + m*blockSize + j] = blockCoeff[i*blockSize + j];
        }
      }
      delete[] blockCoeff;
    }
  }
  return coeff;
}

float* approximate(float* image, int imageSize, float* qBlock, int blockSize) {
  float* DCTbasis = DCTBasis(blockSize);
  float* IDCTbasis = transpose(DCTbasis, blockSize);
  float* DCTCoeff = blockTransform(image, imageSize, DCTbasis, blockSize);
  delete[] DCTbasis;
  float* qDCTCoeff = quantize(DCTCoeff, imageSize, qBlock, blockSize);
  delete[] DCTCoeff;
  float* IqDCTCoeff = Iquantize(qDCTCoeff, imageSize, qBlock, blockSize);
  delete[] qDCTCoeff;
  float* imgOut = blockTransform(IqDCTCoeff, imageSize, IDCTbasis, blockSize);
  delete[] IDCTbasis;
  delete[] IqDCTCoeff;
  return imgOut;
}



float* difference(float* image1, float* image2, int size) {
  float* diffImg = new float [size*size];
  for (int i=0; i < size*size; i++) {
    diffImg[i] = abs(image1[i] - image2[i]);
  }
  return diffImg;
}
