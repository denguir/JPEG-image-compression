#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include "image_lab1.h"
#include "image_lab2.h"
#define PI 3.14159265

using namespace std;

int main() {
  const char* inFile = "lena_256x256.raw";
  const char* noiseFile = "lena_noise.raw";
  const char* noiseFile2 = "lena_noise2.raw";
  const char* blurFile = "lena_blur.raw";
  const char* blurFile1 = "lena_blur1.raw";
  const char* sharpFile = "lena_sharp.raw";
  const char* sharpFile2 = "lena_sharp2.raw";
  const char* blurNoiseFile = "blur_noise.raw";
  const char* noiseBlurFile = "noise_blur.raw";

  float std_noise = 8.85;
  float std_kernel = 2.88;

  float* inImage = load(inFile, 256);

  float* noiseImage = noise(inImage, 256, 0.0, std_noise);
  store(noiseImage, 256, noiseFile);

  float* noiseImage2 = noise(inImage, 256, 0.0, 10*std_noise);
  store(noiseImage2, 256, noiseFile2);

  float* kernel = gaussian_kernel(3, pow(std_kernel, 2));

  float* bluredImage = blur1(inImage, 256, kernel, 3);
  store(bluredImage, 256, blurFile);

  float* bluredImage2 = blur2(inImage, 256, kernel, 3);
  store(bluredImage2, 256, blurFile1);

  float* sharpImage = sharpen(inImage, bluredImage2, 256, 0.5);
  store(sharpImage, 256, sharpFile);

  float* blurSharpImage = sharpen(bluredImage2, blur2(bluredImage2, 256, kernel, 3), 256, 0.5);
  store(sharpImage, 256, sharpFile2);

  float* bluredImageNoise = blur2(noiseImage, 256, kernel, 3);
  store(bluredImageNoise, 256, blurNoiseFile);

  float* noiseImageBlured = noise(bluredImage2, 256, 0.0, std_noise);
  store(noiseImageBlured, 256, noiseBlurFile);

  float PSNR_noise = psnr(inImage, noiseImage, 256, 255);
  std::cout << "PSNR noise, std = 2.88: \t" << PSNR_noise <<'\n';

  float PSNR_noise2 = psnr(inImage, noiseImage2, 256, 255);
  std::cout << "PSNR noise, std = 28.8: \t" << PSNR_noise2 <<'\n';

  float PSNR_blur = psnr(inImage, bluredImage, 256, 255);
  std::cout << "PSNR blur: \t" << PSNR_blur <<'\n';

  float PSNR_blur1 = psnr(inImage, bluredImage2, 256, 255);
  std::cout << "PSNR blur1: \t" << PSNR_blur1 <<'\n';

  float PSNR_noise_blur = psnr(inImage, noiseImageBlured, 256, 255);
  std::cout << "PSNR noise(blur): \t" << PSNR_noise_blur <<'\n';

  float PSNR_blur_noise = psnr(inImage, bluredImageNoise, 256, 255);
  std::cout << "PSNR blur(noise): \t" << PSNR_blur_noise <<'\n';

  float PSNR_sharp = psnr(inImage, sharpImage, 256, 255);
  std::cout << "PSNR sharp: \t" << PSNR_sharp <<'\n';

  float PSNR_sharp2 = psnr(inImage, blurSharpImage, 256, 255);
  std::cout << "PSNR sharp2: \t" << PSNR_sharp2 <<'\n';

  delete[] inImage;
  delete[] noiseImage;
  delete[] kernel;
  delete[] bluredImage;
  delete[] sharpImage;
  delete[] noiseImage2;
  delete[] bluredImage2;

  return 0;
}
