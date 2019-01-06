// Lab 1

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <random>
#include "image_lab1.h"

#define PI 3.14159265

using namespace std;

main() {
    const char* basicFile = "basic_img.raw";
    const char* rndUniFile = "rnd_uni.raw";
    const char* rndGaussFile1 = "rnd_gauss1.raw";
    const char* rndGaussFile2 = "rnd_gauss2.raw";

    float* basicImg = basic_image(256);
    store(basicImg, 256, basicFile);

    float* rnd_uniform = random_uniform_image(256, 0.0, 1.0);
    store(rnd_uniform, 256, rndUniFile);
    float StdDevUni = 0.288;

    float* rnd_gauss1 = gaussian_image(256, 0.5, StdDevUni);
    store(rnd_gauss1, 256, rndGaussFile1);

    float* rnd_gauss2 = gaussian_image(256, 0.5, 10*StdDevUni);
    store(rnd_gauss2, 256, rndGaussFile2);
    return 0;
}
