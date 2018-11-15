#include "algebra.h"
#include "keyutility.h"


void KeyUtility::KeyUtility(uint nk){
  /*int randomData = open("/dev/random", O_RDONLY);
if (randomData < 0)
{
    // something went wrong
}
else
{
    char myRandomData[50];
    size_t randomDataLen = 0;
    while (randomDataLen < sizeof myRandomData)
    {
        ssize_t result = read(randomData, myRandomData + randomDataLen, (sizeof myRandomData) - randomDataLen);
        if (result < 0)
        {
            // something went wrong
        }
        randomDataLen += result;
    }
    close(randomData);
}*/
}

uint32_t KeyUtility::rcon(const uint n) {
  uint8_t b = 2;
  uint32_t result = 0x00000000;
  for (uint i = 0; i < n-1; ++i) {
    b = xtimes(b);
  }

  result = result^b;
  result = result << 24;

  return result;

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

//Takes cares of the cyclic permutation
uint32_t KeyUtility::rotWord(const uint32_t w){

  uint32_t temp = w >> 24;
  uint32_t temp2 = w << 8;
  uint32_t result = temp^temp2;

  return result;

}

//initialise w somewhere to all 0s
uint32_t KeyUtility::expandKey(const uint8_t[] key, uint wordinkey, uint index){
  //uint Nb, Nr;
  uint32_t w[index];
  uint32_t temp;
  uint i = 0;
  while(i<wordinkey){
    for(int j =4*i;j<((4*i)+3);j++){
       w[i]= w[i]^key[j];
       w[i]<<8;
    }
    w[i]=w[i]^key[(4*i)+3];
    i++;
  }

  i=wordinkey;
  //where is Nb and Nr
  while (i<index/*<(Nb*(Nr+1))*/) {
    temp = w[i-1];
    if(i%wordinkey==0){
      temp = subWord(rotWord(temp)) ^ rcon(i/wordinkey);
    }
    else if(wordinkey>6 && i%wordinkey==4){
      temp = subWord(temp);
    }
    w[i]=w[i-wordinkey] ^ temp;
    i++;
  }
  return w[index-1];
}
