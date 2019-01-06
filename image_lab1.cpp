// Lab 1

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>

#define PI 3.14159265

using namespace std;


void store(float* image, int size, const char* name) {
  ofstream imageFile (name, ios::out | ios::binary);
  imageFile.write ((char*)image, sizeof(float)*size*size);
  imageFile.close();
}

void store(int* image, int size, const char* name) {
  ofstream imageFile (name, ios::out | ios::binary);
  imageFile.write ((char*)image, sizeof(int)*size*size);
  imageFile.close();
}

void store(char* image, int size, const char* name) {
  ofstream imageFile (name, ios::out | ios::binary);
  imageFile.write ((char*)image, sizeof(char)*size*size);
  imageFile.close();
}

float* basic_image(int size) {
  float* I = new float [size*size];
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++){
      I[i*size + j] = 0.5 + 0.5*cos(i*PI/32)*cos(j*PI/64);
    }
  }
  return I;
}

float* random_uniform_image(int size, float low, float high) {
  float* I = new float [size*size];
  default_random_engine generator;
  float MSE = 0.0;
  float mean = 0.5*(high - low);
  uniform_real_distribution<float> distribution(low, high);
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++){
      float random_var = distribution(generator);
      I[i*size + j] = random_var;
      MSE = MSE + pow(random_var - mean, 2);
    }
  }
  MSE = MSE / (pow(size, 2));
  cout << "MSE uniform: "<< MSE << endl;
  return I;
}

float* gaussian_image(int size, float mean, float std) {
  float* I = new float [size*size];
  default_random_engine generator;
  float MSE = 0.0;
  normal_distribution<float> distribution(mean, std);
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++){
      float random_var = distribution(generator);
      I[i*size + j] = random_var;
      MSE = MSE + pow(random_var - 0.5, 2);
    }
  }
  MSE = MSE / (pow(size, 2));
  cout << "MSE gaussian: "<< MSE << endl;
  return I;
}
