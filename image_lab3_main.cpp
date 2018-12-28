
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


int main(int argc, char const *argv[]) {
  const char* dctFile = "dct.raw";
  const char* identityFile = "identity.raw";
  const char* coeffFile = "coeff.raw";
  const char* inFile = "lena_256x256.raw";
  const char* compressedFile = "compressed.raw";

  int thValue= 20; // threshold

  float* basis = DCTBasis(256);
  store(basis, 256, dctFile);

  float* basisT = transpose(basis, 256);
  float* identity = matmul(basis, basisT, 256);
  store(identity, 256, identityFile);

  float* imageIn = load(inFile, 256);
  float* coeff = transform(imageIn, basis, 256);
  
  float* coeffCompressed = threshold(coeff, 256, thValue);
  store(coeffCompressed, 256, coeffFile);

  float* compressedImg = transform(coeffCompressed, basisT, 256);
  store(compressedImg, 256, compressedFile);

  float PSNR = psnr(imageIn, compressedImg, 256, 255);
  std::cout << "PSNR with threshold = " << thValue << ": \t" << PSNR <<'\n';

  return 0;
}
