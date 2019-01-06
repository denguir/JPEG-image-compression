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
#include "image_lab6.h"

#define PI 3.14159265
using namespace std;


int main(int argc, char const *argv[]) {
  const char* golombFile = "golomb.txt"; // for testing
  const char* DeltaFile = "delta_encoding.txt";
  const char* DeltaFile2 = "delta_encoding2.txt";
  const char* RLEFile = "rle_encoding.txt";
  const char* RLEFile2 = "rle_encoding2.txt";
  const char* bitStream = "bitstream.txt";

  std::cout << golomb(istringstream(golomb(42)).seekg(0)) << "=" << 42 << '\n';
  std::cout << golomb(ifstream(golombFile).seekg(0)) << "=" << -1 << '\n';

  compress(DeltaFile, RLEFile, bitStream);
  decompress(bitStream, DeltaFile2, RLEFile2, 256, 8);
  return 0;
}
