#include <string>
#include <sstream>

bool IsBitSet(unsigned int, int);
unsigned int map(int);
int demap(int);
std::string golomb(int);
int golomb(std::istream&);
void compress(const char*, const char*, const char*);
void decompress(const char*, const char*, const char*, int, int);
void encode(float*, int, float*, int, const char*);
float* decode(const char*, int, float*, int);
