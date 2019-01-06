// Lab 6

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include <sstream>
#include "image_lab1.h"
#include "image_lab2.h"
#include "image_lab3.h"
#include "image_lab4.h"
#include "image_lab5.h"

#define PI 3.14159265
using namespace std;

bool IsBitSet(unsigned int num, int bit) {
    return 1 == ((num >> bit) & 1);
}

unsigned int map(int v) {
  // maps a signed int to an unsigned int
  int m;
  if (v > 0) m = 2*v - 1;
  else m = 2*abs(v);
  return m;
}

int demap(int v) {
  // inverse of map
  int d;
  if (v % 2 == 0) d = -v/2;
  else d = v/2 + 1;
  return d;
}

string golomb(int value) {
  // Golomb coding in the case when k=0
  string s;
  bool next_bit;
  unsigned int v = map(value) + 1;
  int length = floor(log2(v));
  for (int i=0; i<length; i++) {
    s += "0";
  }
  for (int i=length; i>=0; i--) {
    next_bit = IsBitSet(v, i);
    s += to_string(next_bit);
  }
  return s;
}

int golomb(istream &is) {
  int tot = 0;
  int c = 0;
  while (is.get() == '0') c++;
  int pos = is.tellg();
  is.seekg(pos-1);
  while (c >= 0) {
    if (is.get() == '1') tot += pow(2, c);
    c--;
  }
  return demap(tot-1);
}

void compress(const char* DeltaFile, const char* RLEFile, const char* bitStream) {
  ifstream inDelta(DeltaFile);
  ifstream inRLE(RLEFile);
  ofstream out(bitStream);
  int data;
  if (!inDelta || !inRLE) {
    std::cout << "Cannot open" << DeltaFile << RLEFile << '\n';
  } else {
    for (std::string line; std::getline(inDelta, line);) {
      data = stoi(line);
      out << golomb(data);
    }
    for (std::string line; std::getline(inRLE, line);) {
      data = stoi(line);
      out << golomb(data);
    }
    out.close();
    inDelta.close();
    inRLE.close();
  }
}

void decompress(const char* bitStream, const char* DeltaFile, const char* RLEFile, int imageSize, int blockSize) {
  ifstream in(bitStream);
  ofstream outDelta(DeltaFile);
  ofstream outRLE(RLEFile);
  int data;
  int pos;
  int counter = 0;
  int N = imageSize/blockSize;
  char c;
  if (!in) {
    std::cout << "Cannot open" << bitStream << '\n';
  } else {
    while (in >> c) {
      pos = in.tellg();
      data = golomb(in.seekg(pos-1));
      if (counter < N*N) {
        outDelta << data << endl;
      } else {
        outRLE << data << endl;
      }
      ++counter;
    }
    outDelta.close();
    outRLE.close();
    in.close();
  }
}

void encode(float* image, int imageSize, float* qBlock, int blockSize, const char* bitStream) {
  const char* DCfile = "delta_encoding.txt";
  const char* ACfile = "rle_encoding.txt";
  float* DCTbasis = DCTBasis(blockSize);
  float* DCTCoeff = blockTransform(image, imageSize, DCTbasis, blockSize);
  delete[] DCTbasis;
  float* qDCTCoeff = quantize(DCTCoeff, imageSize, qBlock, blockSize);
  delete[] DCTCoeff;
  float* DC = DCImage(qDCTCoeff, imageSize, blockSize);
  DeltaEncoding(DC, imageSize/blockSize, DCfile);
  delete[] DC;
  RunLengthEncoding(qDCTCoeff, imageSize, blockSize, ACfile);
  delete[] qDCTCoeff;
  compress(DCfile, ACfile, bitStream);
}

float* decode(const char* bitStream, int size, float* qBlock, int blockSize) {
  const char* DCfile = "delta_decoding.txt";
  const char* ACfile = "rle_decoding.txt";
  decompress(bitStream, DCfile, ACfile, size, blockSize);
  float* DCdecoded = DeltaDecoding(DCfile, size/blockSize);
  float* ACdecoded = RunLengthDecoding(ACfile, size, blockSize);
  float* decoded = merge(DCdecoded, ACdecoded, size, blockSize);
  delete[] ACdecoded;
  delete[] DCdecoded;
  float* DCTbasis = DCTBasis(blockSize);
  float* IDCTbasis = transpose(DCTbasis, blockSize);
  delete[] DCTbasis;
  float* IqDCTCoeff = Iquantize(decoded, size, qBlock, blockSize);
  float* imgOut = blockTransform(IqDCTCoeff, size, IDCTbasis, blockSize);
  delete[] IDCTbasis;
  delete[] IqDCTCoeff;
  return imgOut;
}
