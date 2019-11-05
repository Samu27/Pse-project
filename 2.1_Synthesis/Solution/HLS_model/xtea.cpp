#include <iostream>
#include <fstream>
#include <stdint.h>

using namespace std;

void xtea (uint32_t word0, uint32_t word1, uint32_t key0,
          uint32_t key1, uint32_t key2, uint32_t key3,
          bool mode,
          uint32_t * result0, uint32_t * result1) {

  uint64_t sum;
  uint32_t i, delta, v0, v1, temp;
  v0 = word0;
  v1 = word1;
  sum = 0;
  *result0 = 0;
  *result1 = 0;

  if(mode == 0) {
    // encipher
    delta=0x9e3779b9;
    for (i=0; i < 32; i++) {


      if((sum & 3) == 0)
        temp = key0;
      else if((sum & 3) == 1)
        temp = key1;
      else if ((sum & 3) == 2)
        temp = key2;
      else temp = key3;

      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);

      sum += delta;

      if(((sum>>11) & 3) == 0)
        temp = key0;
      else if(((sum>>11) & 3) == 1)
        temp = key1;
      else if (((sum>>11) & 3) == 2)
        temp = key2;
      else temp = key3;

      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);
    }
  }

  else if (mode == 1) {
    // decipher
    delta = 0x9e3779b9;
    sum = delta*32;
    for (i=0; i<32; i++) {

      if(((sum>>11) & 3) == 0)
        temp = key0;
      else if(((sum>>11) & 3) == 1)
        temp = key1;
      else if (((sum>>11) & 3) == 2)
        temp = key2;
      else temp = key3;

      v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);

      sum -= delta;

      if((sum & 3) == 0)
        temp = key0;
      else if((sum & 3) == 1)
        temp = key1;
      else if ((sum & 3) == 2)
        temp = key2;
      else temp = key3;

      v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);

    }
  }
  *result0 = v0;
  *result1 = v1;
}
