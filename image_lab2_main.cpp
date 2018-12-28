#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include "image_lab1.h"
#include "image_lab2.h"
#define PI 3.14159265

using namespace std;

int main(int argc, char const *argv[]) {
  const char* inFile = "lena_256x256.raw";
  const char* noiseFile = "lena_noise.raw";
  const char* blurFile = "lena_blur.raw";
  const char* sharpFile = "lena_sharp.raw";
  const char* blurFile2 = "lena_blur2.raw";

  float std_noise = 7.5;

  float* inImage = load(inFile, 256);
  float* noiseImage = noise(inImage, 256, 0.0, std_noise);
  store(noiseImage, 256, noiseFile);

  float* kernel = gaussian_kernel(3, 1.0);
  float* bluredImage = blur2(noiseImage, 256, kernel, 3);
  store(bluredImage, 256, blurFile);

  float* sharpImage = sharpen(inImage, bluredImage, 256);
  store(sharpImage, 256, sharpFile);

  float* bluredImage2 = blur2(inImage, 256, kernel, 3);
  float* noiseImage2 = noise(bluredImage2, 256, 0.0, std_noise);
  store(noiseImage2, 256, blurFile2);

  float PSNR_noise = psnr(inImage, noiseImage, 256, 255);
  std::cout << "PSNR noise: " << PSNR_noise <<'\n';

  float PSNR_blur = psnr(inImage, bluredImage, 256, 255);
  std::cout << "PSNR blur: " << PSNR_blur <<'\n';

  float PSNR_blur2 = psnr(inImage, noiseImage2, 256, 255);
  std::cout << "PSNR blur2: " << PSNR_blur2 <<'\n';

  float PSNR_sharp = psnr(inImage,sharpImage, 256, 255);
  std::cout << "PSNR sharp: " << PSNR_sharp <<'\n';

  delete[] inImage;
  delete[] noiseImage;
  delete[] kernel;
  delete[] bluredImage;
  delete[] sharpImage;
  delete[] noiseImage2;
  delete[] bluredImage2;

  return 0;
}
