#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;


void swapnum(int &i, int &j) {
  int temp = i;
  i = j;
  j = temp;
}

void f(const int& x) {
  int& y = const_cast<int&>(x);
  ++y;
}

int main(int argc, char const *argv[]) {

  float* ac = new float[3];
  for (int i=0; i<3; i++) {
    ac[i] = i;
  }
  float* co = new float[3];
  *co = *ac;

  std::cout << co[2] << '\n';
  std::cout << ac[2] << '\n';

  return 0;
}
