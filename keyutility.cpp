#include "algebra.h"
#include "keyutility.h"

uint32_t KeyUtility::rcon(const uint n) {
  byte b = 2;
  for (uint i = 0; i < n-1; ++i) {
    b = xtimes(b);
  }

}

uint32_t KeyUtility::subWord(const uint32_t w){
  uint8_t result[4];

result[0] = (value & 0x000000ff);
result[1] = (value & 0x0000ff00) >> 8;
result[2] = (value & 0x00ff0000) >> 16;
result[3] = (value & 0xff000000) >> 24;

}
uint32_t KeyUtility::rotWord(const uint32_t w){

  uint32_t temp = w >> 24;
  uint32_t temp2 = w << 8;
  uint32_t result = temp^temp2;

  return result;

}

uint32_t KeyUtility::expandKey(const char[] key, uint roundNum){
  uint32_t temp;
  int i = 0;
  while
}
