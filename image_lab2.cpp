
// Lab 2

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <random>
#include "image_lab1.h"

#define PI 3.14159265

using namespace std;

float* load(const char* name, int size) {
  float* image = new float [size*size];
  ifstream imageFile(name, ios::out | ios::binary);
  imageFile.read ((char*)image, sizeof(float)*size*size);
  imageFile.close();
  return image;
}

float* noise(float* image, int size, float mean, float std) {
  float* outimage = new float [size*size]; // necessary to have outimage != image
  default_random_engine generator;
  normal_distribution<float> distribution(mean, std);
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++){
      float random_var = distribution(generator);
      outimage[i*size + j] = image[i*size + j] + random_var;
    }
  }
  return outimage;
}

float psnr(float* image1, float* image2, int size, int max) {
  float MSE = 0.0;
  for (int i=0; i < size*size; i++) {
      MSE = MSE + pow(image1[i] - image2[i], 2);
  }
  MSE = MSE / (size*size);
  float PSNR = 20*log10(max) - 10*log10(MSE);
  return PSNR;
}

float* gaussian_kernel(int size, float var) {
  float* kernel = new float [size*size];
  int k = size/2;
  float sum = 0.0;
  for (int x=-k; x<=k; x++) {
    for (int y=-k; y<=k; y++) {
      int r2 = pow(x,2) + pow(y,2);
      float kxy = (1/(2*PI*var)) * exp(-r2/(2*var));
      kernel[(x+k)*size + (y+k)] = kxy;
      sum = sum + kxy;
    }
  }
  // normalize the Kernel
  for (int x=-k; x<=k; x++) {
    for (int y=-k; y<=k; y++) {
      kernel[(x+k)*size + (y+k)] = kernel[(x+k)*size + (y+k)]/sum;
      std::cout << std::setprecision(5) << kernel[(x+k)*size + (y+k)] << '\t';
    }
    std::cout << std::endl;
  }
  return kernel;
}


float* blur1(float* image, int imageSize, float* kernel, int kernelSize) {
  // convolution applied in the image
  int k = kernelSize/2;
  float* bluredImage = new float [imageSize*imageSize];
  for (int row=k; row<imageSize-k; row++) {
    for (int col=k; col<imageSize-k; col++) {
      float accumulation = 0;
      for (int i=-k; i<=k; i++) {
        for (int j=-k; j<=k; j++) {
          float q = image[(row+i)*imageSize + (col + j)];
          accumulation += q * kernel[(k+i)*kernelSize + (k+j)];
        }
      }
      bluredImage[row*imageSize + col] = accumulation;
    }
  }
  return bluredImage;
}

float* blur2(float* image, int imageSize, float* kernel, int kernelSize) {
  // convolution applied with border effects
  int k = kernelSize/2;
  float* bluredImage = new float [imageSize*imageSize];
  for (int row=0; row<imageSize; row++) {
    for (int col=0; col<imageSize; col++) {
      float accumulation = 0;
      for (int i=-k; i<=k; i++) {
        for (int j=-k; j<=k; j++) {
          float q = image[(row+i)*imageSize + (col + j)];
          if (row+i < 0 || row+i > imageSize-1 || col+j < 0 || col+j > imageSize-1);
          else {
            accumulation += q * kernel[(k+i)*kernelSize + (k+j)];
          }
        }
      }
      bluredImage[row*imageSize + col] = accumulation;
    }
  }
  return bluredImage;
}

float* sharpen(float* image, float* smoothImage, int size, float alpha) {
  float* sharpImage = new float [size*size];
  for (int i=0; i<size*size; i++) {
    sharpImage[i] = image[i] + alpha * (image[i] - smoothImage[i]);
  }
  return sharpImage;
}
