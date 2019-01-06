// Lab 5

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include "image_lab1.h"
#include "image_lab2.h"
#include "image_lab3.h"
#include "image_lab4.h"

#define PI 3.14159265
using namespace std;



float* GetCoeffBlock(float* qImage, int imageSize, int blockSize, int x, int y) {
  // build an image from the (x,y) coordinate of each block of the quantized coeffDct matrix
  // 0 < x,y < 8
  int N = imageSize/blockSize;
  float* coeffXY = new float [N*N];
  for (int n=0; n<N; n++) {
    for (int m=0; m<N; m++) {
      coeffXY[n*N + m] = qImage[(n*blockSize + x)*imageSize + m*blockSize + y];
      // To compare with lena32x32, you should devide DC by 8
      // then apply Iquantize()
    }
  }
  return coeffXY;
}

float* DCImage(float* qImage, int imageSize, int blockSize) {
  float* DC = GetCoeffBlock(qImage, imageSize, blockSize, 0, 0);
  return DC;
}

void SetCoeffBlock(float* image, int imageSize, float* coeffBlock, int blockSize, int x, int y) {
  int N = imageSize/blockSize; // should be the size of coeffBlock
  for (int n=0; n<N; n++) {
    for (int m=0; m<N; m++) {
      image[(n*blockSize + x)*imageSize + m*blockSize + y] = coeffBlock[n*N + m];
    }
  }
}

float* GetBlock(float* qImage, int imageSize, int blockSize, int x, int y) {
  // Get the block number (x,y) of the quantized image
  // 0 < x,y < 32
  float* blockXY = new float [blockSize*blockSize];
  for (int i=0; i<blockSize; i++) {
    for (int j=0; j<blockSize; j++) {
      blockXY[i*blockSize + j] = qImage[(x*blockSize + i)*imageSize + y*blockSize + j];
    }
  }
  return blockXY;
}

void SetBlock(float* image, int imageSize, float* block, int blockSize, int x, int y) {
  for (int i=0; i<blockSize; i++) {
    for (int j=0; j<blockSize; j++) {
      image[(x*blockSize + i)*imageSize + y*blockSize + j] = block[i*blockSize + j];
    }
  }
}

void DeltaEncoding(float* coeff, int DCsize, const char* filename) {
  ofstream out(filename);
  int encodedNum;
  if(!out) {
    std::cout << "Cannot open " << filename << '\n';
  } else {
    for (int i=0; i<DCsize*DCsize; i++) {
      if (i == 0) {
        encodedNum = coeff[i];
      } else {
        encodedNum = coeff[i] - coeff[i-1];
      }
      out << encodedNum << endl;
    }
    out.close();
  }
}

float* DeltaDecoding(const char* filename, int DCsize) {
  float* DC = new float[DCsize*DCsize];
  float decodedNum;
  float prevDelta = 0;
  ifstream in(filename);
  if(!in) {
    std::cout << "Cannot open " << filename << '\n';
  } else {
    for (int i=0; i<DCsize*DCsize; i++) {
      in >> decodedNum;
      DC[i] = decodedNum + prevDelta;
      prevDelta = DC[i];
    }
    in.close();
  }
  return DC;
}

int* zigzag(int blockSize) {
  // return an index array to follow that parses a block following a zigzag pattern
  int sum;
  int check = blockSize - 1;
  int arraySize = 2*blockSize - 1;
  std::vector<std::vector<int>> array;
  array.resize(arraySize, std::vector<int>(blockSize, 0));
  //int array[arraySize][blockSize] = {}; // array of indeces initialized to 0
  int* indeces = new int[blockSize*blockSize];

  // first build the 2D array of indeces
  for (int i=0; i<blockSize; i++) {
    for (int j=0; j<blockSize; j++) {
      sum = i+j;
      if (sum % 2 == 0) {
        while (array[sum][check] > 0) check--;
        // store index of block in the end
        array[sum][check] = i*blockSize + j;
        check = 0; // after an even comes an odd sum
      } else {
        while (array[sum][check] > 0) check++;
        // store index of block in the end
        array[sum][check] = i*blockSize + j;
        check = blockSize - 1; // after an odd comes an even sum
      }
    }
  }
  // Now build the 1D array of indeces
  int idx;
  int k = 1;
  indeces[0] = 0;
  indeces[blockSize*blockSize-1] = blockSize*blockSize-1;
  // we already know the boundaries, no need to search along a row
  for (int i=1; i<arraySize-1; i++) {
    for (int j=0; j<blockSize; j++) {
      idx = array[i][j];
      if (idx > 0) {
        indeces[k] = idx;
        k++;
      }
    }
  }
  return indeces;
}

