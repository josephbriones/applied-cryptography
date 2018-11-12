#include "algebra.h"
#include "keyutility.h"

uint32_t KeyUtility::rcon(const uint n) {
  byte b = 2;
  for (uint i = 0; i < n-1; ++i) {
    b = xtimes(b);
  }

}

uint32_t KeyUtility::subWord(const uint32_t w){
  uint8_t b[4],s[4];
  uint32_t result=0x00000000;

  b[0] = (w & 0x000000ff);
  b[1] = (w & 0x0000ff00) >> 8;
  b[2] = (w & 0x00ff0000) >> 16;
  b[3] = (w & 0xff000000) >> 24;

  for(int i =0;i<4;i++){
    s[i]=sbox(b[i]);
  }

  for(int i =0;i<3;i++){
    result = result^s[i];
    result<<8;
  }

  result=result^s[3];

  return result;

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
