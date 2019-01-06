// Lab 4

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include "image_lab1.h"
#include "image_lab2.h"
#include "image_lab3.h"
#include "image_lab4.h"

int main(int argc, char const *argv[]) {
  const char* inFile = "lena_256x256.raw";
  const char* qFile = "quantize.raw";
  const char* qBlockFile = "qblock.raw";
  const char* dctBlockFile = "dct_block.raw";
  const char* IqFile = "Iquantize.raw";
  const char* approxImgFile = "approxImg.raw";
  const char* diffFile = "difference.raw";
  const char* gray8bppFile = "lena8bpp.raw";

  float* imageIn = load(inFile, 256);
  float* basis = DCTBasis(8);

  float* coeffDct = blockTransform(imageIn, 256, basis, 8);
  store(coeffDct, 256, dctBlockFile);

  float qBlock[64] = {16, 11, 10, 16, 24, 40, 51, 61,
                   12, 12, 14, 19, 26, 58, 60, 55,
                   14, 13, 16, 24, 40, 57, 69, 56,
                   14, 17, 22, 29, 51, 87, 80, 62,
                   18, 22, 37, 56, 68, 109, 103, 77,
                   24, 35, 55, 64, 81, 104, 113, 92,
                   49, 64, 78, 87, 103, 121, 120, 101,
                   72, 92, 95, 98, 112, 100, 103, 99};

  store(qBlock, 8, qBlockFile);

  float* qImage = quantize(coeffDct, 256, qBlock, 8);
  store(qImage, 256, qFile);

  float* IqImage = Iquantize(qImage, 256, qBlock, 8);
  store(IqImage, 256, IqFile);

  float* approxImg = approximate(imageIn, 256, qBlock, 8);
  store(approxImg, 256, approxImgFile);

  float* diffImg = difference(imageIn, approxImg, 256);
  store(diffImg, 256, diffFile);

  char* image8bpp = grayscale8bpp(imageIn, 256);
  store(image8bpp, 256, gray8bppFile);

  return 0;
}