float* zigzag(float* block, int blockSize) {
  // return a re-ordered array following a zigzag pattern
  int sum;
  int check = blockSize - 1;
  int arraySize = 2*blockSize - 1;
  std::vector<std::vector<int>> array;
  array.resize(arraySize, std::vector<int>(blockSize, 0));
  //int array[arraySize][blockSize] = {}; // array of indeces initialized to 0
  float* blockZ = new float[blockSize*blockSize];

  // first build the 2D array of indeces
  for (int i=0; i<blockSize; i++) {
    for (int j=0; j<blockSize; j++) {
      sum = i+j;
      if (sum % 2 == 0) {
        while (array[sum][check] > 0) check--;
        // store index of block in the end
        array[sum][check] = i*blockSize + j;
        check = 0; // after an even comes an odd sum
      } else {
        while (array[sum][check] > 0) check++;
        // store index of block in the end
        array[sum][check] = i*blockSize + j;
        check = blockSize - 1; // after an odd comes an even sum
      }
    }
  }
  // Now build the 1D array
  int idx;
  int k = 1;
  blockZ[0] = block[0];
  blockZ[blockSize*blockSize-1] = block[blockSize*blockSize-1];
  // we already know the boundaries, no need to search along a row
  for (int i=1; i<arraySize-1; i++) {
    for (int j=0; j<blockSize; j++) {
      idx = array[i][j];
      if (idx > 0) {
        blockZ[k] = block[idx];
        k++;
      }
    }
  }
  return blockZ;
}

string BlockRLE(float* block, int blockSize) {
  string sequence;
  string subSeq;
  int i = 1; // pass DC coeff
  int c = 0; // counter
  float* blockZ = zigzag(block, blockSize);
  // first bit tells if the first counter counts zeros or non-zeros
  if (blockZ[1] == 0) {
    sequence = to_string(0) + '\n'; // first zeros
  } else {
    sequence = to_string(1) + '\n'; // first non zeros
  }
  while (i < blockSize*blockSize) {
    while (int(blockZ[i]) != 0 && i < blockSize*blockSize) {
      subSeq += to_string(int(blockZ[i]) - 1) + '\n';
      c++; i++;
    }
    if (c > 0) {
      sequence += to_string(c - 1) + '\n' + subSeq;
      c = 0;
      subSeq.clear();
    }
    while (int(blockZ[i]) == 0 && i < blockSize*blockSize) {
      c++; i++;
    }
    sequence += to_string(c - 1) + '\n';
    c = 0;
  }
  delete[] blockZ;
  return sequence;
}


void RunLengthEncoding(float* coeff, int size, int blockSize, const char* filename) {
  ofstream out(filename);
  string encodedSeq;
  int N = size/blockSize;
  int x, y;
  float* block;
  if (!out) {
    std::cout << "Cannot open " << filename << '\n';
  } else {
    for (int i=0; i<N*N; i++) {
      x = i/N;
      y = i%N;
      block = GetBlock(coeff, size, blockSize, x, y);
      encodedSeq = BlockRLE(block, blockSize);
      // std::cout << encodedSeq << '\n';
      out << encodedSeq;
    }
    out.close();
  }
  delete[] block;
}

float* BlockRLD(istream &is, int blockSize, int* indexZ) {
  float* blockACZ = new float[blockSize*blockSize];
  float* blockAC = new float[blockSize*blockSize];
  blockACZ[0] = 0; // DC coeff not attributed
  int i = 1;
  int j;
  for (std::string line; std::getline(is, line); ) {
    blockACZ[i] = stof(line);
    i++;
  }
  // from zigzag to normal pattern
  for (int i=1; i<blockSize*blockSize; i++) {
    j = 0;
    while (indexZ[j] != i) j++;
    blockAC[i] = blockACZ[j];
  }
  delete[] blockACZ;
  return blockAC;
}

