/* Encode an image and compress it into a bitStream using the JPEG algorithm.
  The bitStream can then be decompressed and decoded to retrieve the original image.
  The quantization used is the 50% JPEG by default (defined in qBlock)
*/

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
#include "image_lab6.h"

#define IMG_SIZE 256
#define BPP 32
#define BLOCK_SIZE 8

using namespace std;

std::ifstream::pos_type filesize(const char* filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

float compress_ratio(int imgSize, int bit, const char* filename) {
  float of = imgSize*imgSize*bit;
  int cf = filesize(filename);
  float ratio = of/cf;
  return ratio;
}

int main(int argc, char const *argv[]) {
  const char* inFile = "lena_256x256.raw";
  const char* outFile = "lena_out.raw";
  const char* bitStream = "bitstream.txt";

  float* imageIn = load(inFile, IMG_SIZE);
  float* imageOut;

  // Encoding
  float qBlock[64] = {16, 11, 10, 16, 24, 40, 51, 61,
                     12, 12, 14, 19, 26, 58, 60, 55,
                     14, 13, 16, 24, 40, 57, 69, 56,
                     14, 17, 22, 29, 51, 87, 80, 62,
                     18, 22, 37, 56, 68, 109, 103, 77,
                     24, 35, 55, 64, 81, 104, 113, 92,
                     49, 64, 78, 87, 103, 121, 120, 101,
                     72, 92, 95, 98, 112, 100, 103, 99};

  encode(imageIn, IMG_SIZE, qBlock, BLOCK_SIZE, bitStream);

  // Decoding
  imageOut = decode(bitStream, IMG_SIZE, qBlock, BLOCK_SIZE);
  store(imageOut, IMG_SIZE, outFile);

  // Compression Ratio
  std::cout << "The compression ratio is:" << '\n';
  std::cout << compress_ratio(IMG_SIZE, BPP, bitStream) << '\n';
  return 0;
}
