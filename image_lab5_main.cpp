// Lab 5

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include "image_lab1.h"
#include "image_lab2.h"
#include "image_lab3.h"
#include "image_lab4.h"
#include "image_lab5.h"

#define PI 3.14159265
using namespace std;


int main(int argc, char const *argv[]) {
  // .raw
  const char* inFile = "lena_256x256.raw";
  const char* DCFile = "dc_lena.raw";
  const char* ACFile = "ac_lena.raw";
  const char* blockFile = "block1.raw";
  const char* DCdecodedFile = "decoded_dc.raw";
  const char* qFile = "quantize.raw";
  const char* resFile = "result.raw";
  // .txt
  const char* DeltaFile = "delta_encoding.txt";
  const char* RLEFile = "rle_encoding.txt";

  float* imageIn = load(inFile, 256);
  float* qDCTCoeff = load(qFile, 256);

  // get DC coeff of each block and form a 32x32 image
  float* DC = DCImage(qDCTCoeff, 256, 8);
  store(DC, 32, DCFile);

  // Get block (x, y): useful for debuging
  float* block1 = GetBlock(qDCTCoeff, 256, 8, 0, 31);
  store(block1, 8, blockFile);

  // energy and entropy before Delta encoding
  float energyDC = energy(DC, 32);
  std::cout << "energy DC (no Delta): \t" << energyDC << '\n';
  float H = entropy(DC, 32, 8); // bit = 8 because grayscale img
  std::cout << "entropy (no Delta): \t" << H << '\n';

  // energy and entropy after Delta encoding
  DeltaEncoding(DC, 32, DeltaFile);
  float* DCencoded = loadTxt(DeltaFile, 32);
  float energyDelta = energy(DCencoded, 32);
  std::cout << "energy DC (with Delta): \t" << energyDelta << '\n';
  float HDelta = entropy(DCencoded, 32, 8);
  std::cout << "entropy DC (with Delta): \t" << HDelta << '\n';

  // Delta decoding
  float* DCdecoded = DeltaDecoding(DeltaFile, 32);
  store(DCdecoded, 32, DCdecodedFile);

  // Run Length encoding
  RunLengthEncoding(qDCTCoeff, 256, 8, RLEFile);
  // Run Lengths Decoding
  float* AC = RunLengthDecoding(RLEFile, 256, 8);
  store(AC, 256, ACFile);

  float* CoeffBack = merge(DC, AC, 256, 8);
  store(CoeffBack, 256, resFile);

  return 0;
}