float* RunLengthDecoding(const char* filename, int size, int blockSize) {
  ifstream in(filename);
  float* blockAC = new float[blockSize*blockSize];
  float* AC = new float[size*size]();
  int* indexZ = zigzag(blockSize);
  string blockSeq;
  int decodedSeq;
  int blockCounter;
  int N = size/blockSize;
  int x, y, c, s;
  if(!in) {
    std::cout << "Cannot open " << filename << '\n';
  } else {
    for (int i=0; i<N*N; i++) {
      x = i/N;
      y = i%N;
      blockSeq.clear();
      blockCounter = 0;
      in >> c; // c is a switch, should be equal to 0 or 1
      while (blockCounter < blockSize * blockSize - 1) {
        in >> decodedSeq;
        ++decodedSeq;
        blockCounter += decodedSeq;
        if (c == 1) {
          for (int j=0; j<decodedSeq; j++) {
            in >> s;
            blockSeq += to_string(s+1) + '\n';
          }
        } else {
          for (int j=0; j<decodedSeq; j++) {
            blockSeq += "0 \n";
          }
        }
        c = not c;
      }
      blockAC = BlockRLD(istringstream(blockSeq).seekg(0), blockSize, indexZ);
      SetBlock(AC, size, blockAC, blockSize, x, y);
    }
  }
  delete[] blockAC;
  delete[] indexZ;
  return AC;
}

float* merge(float* DC, float* AC, int imageSize, int blockSize) {
  // merge AC and DC coeff in one single image
  float* coeff = new float[imageSize*imageSize]();
  for (int i=0; i<imageSize*imageSize; i++) {
    coeff[i] = AC[i];
  }
  SetCoeffBlock(coeff, imageSize, DC, blockSize, 0, 0); // set DC coeff
  return coeff;
}

void encode(float* image, int imageSize, float* qBlock, int blockSize, const char* DCfile, const char* ACfile) {
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
}

float* decode(int size, float* qBlock, int blockSize, const char* DCfile, const char* ACfile) {
  float* DCdecoded = DeltaDecoding(DCfile, size/blockSize);
  float* ACdecoded = RunLengthDecoding(ACfile, size, blockSize);
  float* decoded = merge(DCdecoded, ACdecoded, size, blockSize);
  delete[] DCdecoded;
  delete[] ACdecoded;
  float* DCTbasis = DCTBasis(blockSize);
  float* IDCTbasis = transpose(DCTbasis, blockSize);
  delete[] DCTbasis;
  float* IqDCTCoeff = Iquantize(decoded, size, qBlock, blockSize);
  float* imgOut = blockTransform(IqDCTCoeff, size, IDCTbasis, blockSize);
  delete[] IDCTbasis;
  delete[] IqDCTCoeff;
  return imgOut;
}


float* loadTxt(const char* filename, int size) {
  float* img = new float[size*size];
  ifstream in(filename);
  float value;
  if(!in) {
    std::cout << "Cannot open " << filename << '\n';
  } else {
    for (int i=0; i<size*size; i++) {
      in >> value;
      img[i] = value;
    }
    in.close();
  }
  return img;
}

float energy(float* image, int size) {
  float E = 0;
  for (int i=0; i<size*size; i++) {
    E += pow(image[i], 2);
  }
  E /= (size * size);
  return E;
}

int* histogram(float* image, int size, int bit) {
  int sizeHist = pow(2, bit+1); // bit+1, to deal with negatives
  int* hist = new int[sizeHist](); // initialize to zero
  int value;
  for (int i=0; i<size*size; i++) {
    value = image[i];
    if (value < 0) {
      value = sizeHist + value;
    }
    hist[value] += 1;
  }
  return hist;
}

float entropy(float* image, int size, int bit) {
  int* hist = histogram(image, size, bit);
  int sizeHist = pow(2, bit+1);
  float H = 0;
  float pi;
  for (int i=0; i<sizeHist; i++) {
    pi = float(hist[i]) / (size*size);
    if (pi > 0) {
      H -= pi * log2(pi);
    }
  }
  delete[] hist;
  return H;
}

float* downsample(float* image, int size, int downFactor) {
  int downSize = size/downFactor;
  float* downImage = new float [downSize*downSize];
  for (int i=0; i<downSize; i++) {
    for (int j=0; j<downSize; j++) {
      downImage[i*downSize +j] = image[(downFactor*i)*size + downFactor*j];
    }
  }
  return downImage;
}