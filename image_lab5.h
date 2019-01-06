#include <string>
#include <sstream>

float* DCImage(float*, int, int);
float* GetCoeffBlock(float*, int, int, int, int);
void SetCoeffBlock(float*, int, float*, int, int, int);

float* GetBlock(float*, int, int, int, int);
void SetBlock(float*, int, float*, int, int, int);

float* zigzag(float*, int);
int* zigzag(int);

void DeltaEncoding(float*, int, const char*);
float* DeltaDecoding(const char*, int);

std::string BlockRLE(float*, int);
float* BlockRLD(std::istream&, int, int*);

void RunLengthEncoding(float*, int, int, const char*);
float* RunLengthDecoding(const char*, int, int);

void encode(float*, int, float*, int, const char*, const char*);
float* decode(int, float*, int, const char*, const char*);

float* merge(float*, float*, int, int);
float* downsample(float*, int, int);

float energy(float*, int);
float entropy(float*, int, int);

int* histogram(float*, int, int);
float* loadTxt(const char*, int);
